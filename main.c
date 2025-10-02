#include "stm32f10x.h"  // Include STM32F10x device header: Defines all registers like RCC->CR, GPIOC->BSRR... (base addresses e.g., RCC=0x40021000, GPIOC=0x40011000). No SPL library, just CMSIS core.

/* Function prototypes (declare before use) */
void Delay(uint32_t t);     // Delay function (approximate ms), using busy loop.
void GPIO_Config(void);     // Config PC13 pin as push-pull output.
void Clock_Config(void);    // Set up system clock to HSI 8MHz.

int main(void) {
    Clock_Config();         // Call clock config first (power up the chip).
    GPIO_Config();          // Call GPIO config next (set up the LED "switch").
    
    while (1) {             // Infinite loop: Run forever (heart of the blink).
        GPIOC->BSRR = (1 << 13);   // Set bit 13 = 1 (high 3.3V) -> LED on (active high).
                                    // BSRR (Bit Set Reset Register, offset 0x10): Safe register, only sets bit 13 (bits 0-15=set, 16-31=reset), no side effects on other bits.
        
        Delay(25);          // Wait ~25ms (on time). Total cycle ~50ms (fast blink).
        
        GPIOC->BRR = (1 << 13);    // Reset bit 13 = 0 (low 0V) -> LED off.
                                    // BRR (Bit Reset Register, offset 0x14): Only resets bit 13, safe like a dedicated off switch.
        
        Delay(25);          // Wait ~25ms (off time).
    }
    // Never reaches return, due to while(1).
}

void Delay(uint32_t t) {
    unsigned int i, j;      // Local vars: i for outer loop (t ms), j for inner loop (clock cycles count).
    for (i = 0; i < t; i++) {  // Outer loop: Repeat t times (25 for 25ms).
        for (j = 0; j < 0x2AFF; j++);  // Inner loop: Count ~0x2AFF (11000 decimal) times, each ~ few clock cycles at 8MHz -> ~1ms per i loop.
                                        // Not 100% accurate (depends on compiler optimization/clock), but good for testing blink. Tune 0x2AFF if too slow/fast (increase = slower).
    }
    // No registers used, just CPU busy-waiting "counting sheep". Advanced: Use SysTick timer.
}

void GPIO_Config(void) {
    RCC->APB2ENR |= (1 << 4);  // Enable clock for GPIOC (set bit 4 = 1).
                                // APB2ENR (APB2 Enable Register, offset 0x18 in RCC base 0x40021000): "Circuit breaker" to power Port C (IOPCEN).
                                // |= bitwise OR: Sets bit 4=1 without clearing others. Without this, GPIOC is "asleep" and won't work.
    
    GPIOC->CRH &= ~((3 << 20) | (3 << 22));  // Clear config bits for PC13.
                                               // CRH (Configuration Register High, offset 0x04 in GPIOC base 0x40011000): Configures pins 8-15 (PC13 = pin 13, bits 20-23).
                                               // ~() bitwise NOT (invert), &= AND: Clears MODE13 (bits 20-21 = 00) and CNF13 (bits 22-23 = 00).
                                               // 3<<20 = 0b11 shifted left 20 (bits 20-21), 3<<22 = bits 22-23.
    
    GPIOC->CRH |= (1 << 21);   // Set MODE13 = 10b (output 2MHz).
                                // |= bitwise OR: Sets bit 21=1 (bit20=0 from previous clear) -> MODE13=10b (output push-pull at 2MHz speed, fast enough for LED).
                                // CNF13 remains 00b (push-pull: Strong output, like a bidirectional switch, not open-drain).
                                // Result: PC13 ready for high/low output.
}

void Clock_Config(void) {
    RCC->CR |= (1 << 0);       // Enable HSI oscillator (set bit 0 = HSION = 1).
                                // CR (Clock Control Register, offset 0x00 in RCC): "Main switch" to turn on HSI (High Speed Internal oscillator, 8MHz built-in).
    
    while (!(RCC->CR & (1 << 1)));  // Wait for HSI ready (bit 1 = HSIRDY = 1).
                                     // & bitwise AND: Check bit 1. While loop: "Wait until warmed up" (~few us).
    
    RCC->CFGR |= (0x4 << 8) | (0x4 << 11);  // Set prescalers to div2 for PCLK1/2.
                                             // CFGR (Clock Configuration Register, offset 0x04 in RCC): "Speed control panel".
                                             // 0x4 = 100b (divide by 2). Bits 8-10 (PPRE1): PCLK1 = SYSCLK/2 = 4MHz (for slow peripherals like UART/TIM).
                                             // Bits 11-13 (PPRE2): PCLK2 = SYSCLK/2 = 4MHz (for fast peripherals like GPIO/ADC).
                                             // |= bitwise OR: Sets without clearing other bits.
    
    RCC->CFGR &= ~(0x3 << 0);  // Select HSI as SYSCLK (set bits 0-1 = SW = 00).
                                // ~ (0x3<<0) clears bits 0-1, &= AND: Sets SW=00 (SYSCLK source = HSI 8MHz).
    
    while ((RCC->CFGR & (0x3 << 2)) != 0);  // Wait for switch complete (bits 2-3 = SWS = 00).
                                             // Check SWS == HSI. Loop waits (~few cycles).
    // Result: Chip running stably at 8MHz, ready for GPIO.
}
<div align="center">
  <h1> STM32F103 GPIO Blink LED Project</h1>
</div>
<br>


## Introduction:
This project is a simple example of how to control a blinking LED on the STM32F103C8T6/R6 microcontroller using register-level programming (without using the SPL library to avoid compatibility issues). The code configures GPIO PC13 as a push-pull output, sets the clock to HSI 8MHz, and toggles the LED in active-high mode (high = on).

- **Purpose**: Learn basic GPIO, clock configuration, and delay loops on STM32F1xx.
- **Tools**: Keil uVision (MDK-ARM), Proteus (for simulation).
- **Hardware**: STM32F103C8, LED + 330Ω resistor connected to PC13.

## Requirements
- **Software**:
  - Keil uVision 5.x (with STM32F1xx DFP pack, version 2.4.1+).
  - Proteus 8.x (for simulate).
- **Hardware (optional)**: STM32F103C8T6 board + ST-LINK for real flashing.
- **Knowledge**: Basic C, embedded systems, bare-metal.

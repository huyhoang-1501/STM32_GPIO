readme_content = """
# STM32F103 GPIO Blink LED Project

## Giới thiệu
Project này là một ví dụ đơn giản về cách điều khiển LED nháy (blink) trên vi điều khiển STM32F103C8T6/R6 sử dụng **register-level programming** (không dùng thư viện SPL để tránh lỗi compatibility). Code config GPIO PC13 làm output push-pull, set clock HSI 8MHz, và toggle LED active high (high = sáng).

- **Mục đích**: Học cơ bản GPIO, clock config, delay loop trên STM32F1xx.
- **Công cụ**: Keil uVision (MDK-ARM), Proteus (simulate).
- **Hardware**: STM32F103C8T6 (Blue Pill hoặc tương tự), LED + resistor 330Ω nối PC13.

## Yêu cầu
- **Phần mềm**:
  - Keil uVision 5.x (với STM32F1xx DFP pack, version 2.4.1+).
  - Proteus 8.x (để simulate).
- **Hardware (tùy chọn)**: Board STM32F103C8T6 + ST-LINK để flash thật.
- **Kiến thức**: Cơ bản C, embedded systems.

# STM32 Embedded Systems Projects

This repository contains my learning journey, driver implementations, and projects for various STM32 microcontrollers. It includes work on the **STM32F070**, **STM32F107VC**, and **STM32F103C8** (simulation).

## 📂 Repository Structure

The projects are organized by microcontroller series and peripheral functionality.

### 1. STM32F070 (Value Line)
*Recent driver implementations developed in Jan 2026.*

| Project Folder | Description |
| :--- | :--- |
| **`adc_F070`** | Analog-to-Digital Converter implementation for sensor reading. |
| **`led_blink_F070`** | Basic GPIO setup and "Hello World" LED blinking. |
| **`pwm_F070`** | Pulse Width Modulation setup (LED brightness/motor control). |
| **`uart_F070`** | Serial communication (UART) driver implementation. |

### 2. STM32F107VC & Peripheral Experiments
*Modular driver testing and specific component interfacing.*

* **`7-segment/`**: Logic for driving 7-segment displays.
* **`Buzzer/`**: GPIO control for audio feedback (Buzzer).
* **`EX33D project/`**: Specific project implementation (Jan 2026).
* **`LCD_deneme/`**: LCD interfacing trials and character display tests.
* **`PWM/`**: General PWM signal generation tests.
* **`Timers/`**: Hardware timer configurations for delays and interrupts.
* **`USART/`**: Universal Synchronous/Asynchronous Receiver-Transmitter tests.

### 3. Simulation
* **`proteus_F103c8/`**: Proteus simulation schematics and code for the STM32F103C8 (Blue Pill).

---

## 🛠 Tools & Environment

* **IDE:** Keil µVision 5 (MDK-ARM)
* **Simulation Software:** Proteus
* **Hardware:**
  * STM32F070RB
  * STM32F107VC
  * STM32F103C8 (Simulated)

## 🚀 Getting Started

1. **Clone the repo.**
2. **Open the Project:** Navigate to a project folder (e.g., `led_blink_F070`) and open the `.uvprojx` file in Keil µVision.
3. **Build:** Press `F7` or click the **Build** button to compile.
4. **Flash:** Connect your ST-Link and press `F8` (Download) to load the code onto the microcontroller.

## 📝 Notes

* Folders suffixed with `_deneme` indicate experimental or trial code ("deneme" means "trial" in Turkish).
* The `STM32F070` directory contains the most recent updates as of early 2026.

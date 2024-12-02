 # Embedded Operating System for Cortex-M33

This project implements a simple multitasking operating system for the ARM Cortex-M33 processor. The system is designed for educational purposes to explore and build fundamental OS concepts. It supports multitasking, synchronization, basic peripheral drivers, and more.

## Features

- **Task Management**:
  - Task creation and termination.
  - Round-robin scheduling with PendSV-based context switching.
  - Task states: `RUNNING`, `READY`, `WAITING`, and `SLEEPING`.
- **Semaphores**:
  - Binary and counting semaphores for task synchronization.
  - Mutex functionality for protecting critical sections.
- **Timer-Based Delay**:
  - Task delay using a timer interrupt (SysTick).
- **Peripheral Support**:
  - Virtual File System (VFS) for abstract hardware access.
  - Drivers for LEDs, buttons, and other peripherals (with interrupt support).
- **Modularity**:
  - Separation of application logic and kernel logic.

### Prerequisites
- **Hardware**: LPC55S69-EVK board.
- **IDE**: MCUXpresso IDE (or similar Eclipse-based IDE).
- **Toolchain**: ARM GCC or compatible compiler.

### Build and Run
1. Clone this repository:
   ```bash
   git clone https://github.com/HaytamBenz5/LPC55S69-OS.git

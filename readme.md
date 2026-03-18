# Modern Embedded System & RTOS Abstraction (C++17)

This project demonstrates a robust, testable, and hardware-agnostic architecture for embedded systems using **Modern C++ (C++17)** and **FreeRTOS**. It bridges the gap between low-level firmware development and high-level software engineering practices by implementing real-time scheduling and safety-critical monitoring.

## 1. Project Overview
- **Objective**: Decouple hardware dependencies from application logic to ensure high maintainability and testability in a Linux/RTOS environment.
- **Key Features**:
    - **HAL (Hardware Abstraction Layer)**: Interface-based design for sensors and actuators, allowing for easy mocking.
    - **RTOS Concurrency**: Implementation of the Producer-Consumer pattern using native FreeRTOS Queues.
    - **Fault Tolerance**: A multi-layered safety approach including a software watchdog and structured error reporting.
    - **Resource Management**: Utilization of RAII patterns and smart pointers to ensure memory safety in long-running systems.

## 2. Technical Stack
- **OS**: Linux (Ubuntu 22.04 via WSL2)
- **Framework**: FreeRTOS (POSIX Port)
- **Compiler**: GCC 12.x (Supporting C++17 standards)
- **Build System**: CMake 3.10+
- **Language**: C++17

## 3. Architecture & Design Principles

### Real-Time Task Scheduling
The system consists of independent tasks synchronized via thread-safe message passing:
- **Producer Tasks**: `TempTask` and `HumiTask` operate at independent sampling frequencies (1s and 3s respectively).
- **Consumer Task**: `ProcessingTask` runs at a higher priority to consume data, trigger alerts, and monitor system health.

### Reliability & Watchdog Mechanism
To ensure system stability, the architecture includes:
- **Software Watchdog**: The `ProcessingTask` monitors the heartbeat of every sensor. If no data is received within a 3-second window, a timeout alert is triggered.
- **Data Status Flags**: Every message sent via the queue includes a `DataStatus` enum (Valid, Error, OutOfRange) to prevent the application from acting on corrupted or failed sensor readings.

### Memory Safety
- **POD Structures**: Data is passed through queues using Plain Old Data (POD) structures to ensure memory layout consistency across tasks.
- **Heap Management**: Sensor objects and task configurations are safely allocated on the heap to prevent stack corruption or premature object destruction during the RTOS scheduler startup.

## 4. Getting Started

### Prerequisites
Install the essential build toolchain on your Ubuntu/WSL2 environment:
```bash
sudo apt update
sudo apt install -y build-essential gcc-12 g++-12 cmake git

## 4. Getting Started

### Prerequisites
Install the essential build toolchain on your Ubuntu/WSL2 environment:
```bash
sudo apt update
sudo apt install -y build-essential gcc-12 g++-12 cmake git
```
Set GCC 12 as the default compiler:
```bash
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-12 100
```
Build and Run
Execute the following commands in the project root:
```bash
# Generate build files
mkdir -p build && cd build
cmake ..

# Compile the project
make

# Run the simulation
./app
```

## 5. Simulation & Fault Injection
The application automatically simulates various runtime scenarios to demonstrate robustness:

Normal Data Stream: Real-time cross-task communication of sensor values.

Status Errors: Sensors periodically report DataStatus::Error to test the processor's guard clauses.

Sensor Timeout: The Temperature task is programmed to "fail" after a certain number of cycles, allowing you to observe the Watchdog triggering a timeout alert.


# Modern Embedded System Abstraction (C++17)

This project demonstrates a robust, testable, and hardware-agnostic architecture for embedded systems using **Modern C++ (C++17)**. It bridges the gap between low-level firmware development and high-level software engineering practices.

## 1. Project Overview
- **Objective**: Decouple hardware dependencies from application logic to ensure high maintainability and testability in a Linux/RTOS environment.
- **Key Features**:
    - **HAL (Hardware Abstraction Layer)**: Interface-based design for sensors and actuators.
    - **Safe Data Handling**: Utilization of `std::optional` for error handling instead of legacy return codes.
    - **Resource Management**: Implementation of RAII patterns and smart pointers to prevent memory leaks in long-running embedded systems.

## 2. Technical Stack
- **OS**: Linux (Ubuntu 22.04 via WSL2)
- **Compiler**: GCC 12.x (Supporting C++17/20 standards)
- **Build System**: CMake 3.10+
- **Language**: C++17

## 3. Architecture & Design Principles

### Hardware Abstraction
The project utilizes the `ISensor` interface, allowing the application to interact with various peripherals without knowing their specific implementation details. This is crucial for **Unit Testing (Mocking)** without physical hardware.



### Memory Safety
By using `std::unique_ptr`, the system ensures deterministic resource cleanup, which is critical for safety-critical systems where manual memory management (`malloc`/`free`) often leads to catastrophic failures.

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
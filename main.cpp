#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono> // time utilities
#include <atomic> // thread-safe
#include "MockSensor.hpp"

// flag to control the running state of the application
std::atomic<bool> keepRunning(true);

// 1. Read data from the sensor per a sec
void sensorTask(ISensor* sensor) {
    std::cout << "Starting sensor task for: " << sensor->getSensorName() << std::endl;
    
    while (keepRunning) {
        auto data = sensor->readData();
        if (data) {
            std::cout << "Data from " << sensor->getSensorName() << ": " << *data << std::endl;
        } else {
            std::cout << "Failed to read data from " << sensor->getSensorName() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // simulate delay between readings
    }

    std::cout << "Stopping sensor task for: " << sensor->getSensorName() << std::endl;
}

int main() {
    std::cout << "Starting Multi-thread Embedded Simulation..." << std::endl;

    // TODO: This is simplified for local testing/prototyping.
    // In the future, this should be refactored to use an Interface-based Factory 
    // or Dependency Injection (DI) to decouple 'main' from concrete 'Mock' classes.
    // e.g., std::unique_ptr<ISensor> tempSensor = SensorFactory::Create("Temperature");
    auto tempSensor = std::make_unique<MockSensor>("Temperature");
    auto humiSensor = std::make_unique<MockSensor>("Humidity");

    // thread starts. depending on the Interface
    std::thread t1(sensorTask, tempSensor.get());
    std::thread t2(sensorTask, humiSensor.get());

    std::this_thread::sleep_for(std::chrono::seconds(5)); // run for 5 seconds

    std::cout << "Stopping simulation..." << std::endl;
    keepRunning = false; // signal threads to stop

    t1.join();
    t2.join();
    
    std::cout << "Finishing SImulation..." << std::endl;
    return 0;
}
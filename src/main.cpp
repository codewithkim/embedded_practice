#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <atomic> // thread-safe
#include "MockSensor.hpp"
#include "MessageQueue.hpp"
#include "SensorData.hpp"

// flag to control the running state of the application
std::atomic<bool> keepRunning(true);

// shared message queue for sensor data
// TODO: DI
MessageQueue<SensorData> sensorDataQueue;

// 1. Read data from the sensor per a sec
void sensorTask(ISensor* sensor, SensorType type, std::string unit) {
    while (keepRunning) {
        auto data = sensor->readData();
        if (data) {
            SensorData msg{type, *data, std::move(unit)};
            sensorDataQueue.enqueue(msg);
        } else {    
            std::cout << "Failed to read data from " << sensor->getSensorName() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // simulate sensor read interval 
    }
}

void processingTask() {
    std::cout << "Processing Task Started..." << std::endl;
    while (keepRunning) {
        SensorData sensorData = sensorDataQueue.dequeue();
        switch (sensorData.type) {
            case SensorType::Temperature:
                std::cout << "Temperature: " << sensorData.value << " " << sensorData.unit << std::endl;
                if (sensorData.value > 30.0) {
                    std::cout << "Warning: High Temperature!" << std::endl;
                }
                break;
            case SensorType::Humidity:
                std::cout << "Humidity: " << sensorData.value << " " << sensorData.unit << std::endl;
                if (sensorData.value < 20.0) {
                    std::cout << "Warning: Low Humidity!" << std::endl;
                }
                break;
            case SensorType::Gas:
                std::cout << "Gas: " << sensorData.value << " " << sensorData.unit << std::endl;
                if (sensorData.value > 50.0) {
                    std::cout << "Warning: High Gas Level!" << std::endl;
                }
                break;
            default:
                std::cout << "Unknown Sensor Type!" << std::endl;
                break;
        }
    }
    std::cout << "Processing Task Stopped..." << std::endl;
}

int main() {
    // TODO: DI or refactoring to use an Interface-based Factory 
    // e.g., std::unique_ptr<ISensor> tempSensor = SensorFactory::Create("Temperature");
    auto tempSensor = std::make_unique<MockSensor>("Temperature");
    auto humiSensor = std::make_unique<MockSensor>("Humidity");

    // thread starts. depending on the Interface
    std::thread t1(sensorTask, tempSensor.get(), SensorType::Temperature, "C");
    std::thread t2(sensorTask, humiSensor.get(), SensorType::Humidity, "%");
    std::thread t3(sensorTask, tempSensor.get(), SensorType::Gas, "ppm"); // reusing temp sensor for gas data
    std::thread t_proc(processingTask);

    std::this_thread::sleep_for(std::chrono::seconds(5)); // run for 5 seconds

    keepRunning = false; // signal threads to stop

    sensorDataQueue.enqueue({SensorType::Gas, 0.0, ""}); // Dummy data to unblock processing thread if it's waiting

    t1.join();
    t2.join();
    t3.join();
    t_proc.join();
    
    return 0;
}
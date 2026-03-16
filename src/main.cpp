#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <atomic> // thread-safe
#include "MockSensor.hpp"
#include "MessageQueue.hpp"
#include "SensorData.hpp"
#include "FreeRTOS.h" // for vTaskDelay and pdMS_TO_TICKS
#include "task.h" // for vTaskDelay and pdMS_TO_TICKS

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

// wrapper for FreeRTOS task since FreeRTOS is based on C
void vSensorTaskWrapper(void* pvParameters) {
    MockSensor* sensor = static_cast<MockSensor*>(pvParameters);

    while(true) {
        auto data = sensor->readData();
        if (data) {
            printf("Sensor Data: %f\n", *data);
        } else {    
            printf("Failed to read data from %s\n", sensor->getSensorName().c_str());
        }

        // simulate sensor read interval 
        // explictly assigning the delay time 1000ms to a variable to avoid potential issues depending on the environment or FreeRTOS configuration
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
} 

int main() {
    auto tempSensor = std::make_unique<MockSensor>("Temp");

    xTaskCreate(vSensorTaskWrapper, "TempSensorTask", 2048, tempSensor.get(), 1, NULL);

    vTaskStartScheduler();
    return 0;
}
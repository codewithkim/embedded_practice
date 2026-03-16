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

void vProcessingTaskWrapper(void* pvParameters) {
    printf("Processing Task Started...\n");
    while (true) {
        SensorData sensorData = sensorDataQueue.dequeue();

        printf("[Processor] Received %s: %.2f %s\n", 
            (sensorData.type == SensorType::Temperature) ? "Temperature" : 
            (sensorData.type == SensorType::Humidity) ? "Humidity" : "Gas",
            sensorData.value, sensorData.unit.c_str()
        );
        
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
}

// wrapper for FreeRTOS task since FreeRTOS is based on C
void vSensorTaskWrapper(void* pvParameters) {
    printf("Sensor Task Started!\n");

    MockSensor* sensor = static_cast<MockSensor*>(pvParameters);

    while(true) {
        auto data = sensor->readData();
        if (data) {
            SensorData sensorData{sensor->getSensorType(), *data, "unit"};
            sensorDataQueue.enqueue(std::move(sensorData));
            printf("[%s] Sensor Data: %f\n", sensor->getSensorName().c_str(), *data);
        } else {    
            printf("Failed to read data from %s\n", sensor->getSensorName().c_str());
        }

        // simulate sensor read interval 
        // explictly assigning the delay time 1000ms to a variable to avoid potential issues depending on the environment or FreeRTOS configuration
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
} 

int main() {
    auto tempSensor = std::make_unique<MockSensor>("Temp", SensorType::Temperature);
    auto humiditySensor = std::make_unique<MockSensor>("Humidity", SensorType::Humidity);
    
    xTaskCreate(vSensorTaskWrapper, "TempSensorTask", 2048, tempSensor.get(), 1, NULL);
    xTaskCreate(vSensorTaskWrapper, "HumiditySensorTask", 2048, humiditySensor.get(), 1, NULL);

    xTaskCreate(vProcessingTaskWrapper, "ProcessingTask", 2048, NULL, 1, NULL);

    printf("Starting FreeRTOS Scheduler...\n");
    vTaskStartScheduler();

    return 0;
}
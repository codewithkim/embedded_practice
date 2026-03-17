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
#include "queue.h"
#include "AppConfig.hpp"


// flag to control the running state of the application
std::atomic<bool> keepRunning(true);

QueueHandle_t xSensorQueue = NULL;

// shared message queue for sensor data
MessageQueue<SensorData> sensorDataQueue;

void vProcessingTaskWrapper(void* pvParameters) {
    printf("Processing Task Ready...\n");
    fflush(stdout);
    SensorData sensorData;
    
    while (true) {
        // Wait for data (Blocked)
        if (xQueueReceive(xSensorQueue, &sensorData, portMAX_DELAY) == pdPASS) {
            printf("[Processor] Type: %d, Value: %.2f\n",
                (int)sensorData.type, sensorData.value);

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
        } else {
            printf("Failed to receive data from queue in processing task\n");
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

// wrapper for FreeRTOS task since FreeRTOS is based on C
void vSensorTaskWrapper(void* pvParameters) {
    printf("Sensor Task Started!\n");

    auto* config = static_cast<TaskConfig*>(pvParameters);
    MockSensor* sensor = config->sensor;
    QueueHandle_t targetQueue = config->queue;

    // independent freq
    TickType_t xDelay = pdMS_TO_TICKS(sensor->getIntervalMs());

    while(true) {
        auto data = sensor->readData();
        if (data) {
            SensorData sensorData{sensor->getSensorType(), *data, "unit"};
            
            if (xQueueSend(targetQueue, &sensorData, 0) != pdPASS) {
                printf("[Warning] Queue is full %s\n", sensor->getSensorName().c_str());
            } else {
                printf("[%s] Sensor Data: %.2f %s\n", sensor->getSensorName().c_str(), sensorData.value, sensorData.unit);
            }
            
        } else {    
            printf("Failed to read data from %s\n", sensor->getSensorName().c_str());
        }

        // simulate sensor read interval 
        // explictly assigning the delay time 1000ms to a variable to avoid potential issues depending on the environment or FreeRTOS configuration
        vTaskDelay(xDelay); 
    }
} 

int main() {
    auto tempSensor = new MockSensor("Temp", SensorType::Temperature, 1000);
    auto humiditySensor = new MockSensor("Humidity", SensorType::Humidity, 3000);

    // 10 sensor data items
    xSensorQueue = xQueueCreate(10, sizeof(SensorData));
    if (xSensorQueue == NULL) {
        printf("Failed to create FreeRTOS queue!\n");
        return 1;
    }
    
    auto* tempConfig = new TaskConfig{tempSensor, xSensorQueue};
    auto* humidityConfig = new TaskConfig{humiditySensor, xSensorQueue};

    BaseType_t status;

    status = xTaskCreate(vSensorTaskWrapper, "TempTask", 2048, tempConfig, 1, NULL);
    if (status != pdPASS) printf("!!! TempTask Creation Failed !!!\n");

    status = xTaskCreate(vSensorTaskWrapper, "HumiTask", 2048, humidityConfig, 1, NULL);
    if (status != pdPASS) printf("!!! HumiTask Creation Failed !!!\n");

    status = xTaskCreate(vProcessingTaskWrapper, "ProcTask", 2048, NULL, 2, NULL);
    if (status != pdPASS) printf("!!! ProcTask Creation Failed !!!\n");

    printf("Starting FreeRTOS Scheduler...\n");
    vTaskStartScheduler();

    return 0;
}
#pragma once
#include "ISensor.hpp"
#include <random>

class MockSensor : public ISensor {
private:
    std::string name;
    SensorType type;

public:
    MockSensor(std::string sensorName, SensorType sensorType) : name(sensorName), type(sensorType) {}

    bool initialize() override {
        // Simulate initialization logic (always successful for this mock)
        return true;
    }

    std::optional<double> readData() override {
        // Simulate reading data from the sensor (random value for demonstration)
        static std::default_random_engine generator;
        static std::uniform_real_distribution<double> distribution(0.0, 100.0);
        return distribution(generator);
    }

    std::string getSensorName() const override {
        return name;
    }

    SensorType getSensorType() const override {
        return type;
    }
};
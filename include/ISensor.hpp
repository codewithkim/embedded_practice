#pragma once
#include <optional>
#include <string_view>
#include "SensorData.hpp"

class ISensor {
public:
    virtual ~ISensor() = default; // artificial virtual destructor for interface

    virtual bool initialize() = 0; // Initialize the sensor, return true if successful
    virtual std::optional<double> readData() = 0; // Read data from the sensor, return std::nullopt if reading fails
    virtual std::string getSensorName() const = 0; // Get the name
    virtual SensorType getSensorType() const = 0; // Get the type of the sensor
};
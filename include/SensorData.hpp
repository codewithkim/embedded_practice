#pragma once
#include <string>

enum class SensorType {
    Temperature,
    Humidity,
    Gas
};

struct SensorData {
    SensorType type;
    double value;
    std::string unit; // for std::move
};
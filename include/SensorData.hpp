#pragma once
#include <string>
#include <cstring>

enum class SensorType {
    Temperature,
    Humidity,
    Gas
};

struct SensorData {
    SensorType type;
    double value;
    char unit[8];

    SensorData(SensorType t, double v, const char* u) : type(t), value(v) {
        std::strncpy(this->unit, u, sizeof(this->unit) - 1);
        this->unit[sizeof(this->unit) - 1] = '\0'; // Ensure null-termination
    }

    SensorData() : type(SensorType::Temperature), value(0.0) {
        unit[0] = '\0'; // Default to empty string
    }
};
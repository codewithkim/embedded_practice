#pragma once
#include <string>
#include <cstring>

enum class SensorType {
    Temperature,
    Humidity,
    Gas
};

enum class DataStatus {
    Vaild,
    Error,
    OutOfRange
};

struct SensorData {
    SensorType type;
    double value;
    char unit[8];
    DataStatus status;

    SensorData(SensorType t, double v, const char* u, DataStatus s = DataStatus::Vaild) : type(t), value(v), status(s) {
        std::strncpy(this->unit, u, sizeof(this->unit) - 1);
        this->unit[sizeof(this->unit) - 1] = '\0'; // Ensure null-termination
    }

    SensorData() : type(SensorType::Temperature), value(0.0), status(DataStatus::Error) {
        unit[0] = '\0'; // Default to empty string
    }

    DataStatus getStatus() const {
        return status;
    }
};
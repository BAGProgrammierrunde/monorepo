#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * Usage example
 *
 * In global context:
 * DebugMeasurement measurement;
 *
 * At the point to be measured:
 * measurement.start(); - starts the measurement
 * measurement.stop();  - stops the measurement and returns the elapsed time
 * measurement.print(); - prints the unique elapsed times
 */
class DebugMeasurement {
private:
    static constexpr size_t s_MaxFrameTimes = 1000;
    uint64_t m_Measurements[s_MaxFrameTimes] = {};
    size_t m_CurrentMeasurementIndex = 0;
    uint64_t m_StartTime = 0;

public:
    void start();
    uint64_t stop();
    void print() const;
};

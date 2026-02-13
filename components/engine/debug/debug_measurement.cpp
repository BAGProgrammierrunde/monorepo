#include "debug_measurement.h"

#include <esp_log.h>
#include <esp_timer.h>

#define TAG "Debug"

void DebugMeasurement::start() {
    m_StartTime = esp_timer_get_time();
}

uint64_t DebugMeasurement::stop() {
    const uint64_t frameTime = esp_timer_get_time() - m_StartTime;
    bool hasAlready = false;
    for (int i = 0; i < m_CurrentMeasurementIndex; ++i) {
        if (m_Measurements[i] == frameTime) {
            hasAlready = true;
        }
    }
    if (!hasAlready) {
        if (++m_CurrentMeasurementIndex < s_MaxFrameTimes) {
            m_Measurements[m_CurrentMeasurementIndex] = frameTime;
        } else {
            ESP_LOGE(TAG, "Too many unique debug measurements!");
        }

    }
    return frameTime;
}

void DebugMeasurement::print() const {
    ESP_LOGI(TAG, "Debug measurements:", m_CurrentMeasurementIndex);
    for (int i = 0; i < m_CurrentMeasurementIndex; ++i) {
        ESP_LOGI(TAG, "\t%lldus", m_Measurements[i]);
    }
    ESP_LOGI(TAG, "Debug measurements end", m_CurrentMeasurementIndex);
}

#include <iostream>
#include <freertos/FreeRTOS.h>
//#include <driver/gpio.h>
#include <driver/touch_sens.h>

#include "engine/bits.hpp"
#include "engine/ClockTimer.hpp"
#include "engine/IntervalTimer.hpp"
#include "main/TouchPin.hpp"
#include "main/Display.hpp"

extern "C" int app_main() {
    constexpr unsigned int SAMPLE_NUM = 1;

    touch_sensor_sample_config_t sample_cfg[SAMPLE_NUM] = {TOUCH_SENSOR_V2_DEFAULT_SAMPLE_CONFIG(10, TOUCH_VOLT_LIM_L_0V5, TOUCH_VOLT_LIM_H_2V7)};

    touch_sensor_config_t touch_cfg = TOUCH_SENSOR_DEFAULT_BASIC_CONFIG(SAMPLE_NUM, sample_cfg);
    touch_sensor_handle_t sens_handle = NULL;
    touch_sensor_new_controller(&touch_cfg, &sens_handle);

    touch_channel_config_t chan_cfg = {
        .active_thresh = {0},
        .charge_speed = TOUCH_CHARGE_SPEED_4,
        .init_charge_volt = TOUCH_INIT_CHARGE_VOLT_DEFAULT
    };
    int chan_id = 1;
    touch_channel_handle_t chan_handle = NULL;
    touch_sensor_new_channel(sens_handle, chan_id, &chan_cfg, &chan_handle);

    touch_proximity_config_t prox_cfg = {
        .proximity_chan = {chan_handle, chan_handle, chan_handle},
        .scan_times = 5
    };
    touch_sensor_config_proximity_sensing(sens_handle, &prox_cfg);
    
    touch_sensor_enable(sens_handle);

    touch_sensor_start_continuous_scanning(sens_handle);
    //touch_sensor_trigger_oneshot_scanning(sens_handle, -1);
    
    while (true)
    {
        /*uint32_t raw[SAMPLE_NUM] = {0};
        uint32_t benchmark[SAMPLE_NUM] = {0};

        touch_channel_read_data(chan_handle, TOUCH_CHAN_DATA_TYPE_SMOOTH, raw);
        touch_channel_read_data(chan_handle, TOUCH_CHAN_DATA_TYPE_BENCHMARK, benchmark);

        std::int32_t delta = static_cast<std::int32_t>(benchmark[0]) - static_cast<std::int32_t>(raw[0]);
        std::cout << "raw: " << raw[0] << " | bench: " << benchmark[0] << " | delta: " << (delta > 0 ? " " : "") << delta << std::endl;*/

        uint32_t data[SAMPLE_NUM] = {0};
        touch_channel_read_data(chan_handle, TOUCH_CHAN_DATA_TYPE_PROXIMITY, data);
        std::cout << "Data: " << data[0] << std::endl;

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    return 0;
}
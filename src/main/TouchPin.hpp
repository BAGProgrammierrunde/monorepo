#pragma once

namespace pa {
    struct TouchPin {
    private:
        static inline unsigned int sSampleCount = 1; // Only 1 supported by IDF... ?
        static inline std::uint32_t sSampleChargeTimes = 10;
        static inline touch_sensor_handle_t sSensor = nullptr;
        touch_channel_handle_t channel = nullptr;

    public:
        TouchPin() {}

        void init(unsigned int pPinChannelId) { // Look at IDF docs for the touch pin <--> channel id mapping for your controller
            touch_sensor_sample_config_t sampleConfig[sSampleCount] = {TOUCH_SENSOR_V2_DEFAULT_SAMPLE_CONFIG(sSampleChargeTimes, TOUCH_VOLT_LIM_L_0V5, TOUCH_VOLT_LIM_H_2V7)};
            touch_sensor_config_t sensorConfig = TOUCH_SENSOR_DEFAULT_BASIC_CONFIG(SampleCount, sampleConfig);
            touch_sensor_new_controller(&sensorConfig, &sensor);
            // Don't make this objects you create for every pin seperately but rather a static (singleton like but not really you know) class of the one single _sensor_ object where you add/manage them touch pins/channels and all

            touch_channel_config_t channelConfig = {
                .active_thresh = {0},
                .charge_speed = TOUCH_CHARGE_SPEED_4,
                .init_charge_volt = TOUCH_INIT_CHARGE_VOLT_DEFAULT
            };
            touch_sensor_new_channel(sensor, pPinChannelId, &channelConfig, &channel);
        
            /*touch_proximity_config_t prox_cfg = {
                .proximity_chan = {chan_handle, chan_handle, chan_handle},
                .scan_times = 5
            };
            touch_sensor_config_proximity_sensing(sens_handle, &prox_cfg);*/

            touch_sensor_enable(sensor);
        }

        unsigned int getValue() {
            return touch_sensor_enable(sensor);
        }
    };
}

    

    touch_sensor_start_continuous_scanning(sens_handle);
    //touch_sensor_trigger_oneshot_scanning(sens_handle, -1);
    

    /*uint32_t raw[SAMPLE_NUM] = {0};
    uint32_t benchmark[SAMPLE_NUM] = {0};

    touch_channel_read_data(chan_handle, TOUCH_CHAN_DATA_TYPE_SMOOTH, raw);
    touch_channel_read_data(chan_handle, TOUCH_CHAN_DATA_TYPE_BENCHMARK, benchmark);

    std::int32_t delta = static_cast<std::int32_t>(benchmark[0]) - static_cast<std::int32_t>(raw[0]);
    std::cout << "raw: " << raw[0] << " | bench: " << benchmark[0] << " | delta: " << (delta > 0 ? " " : "") << delta << std::endl;*/

    uint32_t data[SAMPLE_NUM] = {0};
    touch_channel_read_data(chan_handle, TOUCH_CHAN_DATA_TYPE_PROXIMITY, data);
    std::cout << "Data: " << data[0] << std::endl;
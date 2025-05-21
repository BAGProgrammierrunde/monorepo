#pragma once

#include <Arduino.h>
#include <DallasTemperature.h> // Includes OneWire.h too

// All just for display? Not just one library include?
#include <driver/rtc_io.h>
#include <DEV_Config.h>
#include <GUI_Paint.h>
#include <EPD.h>
#include <utility/EPD_2in9_V2.h>

#include <vector>
#include <memory>

#include "Data_Types/macros.hpp"
#include "Data_Types/poly_value.hpp"
#include "Data_Types/any_callable.hpp"
#include "Data_Types/IntervalAction.hpp"

#include "Components/Structure/Component.hpp"
#include "Components/Button/Button.hpp"
#include "Components/Joystick/Joystick.hpp"
#include "Components/Potentiometer/Potentiometer.hpp"
#include "Components/TemperatureSensor/TemperatureSensor.hpp"
#include "Components/PassiveBuzzer/predefinedNoteTones.hpp"
#include "Components/PassiveBuzzer/Note.hpp"
#include "Components/PassiveBuzzer/Melody.hpp"
#include "Components/PassiveBuzzer/PassiveBuzzer.hpp"
#include "Components/Display/Display.hpp"

//#include "Configuration.hpp"
//#include "LocalConfiguration.hpp"
#include "Features.hpp"
#include "Utility.hpp"

#include "Components/Structure/Device.hpp"

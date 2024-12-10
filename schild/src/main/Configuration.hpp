#pragma once

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)

// Wenn du eine Komponente verwenden möchtest, ersetze das 'false' mit einem 'true'
#define ENABLE_DISPLAY false
#define ENABLE_LEDS false
#define ENABLE_TEMPERATURE false
#define ENABLE_JOYSTICK false
#define ENABLE_POTENTIOMETER false
#define ENABLE_BUZZER false
#define ENABLE_BUTTONS false

#define BAUD_RATE 115200

/*
 * |          Anschlüsse           |
 * | ----------------------------- |
 * | Farbe  | Display | Firebeetle |
 * | ------ | ------- | ---------- |
 * | lila   | BUSY    | GPIO 22    |
 * | weiß   | RST     | GPIO 21    |
 * | grün   | DC      | GPIO 19    |
 * | orange | CS      | GPIO  0    |
 * | gelb   | CLK     | GPIO 18    |
 * | blau   | DIN     | GPIO 23    |
 * | braun  | GND     | GND        |
 * | grau   | VCC     | 3.3V       |
 * | ------ |-------- | ---------- |
 */

#define PIN_DISPLAY_BUSY 22
#define PIN_DISPLAY_RST 21
#define PIN_DISPLAY_DC 19
#define PIN_DISPLAY_CS 0
#define PIN_DISPLAY_CLK 18
#define PIN_DISPLAY_DIN 23

#define PIN_LED_1 27
#define PIN_LED_2 26

#define PIN_TEMPERATURE 17

#define PIN_JOYSTICK_X 25
#define PIN_JOYSTICK_Y 34
#define PIN_JOYSTICK_BUTTON 35

#define PIN_POTENTIOMETER 15

#define PIN_BUZZER 2

#define PIN_BUTTON_1 9
#define PIN_BUTTON_2 10
#define PIN_BUTTON_3 13
#define PIN_BUTTON_4 5

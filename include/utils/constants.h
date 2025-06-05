#pragma once

#include <Arduino.h>

namespace Constants {
// screen dimensions
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;

// pin definitions
constexpr int FAN_PIN = 26;  // number of PIN which controls the PMS fan
constexpr int PMS_RX_PIN = 16;
constexpr int PMS_TX_PIN = 17;

// time constants (in milliseconds)
constexpr int MIN_LOCK_TIME = 60000;      // 1 minute
constexpr int MEDIUM_LOCK_TIME = 300000;  // 5 minutes
constexpr int LONG_LOCK_TIME = 600000;    // 10 minutes

// serial rates
constexpr int DEBUG_SERIAL_BAUD = 115200;
constexpr int PMS_SERIAL_BAUD = 9600;

// PMS sensor constants
constexpr int PMS_BUFFER_SIZE = 32;
constexpr uint8_t PMS_START_BYTE = 0x42;

// sampling constants
constexpr int MIN_RANGE = 1;
constexpr int MAX_RANGE = 999;

// default values
constexpr int DEFAULT_FETCH_PERIOD = 10000;   // 10 seconds
constexpr int DEFAULT_MEASURE_PERIOD = 1000;  // 1 second
constexpr int DEFAULT_SAMPLES = 10;           // 10 samples
constexpr int DEFAULT_FAN_TIME = 30000;       // 30 seconds

// touch screen calibration
constexpr uint16_t TOUCH_CALIBRATION[5] = {275, 3620, 264, 3532, 1};

// buffer sizes
constexpr int JSON_BUFFER_SIZE = 512;
constexpr int LVGL_BUFFER_SIZE = 10;  // display buffer size (multiplier)

// other constants
constexpr int LVGL_TICK_PERIOD = 60;
constexpr int GMT_OFFSET_SEC = 7200;  // poland, winter - 3600, summer (DST) - 7200
constexpr int SD_CARD_PIN = 27;  // pin of SD_CS

// AQI standard breakpoints
constexpr float AQI_STANDARDS[5] = {21.0f, 61.0f, 101.0f, 141.0f, 201.0f};
}  // namespace Constants

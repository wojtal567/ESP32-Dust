#pragma once

#include <Arduino.h>
#include <lvgl.h>

namespace Constants {
// screen dimensions
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;

// pin definitions
constexpr int PMS_RX_PIN = 16;
constexpr int PMS_TX_PIN = 17;

// serial rates
constexpr int DEBUG_SERIAL_BAUD = 115200;
constexpr int PMS_SERIAL_BAUD = 9600;

// PMS sensor constants
constexpr int PMS_BUFFER_SIZE = 32;
constexpr uint8_t PMS_START_BYTE = 0x42;

// sampling constants
constexpr int MIN_RANGE = 1;
constexpr int MAX_RANGE = 999;

// touch screen calibration
constexpr uint16_t TOUCH_CALIBRATION[5] = {275, 3620, 264, 3532, 1};

// other constants
constexpr int GMT_OFFSET_SEC
    = 7200; // poland, winter - 3600, summer (DST) - 7200 // TODO ADD DROPDOWN

// UI Layout Constants
constexpr int LABEL_PARTICLE_SIZE_POS_X[7] = {9, 56, 103, 153, 198, 245, 288};
constexpr int CONT_PARTICLE_NUMBER_POS_X[6] = {18, 65, 112, 159, 206, 253};

constexpr lv_point_t MAIN_LINE_POINTS[] = {{18, 210}, {300, 210}};

constexpr lv_point_t DIVIDING_LINES_POINTS[][7] = {{{18, 205}, {18, 215}},
                                                   {{65, 205}, {65, 215}},
                                                   {{112, 205}, {112, 215}},
                                                   {{159, 205}, {159, 215}},
                                                   {{206, 205}, {206, 215}},
                                                   {{253, 205}, {253, 215}},
                                                   {{300, 205}, {300, 215}}};

} // namespace Constants

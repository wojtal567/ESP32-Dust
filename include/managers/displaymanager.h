#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

class DisplayManager
{
public:
    DisplayManager();

    void initialize(int screenWidth, int screenHeight, const uint16_t *touchCalibration);

    static void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color);

    static bool touchpadRead(lv_indev_drv_t *indevDriver, lv_indev_data_t *data);

    void handleTasks();

private:
    TFT_eSPI m_tft;
    static lv_disp_buf_t m_dispBuf;
    static lv_color_t m_buf[LV_HOR_RES_MAX * 10];
    static TFT_eSPI *m_tftInstance; // Pointer to TFT instance for static access
};

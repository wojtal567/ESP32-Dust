#include "managers/displaymanager.h"

// static members
lv_disp_buf_t DisplayManager::m_dispBuf;
lv_color_t DisplayManager::m_buf[LV_HOR_RES_MAX * 10];
TFT_eSPI* DisplayManager::m_tftInstance = nullptr;

DisplayManager::DisplayManager() {
    m_tftInstance = &m_tft;
}

void DisplayManager::initialize(int screenWidth, int screenHeight, const uint16_t* touchCalibration) {
    lv_init();
    
    m_tft.begin();
    m_tft.setRotation(3); // landscape 
    
    if (touchCalibration) {
        m_tft.setTouch(const_cast<uint16_t*>(touchCalibration));
    }
    
    lv_disp_buf_init(&m_dispBuf, m_buf, nullptr, LV_HOR_RES_MAX * 10);
    
    lv_disp_drv_t dispDrv;
    lv_disp_drv_init(&dispDrv);
    dispDrv.hor_res = screenWidth;
    dispDrv.ver_res = screenHeight;
    dispDrv.flush_cb = displayFlush;
    dispDrv.buffer = &m_dispBuf;
    lv_disp_drv_register(&dispDrv);
    
    lv_indev_drv_t indevDrv;
    lv_indev_drv_init(&indevDrv);
    indevDrv.type = LV_INDEV_TYPE_POINTER;
    indevDrv.read_cb = touchpadRead;
    lv_indev_drv_register(&indevDrv);
    
    lv_theme_t *theme = lv_theme_material_init(
        LV_THEME_DEFAULT_COLOR_PRIMARY, 
        LV_THEME_DEFAULT_COLOR_SECONDARY, 
        LV_THEME_DEFAULT_FLAG, 
        LV_THEME_DEFAULT_FONT_SMALL, 
        LV_THEME_DEFAULT_FONT_NORMAL, 
        LV_THEME_DEFAULT_FONT_SUBTITLE, 
        LV_THEME_DEFAULT_FONT_TITLE
    );
    lv_theme_set_act(theme);
}

void DisplayManager::displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color) {
    uint16_t c;
    
    m_tftInstance->startWrite();
    m_tftInstance->setAddrWindow(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1));
    
    for (int y = area->y1; y <= area->y2; y++) {
        for (int x = area->x1; x <= area->x2; x++) {
            c = color->full;
            m_tftInstance->writeColor(c, 1);
            color++;
        }
    }
    
    m_tftInstance->endWrite();
    lv_disp_flush_ready(disp);
}

bool DisplayManager::touchpadRead(lv_indev_drv_t *indevDriver, lv_indev_data_t *data) {
    uint16_t touchX, touchY;
    
    bool touched = m_tftInstance->getTouch(&touchX, &touchY, 600);
    
    if (!touched) {
        return false;
    }
    
    if (touchX > LV_HOR_RES_MAX || touchY > LV_VER_RES_MAX) {
        // touch out of bounds
        // Serial.println("Y or y outside of expected parameters..");
        // Serial.print("y:");
        // Serial.print(touchX);
        // Serial.print(" x:");
        // Serial.print(touchY);
        return false;
    } else {
        data->state = touched ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
        data->point.x = touchX;
        data->point.y = touchY;

        // Serial.print("Data x");
        // Serial.println(touchX);
        //
        // Serial.print("Data y");
        // Serial.println(touchY);
    }
    
    return false; // not buffer inputs
}

void DisplayManager::handleTasks() {
    lv_task_handler();
}

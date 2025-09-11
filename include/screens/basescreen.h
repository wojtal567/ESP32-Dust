#pragma once

#include <lvgl.h>

class BaseScreen
{
public:
    enum class ScreenType { MAIN, SETTINGS, WIFI, INFO, TIME_SETTINGS, SAMPLING_SETTINGS, LOCK };

    BaseScreen(ScreenType type);

    virtual ~BaseScreen();

    virtual void initialize() = 0;

    lv_obj_t *getScreenContainer() const;

protected:
    lv_obj_t *createButton(lv_obj_t *parent,
                           const lv_obj_t *copy,
                           lv_coord_t width,
                           lv_coord_t height,
                           lv_coord_t x,
                           lv_coord_t y,
                           lv_event_cb_t eventCallback);

    lv_obj_t *createLabel(lv_obj_t *parent,
                          const lv_obj_t *copy,
                          lv_coord_t x,
                          lv_coord_t y,
                          const char *text,
                          lv_color_t color = LV_COLOR_WHITE);

    lv_obj_t *createContainer(lv_obj_t *parent,
                              const lv_obj_t *copy,
                              lv_coord_t width,
                              lv_coord_t height,
                              lv_coord_t x,
                              lv_coord_t y);

    lv_obj_t *m_screenContainer;
    ScreenType m_type;

private:
    // Disable copy constructor and assignment operator
    BaseScreen(const BaseScreen &) = delete;
    BaseScreen &operator=(const BaseScreen &) = delete;
};

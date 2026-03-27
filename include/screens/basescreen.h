#pragma once

#include <lvgl.h>
#include "utils/constants.h"

// Forward declare the enum outside the template
enum class ScreenType {
    MAIN,
    SETTINGS,
    WIFI,
    INFO,
    TIME_SETTINGS,
    SAMPLING_SETTINGS,
    LOCK,
    LOADING
};

class BaseScreenInterface
{
public:
    BaseScreenInterface(ScreenType type)
        : m_type(type)
    {}
    virtual ~BaseScreenInterface() = default;

    virtual void initialize() = 0;
    virtual lv_obj_t *getScreenContainer() const = 0;

    ScreenType getType() const { return m_type; }

protected:
    ScreenType m_type;

private:
    // Disable copy constructor and assignment operator
    BaseScreenInterface(const BaseScreenInterface &) = delete;
    BaseScreenInterface &operator=(const BaseScreenInterface &) = delete;
};

template <typename DerivedScreen>
class BaseScreen : public BaseScreenInterface
{
public:
    explicit BaseScreen(ScreenType type)
        : BaseScreenInterface(type)
    {
        m_screenContainer = lv_cont_create(NULL, NULL);
        lv_obj_set_style_local_bg_color(m_screenContainer,
                                        LV_OBJ_PART_MAIN,
                                        LV_STATE_DEFAULT,
                                        LV_COLOR_BLACK);

        s_activeInstance = static_cast<DerivedScreen *>(this);
    }

    virtual ~BaseScreen()
    {
        if (s_activeInstance == static_cast<DerivedScreen *>(this)) {
            s_activeInstance = nullptr;
        }

        if (m_screenContainer) {
            lv_obj_del(m_screenContainer);
            m_screenContainer = nullptr;
        }
    }

    virtual void initialize() = 0;

    static DerivedScreen *getActiveInstance() { return s_activeInstance; }

    lv_obj_t *getScreenContainer() const override { return m_screenContainer; }

    lv_obj_t *createButton(lv_obj_t *parent,
                           const lv_obj_t *copy,
                           lv_coord_t width,
                           lv_coord_t height,
                           lv_coord_t x,
                           lv_coord_t y,
                           lv_event_cb_t eventCallback)
    {
        if (!parent)
            parent = m_screenContainer;

        lv_obj_t *btn = lv_btn_create(parent, copy);
        lv_obj_set_size(btn, width, height);
        lv_obj_set_pos(btn, x, y);
        if (eventCallback) {
            lv_obj_set_event_cb(btn, eventCallback);
        }
        return btn;
    }

    lv_obj_t *createLabel(lv_obj_t *parent,
                          const lv_obj_t *copy,
                          lv_coord_t x,
                          lv_coord_t y,
                          const char *text,
                          lv_color_t color = LV_COLOR_WHITE)
    {
        if (!parent)
            parent = m_screenContainer;

        lv_obj_t *label = lv_label_create(parent, copy);
        lv_obj_set_pos(label, x, y);
        lv_label_set_text(label, text);
        lv_obj_set_style_local_text_color(label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, color);
        return label;
    }

    lv_obj_t *createContainer(lv_obj_t *parent,
                              const lv_obj_t *copy,
                              lv_coord_t width,
                              lv_coord_t height,
                              lv_coord_t x,
                              lv_coord_t y)
    {
        if (!parent)
            parent = m_screenContainer;

        lv_obj_t *cont = lv_cont_create(parent, copy);
        lv_obj_set_size(cont, width, height);
        lv_obj_set_pos(cont, x, y);
        return cont;
    }

protected:
    lv_obj_t *m_screenContainer;
    ScreenType m_type;

private:
    static DerivedScreen *s_activeInstance;

    // Disable copy constructor and assignment operator
    BaseScreen(const BaseScreen &) = delete;
    BaseScreen &operator=(const BaseScreen &) = delete;
};

// Static member definition
template <typename DerivedScreen>
DerivedScreen *BaseScreen<DerivedScreen>::s_activeInstance = nullptr;

#include "screens/basescreen.h"
#include "utils/constants.h"

BaseScreen::BaseScreen(ScreenType type)
    : m_type(type)
{
    m_screenContainer = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(m_screenContainer,
                                    LV_OBJ_PART_MAIN,
                                    LV_STATE_DEFAULT,
                                    LV_COLOR_BLACK);
}

BaseScreen::~BaseScreen()
{
    if (m_screenContainer) {
        lv_obj_del(m_screenContainer);
        m_screenContainer = nullptr;
    }
}

lv_obj_t *BaseScreen::createButton(lv_obj_t *parent,
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

lv_obj_t *BaseScreen::createLabel(lv_obj_t *parent,
                                  const lv_obj_t *copy,
                                  lv_coord_t x,
                                  lv_coord_t y,
                                  const char *text,
                                  lv_color_t color)
{
    if (!parent)
        parent = m_screenContainer;

    lv_obj_t *label = lv_label_create(parent, copy);
    lv_obj_set_pos(label, x, y);
    lv_label_set_text(label, text);
    lv_obj_set_style_local_text_color(label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, color);
    return label;
}

lv_obj_t *BaseScreen::createContainer(lv_obj_t *parent,
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

lv_obj_t *BaseScreen::getScreenContainer() const
{
    return m_screenContainer;
}
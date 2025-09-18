#include <GlobalVariables.hpp>

#include "Utils/constants.h"
#include "Utils/stringConstants.h"
#include "managers/SensorManager.h"
#include "managers/stylemanager.h"
#include "utils/timeUtils.h"

lv_obj_t *my_lv_label_create(lv_obj_t *par, const lv_obj_t *copy, lv_coord_t x_position, lv_coord_t y_position, const char *text = "", lv_color_t color = LV_COLOR_WHITE)
{
    lv_obj_t *new_label = lv_label_create(par, copy);
    lv_obj_set_pos(new_label, x_position, y_position);
    lv_label_set_text(new_label, text);
    lv_obj_set_style_local_text_color(new_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, color);

    return new_label;
}

// Unlocking button clicked
static void unlockButton_task(lv_obj_t *obj, lv_event_t event)
{
    Serial.print(lv_btn_get_state(unlockButton));
    if (event == LV_EVENT_CLICKED)
        screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);
}

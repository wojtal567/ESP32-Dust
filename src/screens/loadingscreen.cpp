#include "screens/loadingscreen.h"

#include <lvgl.h>
#include "managers/stylemanager.h"

void LoadingScreen::initialize()
{
    lv_obj_t *label = createLabel(nullptr, nullptr, 10, 10, "Loading...", LV_COLOR_WHITE);
    StyleManager::applyFont22White(label);
}

#include <LVGLTasks.hpp>
#include "managers/stylemanager.h"

void lockScreen()
{
    contDateTimeAtLock = lv_cont_create(lockScr, NULL);
    lv_obj_set_auto_realign(contDateTimeAtLock, true);
    lv_obj_align(contDateTimeAtLock, NULL, LV_ALIGN_CENTER, 0, -40);
    lv_cont_set_fit(contDateTimeAtLock, LV_FIT_TIGHT);
    lv_cont_set_layout(contDateTimeAtLock, LV_LAYOUT_PRETTY_MID);
    StyleManager::applyTransparentContainer(contDateTimeAtLock);
    StyleManager::applyBorderlessContainer(contDateTimeAtLock);
    StyleManager::applyWhiteFont(contDateTimeAtLock);

    unlockButton = lv_btn_create(lockScr, nullptr); // setButton);//TODO BRAK POS
    labelUnlockButton = lv_label_create(unlockButton, NULL);
    lv_obj_set_style_local_text_font(unlockButton,
                                     LV_OBJ_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     StyleManager::getMonte16LockFont());
    lv_obj_align(unlockButton, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -25);
    lv_label_set_text(labelUnlockButton, StringConstants::UNLOCK_SYMBOL);
    lv_btn_set_fit(unlockButton, LV_FIT_TIGHT);
    lv_obj_set_event_cb(unlockButton, unlockButton_task);

    labelTimeLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelTimeLock,  "No WiFi connection");
    lv_label_set_align(labelTimeLock, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(labelTimeLock, NULL, LV_ALIGN_CENTER, 0, 30);

    labelDateLock = lv_label_create(contDateTimeAtLock, NULL);
    lv_label_set_text(labelDateLock, "");
    lv_obj_align(labelDateLock, NULL, LV_ALIGN_CENTER, 0, 0);

    wifiStatusAtLock = lv_label_create(lockScr, nullptr); // wifiStatusAtMain);
    lv_obj_align(wifiStatusAtLock, NULL, LV_ALIGN_CENTER, -36, 10);
    lv_label_set_text(wifiStatusAtLock, LV_SYMBOL_WIFI);
    wifiStatusAtLockWarning = my_lv_label_create(wifiStatusAtLock,
                                                 nullptr, // m_wifiStatusWarning from MainScreen);
                                                 6,
                                                 6,
                                                 LV_SYMBOL_CLOSE,
                                                 LV_COLOR_RED);

    sdStatusAtLock = lv_label_create(lockScr, nullptr); // m_sdStatus from MainScreen);
    lv_obj_align(sdStatusAtLock, NULL, LV_ALIGN_CENTER, 36, 10);
    lv_label_set_text(sdStatusAtLock, LV_SYMBOL_SD_CARD);
    sdStatusAtLockWarning = my_lv_label_create(sdStatusAtLock, wifiStatusAtLockWarning, 2, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    ledAtLock = lv_led_create(lockScr, NULL);
    lv_obj_set_size(ledAtLock, 13, 13);
    lv_obj_align(ledAtLock, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_led_set_bright(ledAtLock, 200);
    lv_obj_set_style_local_bg_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(ledAtLock, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
}
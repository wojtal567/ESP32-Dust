#include "screens/lockscreen.h"

#include <managers/stylemanager.h>
#include <utils/stringConstants.h>

LockScreen *LockScreen::s_activeInstance = nullptr;

LockScreen::LockScreen(ScreenManager &screenManager)
    : BaseScreen(ScreenType::LOCK)
    , m_screenManager(screenManager)
{
    s_activeInstance = this;
}

LockScreen::~LockScreen()
{
    if (s_activeInstance == this) {
        s_activeInstance = nullptr;
    }
}

void LockScreen::initialize()
{
    m_dateTimeContainer = lv_cont_create(m_screenContainer, NULL);
    lv_obj_set_auto_realign(m_dateTimeContainer, true);
    lv_obj_align(m_dateTimeContainer, NULL, LV_ALIGN_CENTER, 0, -40);
    lv_cont_set_fit(m_dateTimeContainer, LV_FIT_TIGHT);
    lv_cont_set_layout(m_dateTimeContainer, LV_LAYOUT_PRETTY_MID);
    StyleManager::applyTransparentContainer(m_dateTimeContainer);
    StyleManager::applyBorderlessContainer(m_dateTimeContainer);
    StyleManager::applyWhiteFont(m_dateTimeContainer);

    m_unlockButton = lv_btn_create(m_screenContainer, nullptr);
    StyleManager::applyTransparentButton(m_unlockButton);

    m_unlockButtonLabel = lv_label_create(m_unlockButton, NULL);
    lv_obj_set_style_local_text_font(m_unlockButton,
                                     LV_OBJ_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     StyleManager::getMonte16LockFont());
    lv_obj_align(m_unlockButton, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -25);
    lv_label_set_text(m_unlockButtonLabel, StringConstants::UNLOCK_SYMBOL);
    lv_btn_set_fit(m_unlockButton, LV_FIT_TIGHT);
    lv_obj_set_event_cb(m_unlockButton, unlockButtonCallback);

    m_labelTime = lv_label_create(m_dateTimeContainer, NULL);
    lv_label_set_align(m_labelTime, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(m_labelTime, NULL, LV_ALIGN_CENTER, 0, 30);

    m_labelDate = lv_label_create(m_dateTimeContainer, NULL);
    lv_label_set_text(m_labelDate, "");
    lv_obj_align(m_labelDate, NULL, LV_ALIGN_CENTER, 0, 0);

    m_wifiStatus = createLabel(m_screenContainer, nullptr, -36, 10, LV_SYMBOL_WIFI, LV_COLOR_WHITE);
    lv_obj_align(m_wifiStatus, NULL, LV_ALIGN_CENTER, -36, 10);
    m_wifiStatusWarning = createLabel(m_wifiStatus, nullptr, 6, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);
    StyleManager::applyTransparentContainer(m_wifiStatusWarning);
    StyleManager::applyFont12(m_wifiStatusWarning);

    m_sdStatus = createLabel(m_screenContainer, nullptr, 0, 0, LV_SYMBOL_SD_CARD, LV_COLOR_WHITE);
    lv_obj_align(m_sdStatus, NULL, LV_ALIGN_CENTER, 36, 10);
    m_sdStatusWarning
        = createLabel(m_sdStatus, m_wifiStatusWarning, 2, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    m_led = lv_led_create(m_screenContainer, NULL);
    lv_obj_set_size(m_led, 13, 13);
    lv_obj_align(m_led, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_led_set_bright(m_led, 200);
    lv_obj_set_style_local_bg_color(m_led, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(m_led, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(m_led, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
}

void LockScreen::updateDateTime(const String &dateStr, const String &timeStr)
{
    if (dateStr.length() > 0) {
        lv_label_set_text(m_labelDate, dateStr.c_str());
    } else {
        lv_label_set_text(m_labelDate, "");
    }

    if (timeStr.length() > 0) {
        lv_label_set_text(m_labelTime, timeStr.c_str());
    } else {
        lv_label_set_text(m_labelTime, "No Datetime set");
    }
}

void LockScreen::updateWifiSdStatus(bool wifiConnected, bool sdCardConnected)
{
    lv_obj_set_hidden(m_wifiStatusWarning, wifiConnected);
    lv_obj_set_hidden(m_sdStatusWarning, sdCardConnected);
}

void LockScreen::updateLedStatus(bool lastSampleSaved)
{
    lv_obj_set_style_local_bg_color(m_led,
                                    LV_LED_PART_MAIN,
                                    LV_STATE_DEFAULT,
                                    lastSampleSaved ? LV_COLOR_GREEN : LV_COLOR_RED);
    lv_obj_set_style_local_shadow_color(m_led,
                                        LV_LED_PART_MAIN,
                                        LV_STATE_DEFAULT,
                                        lastSampleSaved ? LV_COLOR_GREEN : LV_COLOR_RED);
}

void LockScreen::unlockButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleUnlockButtonEvent(btn, event);
    }
}

void LockScreen::handleUnlockButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        m_screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);
    }
}
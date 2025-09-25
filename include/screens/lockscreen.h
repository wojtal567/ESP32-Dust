#pragma once

#include "screens/basescreen.h"

#include "screens/screenmanager.h"

#include <Arduino.h>

class LockScreen : public BaseScreen<LockScreen>
{
public:
    LockScreen(ScreenManager &screenManager);
    ~LockScreen() override;

    void initialize() override;
    void updateDateTime(const String &dateStr, const String &timeStr);
    void updateWifiSdStatus(bool wifiConnected, bool sdCardConnected);
    void updateLedStatus(bool lastSampleSaved);

private:
    lv_obj_t *m_dateTimeContainer{nullptr};
    lv_obj_t *m_unlockButtonLabel{nullptr};
    lv_obj_t *m_unlockButton{nullptr};
    lv_obj_t *m_labelDate{nullptr};
    lv_obj_t *m_labelTime{nullptr};
    lv_obj_t *m_wifiStatus{nullptr};
    lv_obj_t *m_sdStatus{nullptr};
    lv_obj_t *m_wifiStatusWarning{nullptr};
    lv_obj_t *m_sdStatusWarning{nullptr};
    lv_obj_t *m_led{nullptr};

    ScreenManager &m_screenManager;

    // Static callbacks
    static void unlockButtonCallback(lv_obj_t *btn, lv_event_t event);
    void handleUnlockButtonEvent(lv_obj_t *btn, lv_event_t event);
};
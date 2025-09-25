#pragma once

#include "screens/basescreen.h"
#include "screens/screenmanager.h"

class SettingsScreen : public BaseScreen
{
public:
    SettingsScreen(ScreenManager &screenManager);
    ~SettingsScreen() override;

    void initialize() override;
private:
    lv_obj_t *m_backButton{nullptr};
    lv_obj_t *m_backButtonLabel{nullptr};
    lv_obj_t *m_screenTitleLabel{nullptr};
    lv_obj_t *m_wifiSettingsButton{nullptr};
    lv_obj_t *m_wifiSettingsButtonIcon{nullptr};
    lv_obj_t *m_wifiSettingsButtonLabel{nullptr};
    lv_obj_t *m_timeSettingsButton{nullptr};
    lv_obj_t *m_timeSettingsButtonIcon{nullptr};
    lv_obj_t *m_timeSettingsButtonLabel{nullptr};
    lv_obj_t *m_infoButton{nullptr};
    lv_obj_t *m_infoButtonIcon{nullptr};
    lv_obj_t *m_infoButtonLabel{nullptr};
    lv_obj_t *m_samplingSettingsButton{nullptr};
    lv_obj_t *m_samplingSettingsButtonIcon{nullptr};
    lv_obj_t *m_samplingSettingsButtonLabel{nullptr};

    ScreenManager &m_screenManager;
    
    // Static callbacks
    static void backButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void wifiSettingsButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void infoButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void timeSettingsButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void samplingSettingsButtonCallback(lv_obj_t *btn, lv_event_t event);
    
    void handleBackButtonEvent(lv_obj_t *btn, lv_event_t event);
    void handleWifiSettingsButtonEvent(lv_obj_t *btn, lv_event_t event);
    void handleInfoButtonEvent(lv_obj_t *btn, lv_event_t event);
    void handleTimeSettingsButtonEvent(lv_obj_t *btn, lv_event_t event);
    void handleSamplingSettingsButtonEvent(lv_obj_t *btn, lv_event_t event);
    
    static SettingsScreen* s_activeInstance;
};
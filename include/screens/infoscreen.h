#pragma once

#include "screens/basescreen.h"

#include <Arduino.h>
#include "utils/types.h"

class NetworkManager;
class ScreenManager;

class InfoScreen : public BaseScreen<InfoScreen>
{
public:
    InfoScreen(NetworkManager *networkManager,
               const Types::ConfigData &config,
               ScreenManager *screenManager);
    ~InfoScreen();

    void initialize() override;
    void updateConfigLabel();

private:
    lv_obj_t *m_backButton{nullptr};
    lv_obj_t *m_backButtonLabel{nullptr};
    lv_obj_t *m_labelAtBar{nullptr};
    lv_obj_t *m_wifiLabel{nullptr};
    lv_obj_t *m_wifiAddressLabel{nullptr};
    lv_obj_t *m_configLabel{nullptr};

    NetworkManager *m_networkManager;
    const Types::ConfigData &m_config;
    ScreenManager *m_screenManager;

    // Static callbacks
    static void backButtonCallback(lv_obj_t *btn, lv_event_t event);
    void handleBackButtonEvent(lv_obj_t *btn, lv_event_t event);
};
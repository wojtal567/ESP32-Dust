#pragma once

#include "screens/basescreen.h"
#include "managers/networkmanager.h"

#include <Arduino.h>

class InfoScreen : public BaseScreen
{
public:
    InfoScreen(const NetworkManager& networkManager);
    ~InfoScreen();

    void initialize() override;
    void updateConfigLabel(const String& configText);
    void updateWiFiStatus();

private:
    lv_obj_t *m_backButton{nullptr};
    lv_obj_t *m_backButtonLabel{nullptr};
    lv_obj_t *m_labelAtBar{nullptr};
    lv_obj_t *m_wifiLabel{nullptr};
    lv_obj_t *m_wifiAddressLabel{nullptr};
    lv_obj_t *m_configLabel{nullptr};

    const NetworkManager& m_networkManager;
};
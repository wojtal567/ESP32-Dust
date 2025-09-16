#pragma once

#include "managers/networkmanager.h"
#include "screens/basescreen.h"
#include "utils/types.h"

#include <Arduino.h>

class InfoScreen : public BaseScreen
{
public:
    InfoScreen(const NetworkManager &networkManager, const Types::ConfigData &config);
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

    const NetworkManager& m_networkManager;
    const Types::ConfigData &m_config;
};
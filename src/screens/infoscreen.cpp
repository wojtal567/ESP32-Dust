#include "screens/infoscreen.h"

#include "globalvariables.hpp"

#include <managers/stylemanager.h>

InfoScreen::InfoScreen(const NetworkManager &networkManager, const Types::ConfigData &config)
    : BaseScreen(ScreenType::INFO)
    , m_networkManager(networkManager)
    , m_config(config)
{}

InfoScreen::~InfoScreen() {}

void InfoScreen::initialize()
{
    m_backButton = createButton(m_screenContainer,
                                nullptr,
                                30,
                                15,
                                14,
                                10,
                                [](lv_obj_t *obj, lv_event_t event) {
                                    if (event == LV_EVENT_CLICKED) {
                                        screenManager.switchToScreen(
                                            BaseScreen::ScreenType::SETTINGS);
                                    }
                                });
    StyleManager::applyTransparentButton(m_backButton);

    m_backButtonLabel = lv_label_create(m_backButton, NULL);
    lv_label_set_text(m_backButtonLabel, LV_SYMBOL_LEFT);

    m_labelAtBar = createLabel(m_screenContainer, NULL, 216, 10, "Device info");

    m_wifiLabel = createLabel(m_screenContainer, NULL, 5, 53, "WiFi address: ");
    lv_obj_set_style_local_text_font(m_wifiLabel,
                                     LV_OBJ_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     &lv_font_montserrat_14);

    m_wifiAddressLabel = createLabel(m_screenContainer, m_wifiLabel, 115, 53, "");

    m_configLabel = createLabel(m_screenContainer, NULL, 5, 70, "");
}

void InfoScreen::updateConfigLabel()
{
    String current_config = (String) "SSID: " + m_config.ssid.c_str();
    current_config += (String) "\nNumber of samples: " + (String)m_config.numberOfSamples;
    if (m_config.lcdLockTime == -1)
        current_config += "\nLCD lock time: Never";
    if (m_config.lcdLockTime == 30000)
        current_config += "\nLCD lock time: 30s";
    if (m_config.lcdLockTime > 30000)
        current_config += "\nLCD lock time: " + (String)(m_config.lcdLockTime / 60000) + "m";
    current_config += (String) "\nFan running time before measure: " + m_config.turnFanTime / 1000
                      + "s\n";
    current_config += (String) "Time between measurments: " + m_config.measurePeriod / 1000
                      + "s\nMeasurements saving time: ";
    if (m_config.timeBetweenSavingSamples >= 3600000)
        current_config += m_config.timeBetweenSavingSamples / 60000 / 60 + (String) "h"
                          + (m_config.timeBetweenSavingSamples / 60000) % 60 + (String) "m"
                          + (m_config.timeBetweenSavingSamples / 1000) % 60 + "s";
    else if (m_config.timeBetweenSavingSamples >= 60000) {
        current_config += (m_config.timeBetweenSavingSamples / 60000) % 60 + (String) "m "
                          + (m_config.timeBetweenSavingSamples / 1000) % 60 + "s";
    } else {
        current_config += (m_config.timeBetweenSavingSamples / 1000) + (String) "s";
    }

    lv_label_set_text(m_configLabel, current_config.c_str());

    const bool isConnected = m_networkManager.isConnected();
    lv_label_set_text(m_wifiAddressLabel,
                      isConnected ? m_networkManager.getIpAddress().c_str() : "No WiFi connection");
}

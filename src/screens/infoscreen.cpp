#include "screens/infoscreen.h"

#include "globalvariables.hpp"

InfoScreen::InfoScreen(const NetworkManager &networkManager)
    : BaseScreen(ScreenType::INFO)
    , m_networkManager(networkManager)
{}

InfoScreen::~InfoScreen() {}

void InfoScreen::initialize()
{
    m_backButton = createButton(m_screenContainer,
                                backSettingsBtn,
                                30,
                                15,
                                14,
                                10,
                                [](lv_obj_t *obj, lv_event_t event) {
                                    if (event == LV_EVENT_CLICKED) {
                                        lv_disp_load_scr(settingsScr);
                                        // screenManager.switchToScreen(BaseScreen::ScreenType::SETTINGS);
                                    }
                                });
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

void InfoScreen::updateConfigLabel(const String &configText)
{
    lv_label_set_text(m_configLabel, configText.c_str());
}

void InfoScreen::updateWiFiStatus()
{
    const bool isConnected = m_networkManager.isConnected();
    lv_label_set_text(m_wifiAddressLabel,
                      isConnected
                      ? networkManager.getIpAddress().c_str() : "No WiFi connection");
}
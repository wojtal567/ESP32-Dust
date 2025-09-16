#include "screens/settingsscreen.h"

#include <managers/stylemanager.h>
#include <GlobalVariables.hpp>

SettingsScreen::SettingsScreen()
    : BaseScreen(ScreenType::SETTINGS)
{}

SettingsScreen::~SettingsScreen() {}

void SettingsScreen::initialize()
{

    m_backButton
        = createButton(m_screenContainer, NULL, 30, 15, 14, 10, [](lv_obj_t *obj, lv_event_t event) {
              if (event == LV_EVENT_CLICKED) {
                  screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);
              }
          });
    m_backButtonLabel = lv_label_create(m_backButton, NULL);
    lv_label_set_text(m_backButtonLabel, LV_SYMBOL_LEFT);
    StyleManager::applyTransparentButton(m_backButton);

    m_screenTitleLabel = createLabel(m_screenContainer, NULL, 239, 10, "Settings");

    m_wifiSettingsButton
        = createButton(m_screenContainer, NULL, 60, 60, 60, 38, [](lv_obj_t *obj, lv_event_t event) {
              if (event == LV_EVENT_CLICKED) {
                  screenManager.switchToScreen(BaseScreen::ScreenType::WIFI);
              }
          });
    m_wifiSettingsButtonIcon = lv_label_create(m_wifiSettingsButton, NULL);
    lv_label_set_text(m_wifiSettingsButtonIcon, StringConstants::WIFI_SYMBOL);
    StyleManager::applyTransparentButton(m_wifiSettingsButton);
    StyleManager::applyHugeFont(m_wifiSettingsButtonIcon);

    m_wifiSettingsButtonLabel = createLabel(m_screenContainer, NULL, 63, 103, "WiFi");
    StyleManager::applyFont20(m_wifiSettingsButtonLabel);

    m_infoButton = createButton(m_screenContainer,
                                m_wifiSettingsButton,
                                60,
                                60,
                                200,
                                38,
                                [](lv_obj_t *obj, lv_event_t event) {
                                    if (event == LV_EVENT_CLICKED) {
                                        infoScreen->updateConfigLabel();
                                        screenManager.switchToScreen(BaseScreen::ScreenType::INFO);
                                    }
                                });
    m_infoButtonIcon = lv_label_create(m_infoButton, NULL);
    lv_label_set_text(m_infoButtonIcon, StringConstants::INFO_SYMBOL);
    StyleManager::applyTransparentButton(m_infoButton);
    StyleManager::applyHugeFont(m_infoButtonIcon);

    m_infoButtonLabel = createLabel(m_screenContainer, m_wifiSettingsButtonLabel, 207, 103, "Info");

    m_timeSettingsButton = createButton(m_screenContainer,
                                        m_wifiSettingsButton,
                                        60,
                                        60,
                                        60,
                                        140,
                                        [](lv_obj_t *obj, lv_event_t event) {
                                            if (event == LV_EVENT_CLICKED) {
                                                timeSettingsScreen->updateData();
                                                screenManager.switchToScreen(
                                                    BaseScreen::ScreenType::TIME_SETTINGS);
                                            }
                                        });
    m_timeSettingsButtonIcon = lv_label_create(m_timeSettingsButton, NULL);
    lv_label_set_text(m_timeSettingsButtonIcon, StringConstants::CLOCK_SYMBOL);
    StyleManager::applyTransparentButton(m_timeSettingsButton);
    StyleManager::applyHugeFont(m_timeSettingsButtonIcon);

    m_timeSettingsButtonLabel = createLabel(m_screenContainer,
                                            m_wifiSettingsButtonLabel,
                                            65,
                                            200,
                                            "Time");

    m_samplingSettingsButton = createButton(m_screenContainer,
                                            m_wifiSettingsButton,
                                            60,
                                            60,
                                            200,
                                            140,
                                            [](lv_obj_t *obj, lv_event_t event) {
                                                if (event == LV_EVENT_CLICKED) {
                                                    lv_scr_load(samplingSettingsScr);
                                                    // screenManager.switchToScreen(BaseScreen::ScreenType::SAMPLING_SETTINGS);
                                                }
                                            });
    m_samplingSettingsButtonIcon = lv_label_create(m_samplingSettingsButton, NULL);
    lv_label_set_text(m_samplingSettingsButtonIcon, StringConstants::COGS_SYMBOL);
    StyleManager::applyTransparentButton(m_samplingSettingsButton);
    StyleManager::applyHugeFont(m_samplingSettingsButtonIcon);

    m_samplingSettingsButtonLabel = createLabel(m_screenContainer,
                                                m_wifiSettingsButtonLabel,
                                                180,
                                                200,
                                                "Sampling");
}
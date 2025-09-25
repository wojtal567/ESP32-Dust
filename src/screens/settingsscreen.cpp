#include "screens/settingsscreen.h"
#include "screens/infoscreen.h"
#include "screens/samplingsettingsscreen.h"
#include "screens/timesettingsscreen.h"

#include <managers/stylemanager.h>
#include <utils/stringConstants.h>

SettingsScreen *SettingsScreen::s_activeInstance = nullptr;

SettingsScreen::SettingsScreen(ScreenManager &screenManager)
    : BaseScreen(ScreenType::SETTINGS)
    , m_screenManager(screenManager)
{
    s_activeInstance = this;
}

SettingsScreen::~SettingsScreen() {}

void SettingsScreen::initialize()
{

    m_backButton = createButton(m_screenContainer, NULL, 30, 15, 14, 10, backButtonCallback);
    m_backButtonLabel = lv_label_create(m_backButton, NULL);
    lv_label_set_text(m_backButtonLabel, LV_SYMBOL_LEFT);
    StyleManager::applyTransparentButton(m_backButton);

    m_screenTitleLabel = createLabel(m_screenContainer, NULL, 239, 10, "Settings");

    m_wifiSettingsButton
        = createButton(m_screenContainer, NULL, 60, 60, 60, 38, wifiSettingsButtonCallback);
    m_wifiSettingsButtonIcon = lv_label_create(m_wifiSettingsButton, NULL);
    lv_label_set_text(m_wifiSettingsButtonIcon, StringConstants::WIFI_SYMBOL);
    StyleManager::applyTransparentButton(m_wifiSettingsButton);
    StyleManager::applyHugeFont(m_wifiSettingsButtonIcon);

    m_wifiSettingsButtonLabel = createLabel(m_screenContainer, NULL, 63, 103, "WiFi");
    StyleManager::applyFont20(m_wifiSettingsButtonLabel);

    m_infoButton
        = createButton(m_screenContainer, m_wifiSettingsButton, 60, 60, 200, 38, infoButtonCallback);
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
                                        timeSettingsButtonCallback);
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
                                            samplingSettingsButtonCallback);
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

void SettingsScreen::backButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleBackButtonEvent(btn, event);
    }
}

void SettingsScreen::wifiSettingsButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleWifiSettingsButtonEvent(btn, event);
    }
}

void SettingsScreen::infoButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleInfoButtonEvent(btn, event);
    }
}

void SettingsScreen::timeSettingsButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleTimeSettingsButtonEvent(btn, event);
    }
}

void SettingsScreen::samplingSettingsButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleSamplingSettingsButtonEvent(btn, event);
    }
}

void SettingsScreen::handleBackButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        m_screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);
    }
}

void SettingsScreen::handleWifiSettingsButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        m_screenManager.switchToScreen(BaseScreen::ScreenType::WIFI);
    }
}

void SettingsScreen::handleInfoButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        // Update the info screen before switching
        auto *infoScreen = static_cast<InfoScreen *>(
            m_screenManager.getScreen(BaseScreen::ScreenType::INFO));
        if (infoScreen) {
            infoScreen->updateConfigLabel();
        }
        m_screenManager.switchToScreen(BaseScreen::ScreenType::INFO);
    }
}

void SettingsScreen::handleTimeSettingsButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        // Update the time settings screen before switching
        auto *timeSettingsScreen = static_cast<TimeSettingsScreen *>(
            m_screenManager.getScreen(BaseScreen::ScreenType::TIME_SETTINGS));
        if (timeSettingsScreen) {
            timeSettingsScreen->updateData();
        }
        m_screenManager.switchToScreen(BaseScreen::ScreenType::TIME_SETTINGS);
    }
}

void SettingsScreen::handleSamplingSettingsButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        // Update the sampling settings screen before switching
        auto *samplingSettingsScreen = static_cast<SamplingSettingsScreen *>(
            m_screenManager.getScreen(BaseScreen::ScreenType::SAMPLING_SETTINGS));
        if (samplingSettingsScreen) {
            samplingSettingsScreen->updateData();
        }
        m_screenManager.switchToScreen(BaseScreen::ScreenType::SAMPLING_SETTINGS);
    }
}
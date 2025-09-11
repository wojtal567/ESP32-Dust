#include "screens/screenmanager.h"

ScreenManager::ScreenManager()
    : m_currentScreen(nullptr)
    , m_mainScreen(nullptr)
    , m_settingsScreen(nullptr)
    , m_wifiScreen(nullptr)
    , m_infoScreen(nullptr)
    , m_timeSettingsScreen(nullptr)
    , m_samplingSettingsScreen(nullptr)
    , m_lockScreen(nullptr)
{}

void ScreenManager::initialize(BaseScreen *mainScreen,
                               BaseScreen *settingsScreen,
                               BaseScreen *wifiScreen,
                               BaseScreen *infoScreen,
                               BaseScreen *timeSettingsScreen,
                               BaseScreen *samplingSettingsScreen,
                               BaseScreen *lockScreen)
{
    m_mainScreen = mainScreen;
    m_settingsScreen = settingsScreen;
    m_wifiScreen = wifiScreen;
    m_infoScreen = infoScreen;
    m_timeSettingsScreen = timeSettingsScreen;
    m_samplingSettingsScreen = samplingSettingsScreen;
    m_lockScreen = lockScreen;
}

bool ScreenManager::switchToScreen(BaseScreen::ScreenType type)
{
    BaseScreen *targetScreen = getScreen(type);
    if (!targetScreen) {
        return false; // Screen not found
    }

    // Show the new screen
    m_currentScreen = targetScreen;

    // Actually switch the LVGL display to the new screen
    if (targetScreen->getScreenContainer()) {
        lv_disp_load_scr(targetScreen->getScreenContainer());
    }

    return true;
}

BaseScreen *ScreenManager::getScreen(BaseScreen::ScreenType type)
{
    switch (type) {
    case BaseScreen::ScreenType::MAIN:
        return m_mainScreen;
    case BaseScreen::ScreenType::SETTINGS:
        return m_settingsScreen;
    case BaseScreen::ScreenType::WIFI:
        return m_wifiScreen;
    case BaseScreen::ScreenType::INFO:
        return m_infoScreen;
    case BaseScreen::ScreenType::TIME_SETTINGS:
        return m_timeSettingsScreen;
    case BaseScreen::ScreenType::SAMPLING_SETTINGS:
        return m_samplingSettingsScreen;
    case BaseScreen::ScreenType::LOCK:
        return m_lockScreen;
    default:
        return nullptr;
    }
}
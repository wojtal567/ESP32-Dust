#include "managers/screenmanager.h"

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

void ScreenManager::initialize(BaseScreenInterface *mainScreen,
                               BaseScreenInterface *settingsScreen,
                               BaseScreenInterface *wifiScreen,
                               BaseScreenInterface *infoScreen,
                               BaseScreenInterface *timeSettingsScreen,
                               BaseScreenInterface *samplingSettingsScreen,
                               BaseScreenInterface *lockScreen)
{
    m_mainScreen = mainScreen;
    m_settingsScreen = settingsScreen;
    m_wifiScreen = wifiScreen;
    m_infoScreen = infoScreen;
    m_timeSettingsScreen = timeSettingsScreen;
    m_samplingSettingsScreen = samplingSettingsScreen;
    m_lockScreen = lockScreen;
}

bool ScreenManager::switchToScreen(ScreenType type)
{
    BaseScreenInterface *targetScreen = getScreen(type);
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

BaseScreenInterface *ScreenManager::getScreen(ScreenType type)
{
    switch (type) {
    case ScreenType::MAIN:
        return m_mainScreen;
    case ScreenType::SETTINGS:
        return m_settingsScreen;
    case ScreenType::WIFI:
        return m_wifiScreen;
    case ScreenType::INFO:
        return m_infoScreen;
    case ScreenType::TIME_SETTINGS:
        return m_timeSettingsScreen;
    case ScreenType::SAMPLING_SETTINGS:
        return m_samplingSettingsScreen;
    case ScreenType::LOCK:
        return m_lockScreen;
    default:
        return nullptr;
    }
}

BaseScreenInterface *ScreenManager::getCurrentScreen()
{
    return m_currentScreen;
}

bool ScreenManager::hasActiveScreen()
{
    return m_currentScreen != nullptr;
}

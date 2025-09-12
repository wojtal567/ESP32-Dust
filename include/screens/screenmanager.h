#pragma once

#include "screens/basescreen.h"

class ScreenManager
{
public:
    ScreenManager();

    void initialize(BaseScreen *mainScreen,
                    BaseScreen *settingsScreen,
                    BaseScreen *wifiScreen,
                    BaseScreen *infoScreen = nullptr,
                    BaseScreen *timeSettingsScreen = nullptr,
                    BaseScreen *samplingSettingsScreen = nullptr,
                    BaseScreen *lockScreen = nullptr);

    bool switchToScreen(BaseScreen::ScreenType type);

    BaseScreen *getCurrentScreen() { return m_currentScreen; }

    BaseScreen *getScreen(BaseScreen::ScreenType type);

    bool hasActiveScreen() { return m_currentScreen != nullptr; }

private:
    BaseScreen *m_currentScreen = nullptr;
    BaseScreen *m_mainScreen = nullptr;
    BaseScreen *m_settingsScreen = nullptr;
    BaseScreen *m_wifiScreen = nullptr;
    BaseScreen *m_infoScreen = nullptr;
    BaseScreen *m_timeSettingsScreen = nullptr;
    BaseScreen *m_samplingSettingsScreen = nullptr;
    BaseScreen *m_lockScreen = nullptr;
};
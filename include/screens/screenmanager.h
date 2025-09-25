#pragma once

#include "screens/basescreen.h"

class ScreenManager
{
public:
    ScreenManager();

    void initialize(BaseScreenInterface *mainScreen,
                    BaseScreenInterface *settingsScreen,
                    BaseScreenInterface *wifiScreen,
                    BaseScreenInterface *infoScreen,
                    BaseScreenInterface *timeSettingsScreen,
                    BaseScreenInterface *samplingSettingsScreen,
                    BaseScreenInterface *lockScreen);

    bool switchToScreen(ScreenType type);

    BaseScreenInterface *getCurrentScreen() { return m_currentScreen; }

    BaseScreenInterface *getScreen(ScreenType type);

    bool hasActiveScreen() { return m_currentScreen != nullptr; }

private:
    BaseScreenInterface *m_currentScreen = nullptr;
    BaseScreenInterface *m_mainScreen = nullptr;
    BaseScreenInterface *m_settingsScreen = nullptr;
    BaseScreenInterface *m_wifiScreen = nullptr;
    BaseScreenInterface *m_infoScreen = nullptr;
    BaseScreenInterface *m_timeSettingsScreen = nullptr;
    BaseScreenInterface *m_samplingSettingsScreen = nullptr;
    BaseScreenInterface *m_lockScreen = nullptr;
};
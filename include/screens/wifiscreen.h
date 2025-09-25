#pragma once

#include "screens/basescreen.h"

#include "managers/rtcmanager.h"
#include "screens/screenmanager.h"
#include "utils/types.h"

class NetworkManager;

class WifiScreen : public BaseScreen
{
public:
    WifiScreen(const Types::ConfigData &config,
               NetworkManager *networkManager,
               RTCManager &rtc,
               ScreenManager &screenManager);
    ~WifiScreen();

    void initialize() override;

private:
    void handleTextAreaEvent(lv_obj_t *ta, lv_event_t event);
    void handleKeyboardEvent(lv_obj_t *kb, lv_event_t event);
    void handleCancelButtonEvent(lv_obj_t *btn, lv_event_t event);
    void handleShowPasswordButtonEvent(lv_obj_t *btn, lv_event_t event);
    void handleConnectButtonEvent(lv_obj_t *btn, lv_event_t event);

    // Static wrappers for LVGL callbacks
    static void textAreaCallback(lv_obj_t *ta, lv_event_t event);
    static void keyboardCallback(lv_obj_t *kb, lv_event_t event);
    static void cancelButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void showPasswordButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void connectButtonCallback(lv_obj_t *btn, lv_event_t event);
    // Instance tracking for callbacks
    static WifiScreen* s_activeInstance;

    lv_obj_t *m_cancelButton{nullptr};
    lv_obj_t *m_cancelButtonLabel{nullptr};
    lv_obj_t *m_barLabel{nullptr};
    lv_obj_t *m_ssidLabel{nullptr};
    lv_obj_t *m_ssidTextArea{nullptr};
    lv_obj_t *m_passwordLabel{nullptr};
    lv_obj_t *m_passwordTextArea{nullptr};
    lv_obj_t *m_showPasswordButton{nullptr};
    lv_obj_t *m_showPasswordButtonLabel{nullptr};
    lv_obj_t *m_connectButton{nullptr};
    lv_obj_t *m_connectButtonLabel{nullptr};
    lv_obj_t *m_keyboard{nullptr};

    Types::ConfigData m_config;
    NetworkManager *m_networkManager;
    RTCManager &m_rtcManager;
    ScreenManager &m_screenManager;
};
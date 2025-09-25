#include "screens/wifiscreen.h"

#include "managers/networkmanager.h"
#include "managers/stylemanager.h"
#include "utils/constants.h"
#include "utils/stringConstants.h"

WifiScreen::WifiScreen(const Types::ConfigData &config,
                       NetworkManager *networkManager,
                       RTCManager &rtc,
                       ScreenManager &screenManager)
    : BaseScreen<WifiScreen>(ScreenType::WIFI)
    , m_config(config)
    , m_networkManager(networkManager)
    , m_rtcManager(rtc)
    , m_screenManager(screenManager)
{}

WifiScreen::~WifiScreen() {}

void WifiScreen::initialize()
{
    m_cancelButton = createButton(m_screenContainer, nullptr, 30, 15, 14, 10, cancelButtonCallback);

    StyleManager::applyTransparentButton(m_cancelButton);

    m_cancelButtonLabel = lv_label_create(m_cancelButton, NULL);
    lv_label_set_text(m_cancelButtonLabel, LV_SYMBOL_LEFT);
    StyleManager::applyTransparentButton(m_cancelButton);
    StyleManager::applyWhiteFont(m_cancelButton);

    m_barLabel = createLabel(m_screenContainer, NULL, 201, 10, "WiFi settings");

    m_ssidLabel = createLabel(m_screenContainer, NULL, 5, 53, "SSID: ");

    m_ssidTextArea = lv_textarea_create(m_screenContainer, NULL);
    lv_textarea_set_text(m_ssidTextArea, "");
    lv_textarea_set_pwd_mode(m_ssidTextArea, false);
    lv_textarea_set_one_line(m_ssidTextArea, true);
    lv_obj_set_event_cb(m_ssidTextArea, textAreaCallback);
    lv_textarea_set_cursor_hidden(m_ssidTextArea, true);
    lv_obj_set_size(m_ssidTextArea, 140, 34);
    lv_obj_set_pos(m_ssidTextArea, 100, 45);
    StyleManager::applyBorderlessContainer(m_ssidTextArea);

    m_passwordLabel = createLabel(m_screenContainer, NULL, 5, 92, "Password: ");
    m_passwordTextArea = lv_textarea_create(m_screenContainer, NULL);
    lv_textarea_set_text(m_passwordTextArea, "");
    lv_textarea_set_pwd_show_time(m_passwordTextArea, 5000);
    lv_textarea_set_pwd_mode(m_passwordTextArea, true);
    lv_textarea_set_one_line(m_passwordTextArea, true);
    lv_obj_set_event_cb(m_passwordTextArea, textAreaCallback);
    lv_textarea_set_cursor_hidden(m_passwordTextArea, true);
    lv_obj_set_size(m_passwordTextArea, 140, 34);
    lv_obj_set_pos(m_passwordTextArea, 100, 85);
    StyleManager::applyBorderlessContainer(m_passwordTextArea);

    m_showPasswordButton
        = createButton(m_screenContainer, NULL, 75, 43, 243, 85, showPasswordButtonCallback);
    m_showPasswordButtonLabel = lv_label_create(m_showPasswordButton, NULL);
    lv_label_set_text(m_showPasswordButtonLabel, LV_SYMBOL_EYE_OPEN);
    StyleManager::applyTransparentButton(m_showPasswordButton);
    StyleManager::applyWhiteFont(m_showPasswordButtonLabel);

    m_connectButton = createButton(m_screenContainer, NULL, 75, 43, 243, 43, connectButtonCallback);
    m_connectButtonLabel = lv_label_create(m_connectButton, NULL);
    lv_label_set_text(m_connectButtonLabel, "Connect");
    StyleManager::applyWhiteButton(m_connectButton);
}

void WifiScreen::handleTextAreaEvent(lv_obj_t *ta, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        if (ta == m_ssidTextArea) {
            lv_textarea_set_cursor_hidden(m_ssidTextArea, false);
            if (m_passwordTextArea) {
                lv_textarea_set_cursor_hidden(m_passwordTextArea, true);
            }
        }
        if (m_passwordTextArea && ta == m_passwordTextArea) {
            lv_textarea_set_cursor_hidden(m_passwordTextArea, false);
            lv_textarea_set_cursor_hidden(m_ssidTextArea, true);
        }

        if (m_keyboard == NULL) {
            m_keyboard = lv_keyboard_create(lv_scr_act(), NULL);
            lv_obj_set_size(m_keyboard, LV_HOR_RES, LV_VER_RES / 2);
            lv_obj_set_event_cb(m_keyboard, keyboardCallback);
            lv_keyboard_set_textarea(m_keyboard, ta);
        } else {
            lv_keyboard_set_textarea(m_keyboard, ta);
        }
    }
}

void WifiScreen::handleKeyboardEvent(lv_obj_t *kb, lv_event_t event)
{
    if (event != LV_EVENT_CANCEL) {
        lv_keyboard_def_event_cb(kb, event);
    }
}

void WifiScreen::handleCancelButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        m_screenManager.switchToScreen(ScreenType::SETTINGS);
        lv_textarea_set_text(m_ssidTextArea, "");
        lv_textarea_set_text(m_passwordTextArea, "");
    }
}

void WifiScreen::handleShowPasswordButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        if (lv_textarea_get_pwd_mode(m_passwordTextArea)) {
            lv_textarea_set_pwd_mode(m_passwordTextArea, false);
            lv_label_set_text(m_showPasswordButtonLabel, LV_SYMBOL_EYE_CLOSE);
        } else {
            lv_textarea_set_pwd_mode(m_passwordTextArea, true);
            lv_textarea_set_pwd_show_time(m_passwordTextArea, 1);
            lv_textarea_set_text(m_passwordTextArea, lv_textarea_get_text(m_passwordTextArea));
            lv_textarea_set_pwd_show_time(m_passwordTextArea, 5000);
            lv_label_set_text(m_showPasswordButtonLabel, LV_SYMBOL_EYE_OPEN);
        }
    }
}

void WifiScreen::handleConnectButtonEvent(lv_obj_t *btn, lv_event_t event)
{

    if (event == LV_EVENT_CLICKED
        && ((lv_textarea_get_text(m_ssidTextArea) != NULL
             && lv_textarea_get_text(m_ssidTextArea)[0] != '\0')
            || (lv_textarea_get_text(m_passwordTextArea) != NULL
                && lv_textarea_get_text(m_passwordTextArea)[0] != '\0'))) {
        const String ssid = lv_textarea_get_text(m_ssidTextArea);
        const String pwd = lv_textarea_get_text(m_passwordTextArea);

        m_config.ssid = ssid.c_str();
        m_config.password = pwd.c_str();

        Serial.println(m_config.ssid.c_str());
        m_networkManager->connectAsync(m_config.ssid.c_str(), m_config.password.c_str());

        m_networkManager->saveConfig(m_config, StringConstants::CONFIG_FILE_PATH);

        m_networkManager->printConfig(StringConstants::CONFIG_FILE_PATH);
        bool connected = m_networkManager->connect();
        if (connected) {
            Serial.println("btn_connect -> connected to Wi-Fi! IP: "
                           + m_networkManager->getIpAddress());
            m_rtcManager.syncWithNTP(StringConstants::NTP_SERVER, Constants::GMT_OFFSET_SEC);
            m_networkManager->setupServer();
        } else {
            Serial.println(
                "btn_connect -> can't connect. Probably you have entered wrong credentials.");
        }
        m_screenManager.switchToScreen(ScreenType::MAIN);
        lv_textarea_set_text(m_ssidTextArea, "");
        lv_textarea_set_text(m_passwordTextArea, "");
    }
}

// Static callback wrappers
void WifiScreen::textAreaCallback(lv_obj_t *ta, lv_event_t event)
{
    if (auto instance = getActiveInstance()) {
        instance->handleTextAreaEvent(ta, event);
    }
}

void WifiScreen::keyboardCallback(lv_obj_t *kb, lv_event_t event)
{
    if (auto instance = getActiveInstance()) {
        instance->handleKeyboardEvent(kb, event);
    }
}

void WifiScreen::cancelButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto instance = getActiveInstance()) {
        instance->handleCancelButtonEvent(btn, event);
    }
}

void WifiScreen::showPasswordButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto instance = getActiveInstance()) {
        instance->handleShowPasswordButtonEvent(btn, event);
    }
}

void WifiScreen::connectButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto instance = getActiveInstance()) {
        instance->handleConnectButtonEvent(btn, event);
    }
}

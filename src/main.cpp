#include <Arduino.h>
#include <HTTPClient.h>
#include <time.h>

#include <api/httpapi.h>
#include <managers/stylemanager.h>
#include <managers/taskmanager.h>
#include "GlobalVariables.hpp"
// #include <LVGLTasks.hpp> // Commented out - functions moved to TaskManager

void setup()
{
    sqlite3_initialize();
    // Serial debug
    Serial.begin(Constants::DEBUG_SERIAL_BAUD);
    Serial2.begin(Constants::PMS_SERIAL_BAUD,
                  SERIAL_8N1,
                  Constants::PMS_RX_PIN,
                  Constants::PMS_TX_PIN);

    // Initialize sensors
    sensorManager.initialize(&Serial, &Serial2);

    // Initialize display using DisplayManager
    displayManager.initialize(Constants::SCREEN_WIDTH,
                              Constants::SCREEN_HEIGHT,
                              Constants::TOUCH_CALIBRATION);

    // Initialize StyleManager for centralized styling
    StyleManager::initialize();

    networkManager.loadConfig(config, StringConstants::CONFIG_FILE_PATH);

    mainScreen = new MainScreen();
    mainScreen->initialize();

    wifiScreen = new WifiScreen();
    wifiScreen->initialize();

    infoScreen = new InfoScreen(networkManager, config);
    infoScreen->initialize();

    settingsScreen = new SettingsScreen();
    settingsScreen->initialize();

    timeSettingsScreen = new TimeSettingsScreen(config, rtcManager);
    timeSettingsScreen->initialize();

    samplingSettingsScreen = new SamplingSettingsScreen(config, &taskManager);
    samplingSettingsScreen->initialize();

    lockScreen = new LockScreen();
    lockScreen->initialize();

    screenManager.initialize(mainScreen,
                             settingsScreen,
                             wifiScreen,
                             infoScreen,
                             timeSettingsScreen,
                             samplingSettingsScreen,
                             lockScreen);

    delay(1000);

    taskManager.initialize();

    networkManager.printConfig(StringConstants::CONFIG_FILE_PATH);

    if (config.ssid != "") {
        networkManager.setCredentials(config.ssid.c_str(), config.password.c_str());
        bool connected = networkManager.connect();
        if (connected) {
            Serial.println(
                "setup -> connected to Wi-Fi provided by data from configuration file! IP: "
                + networkManager.getIpAddress());
            rtcManager.syncWithNTP(StringConstants::NTP_SERVER, Constants::GMT_OFFSET_SEC);
            networkManager.setupServer();
        } else
            Serial.println(
                "setup -> can't connect to Wi-Fi - probably no data or corrupted or wrong!");
    }

    screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);

    delay(500);
}

void loop()
{
    displayManager.handleTasks();
    networkManager.handleServerClient();

    lv_task_handler();

    delay(5);
}

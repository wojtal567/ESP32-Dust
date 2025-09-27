#include <Arduino.h>
#include <HTTPClient.h>
#include <time.h>

#include "managers/MySD.hpp"
#include "managers/displaymanager.h"
#include "managers/networkmanager.h"
#include "managers/rtcmanager.h"
#include "managers/sensormanager.h"
#include "managers/stylemanager.h"
#include "managers/taskmanager.h"
#include "screens/infoscreen.h"
#include "screens/lockscreen.h"
#include "screens/mainscreen.h"
#include "screens/samplingsettingsscreen.h"
#include "screens/screenmanager.h"
#include "screens/settingsscreen.h"
#include "screens/timesettingsscreen.h"
#include "screens/wifiscreen.h"

#include <sqlite3.h>
#include "utils/constants.h"
#include "utils/stringConstants.h"

NetworkManager *networkManager = nullptr;
DisplayManager *displayManager = nullptr;
// Default configuration
Types::ConfigData config = {.ssid = "",
                            .password = "",
                            .measurePeriod = 30000,
                            .numberOfSamples = 5,
                            .timeBetweenSavingSamples = 3600000,
                            .turnFanTime = 30000,
                            .lcdLockTime = 60000};
void setup()
{

    sqlite3_initialize();
    // Serial debug
    Serial.begin(Constants::DEBUG_SERIAL_BAUD);
    Serial2.begin(Constants::PMS_SERIAL_BAUD,
                  SERIAL_8N1,
                  Constants::PMS_RX_PIN,
                  Constants::PMS_TX_PIN);

    static MySD sdCard(Constants::SD_CARD_PIN);

    networkManager = new NetworkManager(sdCard);

    displayManager = new DisplayManager();
    displayManager->initialize(Constants::SCREEN_WIDTH,
                               Constants::SCREEN_HEIGHT,
                               Constants::TOUCH_CALIBRATION);

    StyleManager::initialize();

    SensorManager *sensorManager = new SensorManager();
    sensorManager->initialize(&Serial, &Serial2);

    RTCManager *rtcManager = new RTCManager();

    ScreenManager *screenManager = new ScreenManager();

    MainScreen *mainScreen = new MainScreen(screenManager);
    mainScreen->initialize();

    SettingsScreen *settingsScreen = new SettingsScreen(screenManager);
    settingsScreen->initialize();

    WifiScreen *wifiScreen = new WifiScreen(config,
                                            networkManager,
                                            rtcManager,
                                            screenManager,
                                            sdCard);

    InfoScreen *infoScreen = new InfoScreen(networkManager, config, screenManager);
    infoScreen->initialize();

    LockScreen *lockScreen = new LockScreen(screenManager);
    lockScreen->initialize();

    TimeSettingsScreen *timeSettingsScreen = new TimeSettingsScreen(config,
                                                                    rtcManager,
                                                                    networkManager,
                                                                    screenManager,
                                                                    sdCard);
    timeSettingsScreen->initialize();

    sdCard.loadConfig(config, StringConstants::CONFIG_FILE_PATH);
    sdCard.printConfig(StringConstants::CONFIG_FILE_PATH);

    TaskManager *taskManager = new TaskManager(config,
                                               networkManager,
                                               sensorManager,
                                               sdCard,
                                               rtcManager,
                                               screenManager,
                                               mainScreen,
                                               lockScreen);
    taskManager->initialize();

    SamplingSettingsScreen *samplingSettingsScreen = new SamplingSettingsScreen(config,
                                                                                taskManager,
                                                                                networkManager,
                                                                                screenManager,
                                                                                sdCard);
    samplingSettingsScreen->initialize();

    screenManager->initialize(mainScreen,
                              settingsScreen,
                              wifiScreen,
                              infoScreen,
                              timeSettingsScreen,
                              samplingSettingsScreen,
                              lockScreen);

    networkManager->setTaskManager(taskManager);

    if (config.ssid != "") {
        networkManager->setCredentials(config.ssid.c_str(), config.password.c_str());
        bool connected = networkManager->connect();
        if (connected) {
            Serial.println(
                "setup -> connected to Wi-Fi provided by data from configuration file! IP: "
                + networkManager->getIpAddress());
            rtcManager->syncWithNTP(StringConstants::NTP_SERVER, Constants::GMT_OFFSET_SEC);
            networkManager->setupServer();
        } else
            Serial.println(
                "setup -> can't connect to Wi-Fi - probably no data or corrupted or wrong!");
    }

    screenManager->switchToScreen(ScreenType::MAIN);

    delay(500);
}

void loop()
{
    networkManager->handleServerClient();
    displayManager->handleTasks();

    delay(5);
}

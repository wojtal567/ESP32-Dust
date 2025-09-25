#include <Arduino.h>
#include <HTTPClient.h>
#include <time.h>

#include <managers/displaymanager.h>
#include <managers/networkmanager.h>
#include <managers/rtcmanager.h>
#include <managers/sensormanager.h>
#include <managers/stylemanager.h>
#include <managers/taskmanager.h>
#include <screens/infoscreen.h>
#include <screens/lockscreen.h>
#include <screens/mainscreen.h>
#include <screens/samplingsettingsscreen.h>
#include <screens/screenmanager.h>
#include <screens/settingsscreen.h>
#include <screens/timesettingsscreen.h>
#include <screens/wifiscreen.h>

#include <sqlite3.h>
#include <utils/constants.h>
#include <utils/stringConstants.h>

NetworkManager *networkManager = nullptr;
DisplayManager *displayManager = nullptr;

void setup()
{
    // Default configuration
    Types::ConfigData config = {.ssid = "",
                                .password = "",
                                .measurePeriod = 30000,
                                .numberOfSamples = 5,
                                .timeBetweenSavingSamples = 3600000,
                                .turnFanTime = 30000,
                                .lcdLockTime = 60000};

    sqlite3_initialize();
    // Serial debug
    Serial.begin(Constants::DEBUG_SERIAL_BAUD);
    Serial2.begin(Constants::PMS_SERIAL_BAUD,
                  SERIAL_8N1,
                  Constants::PMS_RX_PIN,
                  Constants::PMS_TX_PIN);

    MySD *sdCard = new MySD(Constants::SD_CARD_PIN);

    networkManager = new NetworkManager(sdCard);

    SensorManager *sensorManager = new SensorManager();
    sensorManager->initialize(&Serial, &Serial2);

    RTCManager rtcManager;

    ScreenManager screenManager;

    MainScreen *mainScreen = new MainScreen(screenManager);
    mainScreen->initialize();

    SettingsScreen *settingsScreen = new SettingsScreen(screenManager);
    settingsScreen->initialize();

    WifiScreen *wifiScreen = new WifiScreen(config, networkManager, rtcManager, screenManager);

    InfoScreen *infoScreen = new InfoScreen(networkManager, config, screenManager);
    infoScreen->initialize();

    LockScreen *lockScreen = new LockScreen(screenManager);
    lockScreen->initialize();

    TimeSettingsScreen *timeSettingsScreen = new TimeSettingsScreen(config,
                                                                    rtcManager,
                                                                    networkManager,
                                                                    screenManager);
    timeSettingsScreen->initialize();

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
                                                                                screenManager);
    samplingSettingsScreen->initialize();

    screenManager.initialize(mainScreen,
                             settingsScreen,
                             wifiScreen,
                             infoScreen,
                             timeSettingsScreen,
                             samplingSettingsScreen,
                             lockScreen);

    networkManager->setTaskManager(taskManager);
    displayManager = new DisplayManager();
    // Initialize display using DisplayManager
    displayManager->initialize(Constants::SCREEN_WIDTH,
                               Constants::SCREEN_HEIGHT,
                               Constants::TOUCH_CALIBRATION);

    networkManager->loadConfig(config, StringConstants::CONFIG_FILE_PATH);
    networkManager->printConfig(StringConstants::CONFIG_FILE_PATH);

    if (config.ssid != "") {
        networkManager->setCredentials(config.ssid.c_str(), config.password.c_str());
        bool connected = networkManager->connect();
        if (connected) {
            Serial.println(
                "setup -> connected to Wi-Fi provided by data from configuration file! IP: "
                + networkManager->getIpAddress());
            rtcManager.syncWithNTP(StringConstants::NTP_SERVER, Constants::GMT_OFFSET_SEC);
            networkManager->setupServer();
        } else
            Serial.println(
                "setup -> can't connect to Wi-Fi - probably no data or corrupted or wrong!");
    }

    screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);

    delay(500);
}

void loop()
{
    networkManager->handleServerClient();
    displayManager->handleTasks();

    lv_task_handler();

    delay(5);
}

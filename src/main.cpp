#include <Arduino.h>
#include <HTTPClient.h>
#include <time.h>

#include <LVGLInits.hpp>

// ! --------------------------------------------REST WebServer config
void setAppIp()
{
    WebServer &server = networkManager.getServer();
    String postBody = server.arg("plain");
    Serial.println(postBody);
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, postBody);
    if (error) {
        Serial.print(F(error.c_str()));

        server.send(400,
                    F("text/html"),
                    "Error while parsing json body! <br>" + (String)error.c_str());
    } else {
        JsonObject postObj = doc.as<JsonObject>();

        Serial.print(F("HTTP Method: "));
        Serial.println(server.method());

        if (server.method() == HTTP_POST) {
            if (postObj.containsKey("ip")) {

                appIpAddress = postObj["ip"].as<String>();

                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                String buf;
                serializeJson(doc, buf);
                lv_task_set_prio(getAppLastRecordAndSynchronize, LV_TASK_PRIO_MID);

                server.send(201, F("application/json"), buf);
            } else {
                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                doc["message"] = F("No data found or incorrect!");

                String buf;
                serializeJson(doc, buf);

                server.send(400, F("application/json"), buf);
            }
        }
    }
}

void restServerRouting()
{
    WebServer &server = networkManager.getServer();
    server.on("/", HTTP_GET, []() {
        WebServer &server = networkManager.getServer();
        server.send(200, F("text/html"), F("You have entered the wrong neighbourhood"));
    });
    server.on(F("/setAppIp"), HTTP_POST, setAppIp);
}

void handleNotFound()
{
    WebServer &server = networkManager.getServer();
    String message = "File Not Found \n\n" + (String) "URI: " + server.uri()
                             + "\n Method: " + (server.method() == HTTP_GET)
                         ? "GET"
                         : "POST" + (String) "\n Arguments: " + server.args() + "\n";

    for (uint8_t i = 0; i < server.args(); i++)
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";

    server.send(404, "text/plain", message);
}

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

    // Create old-style screen containers for existing screens
    settingsScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(settingsScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    infoScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(infoScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    timeSettingsScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(timeSettingsScr,
                                    LV_OBJ_PART_MAIN,
                                    LV_STATE_DEFAULT,
                                    LV_COLOR_BLACK);
    wifiScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(wifiScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lockScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(lockScr, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    samplingSettingsScr = lv_cont_create(NULL, NULL);
    lv_obj_set_style_local_bg_color(samplingSettingsScr,
                                    LV_OBJ_PART_MAIN,
                                    LV_STATE_DEFAULT,
                                    LV_COLOR_BLACK);

    // Initialize other screens using old functions
    wifiScreen();
    lockScreen();
    settingsScreen();
    infoScreen();
    timesettingsScreen();
    samplingsettingsScreen();

    mainScreen = new MainScreen();
    mainScreen->initialize();

    // Initialize ScreenManager with the new main screen
    screenManager.initialize(mainScreen);

    networkManager.loadConfig(config, StringConstants::CONFIG_FILE_PATH);
    delay(1000);

    lv_dropdown_set_selected(lockScreenDDlist, getDDListIndexBasedOnLcdLockTime(config.lcdLockTime));

    date = lv_task_create(dateTimeFunc, 800, LV_TASK_PRIO_MID, NULL);
    status = lv_task_create(statusFunc, 5000, LV_TASK_PRIO_LOW, NULL);
    lv_spinbox_set_value(measurePeriodHour, ((config.timeBetweenSavingSamples / 60000) / 60));
    lv_spinbox_set_value(measurePeriodsecond, (config.timeBetweenSavingSamples / 1000) % 60);
    lv_spinbox_set_value(measurePeriodMinute, ((config.timeBetweenSavingSamples / 60000) % 60));
    lv_spinbox_set_value(measureAvPeriod, (config.measurePeriod / 1000));
    lv_spinbox_set_value(measureNumber, config.numberOfSamples);
    lv_spinbox_set_value(turnFanOnTime, (config.turnFanTime / 1000));
    set_spinbox_digit_format(measureNumber, Constants::MIN_RANGE, Constants::MAX_RANGE, 0);
    set_spinbox_digit_format(measureAvPeriod, Constants::MIN_RANGE, Constants::MAX_RANGE, 0);
    set_spinbox_digit_format(turnFanOnTime, Constants::MIN_RANGE, Constants::MAX_RANGE, 0);

    getSample = lv_task_create(getSampleFunc,
                               (config.timeBetweenSavingSamples
                                - (config.numberOfSamples - 1) * config.measurePeriod),
                               LV_TASK_PRIO_HIGH,
                               NULL);
    turnFanOn = lv_task_create(turnFanOnFunc,
                               config.timeBetweenSavingSamples - config.turnFanTime,
                               LV_TASK_PRIO_HIGHEST,
                               NULL);
    inactiveTime = lv_task_create(inactive_screen, 1, LV_TASK_PRIO_HIGH, NULL);
    getAppLastRecordAndSynchronize = lv_task_create_basic();
    lv_task_set_cb(getAppLastRecordAndSynchronize, fetchLastRecordAndSynchronize);
    lv_task_set_period(getAppLastRecordAndSynchronize,
                       30000); // Fixed period instead of fetchPeriod
    lv_task_set_prio(getAppLastRecordAndSynchronize, LV_TASK_PRIO_MID);
    lv_task_handler();
    networkManager.printConfig(StringConstants::CONFIG_FILE_PATH);

    if (config.ssid != "") {
        networkManager.setCredentials(config.ssid.c_str(), config.password.c_str());
        bool connected = networkManager.connect();
        if (connected) {
            Serial.println(
                "setup -> connected to Wi-Fi provided by data from configuration file! IP: "
                + networkManager.getIpAddress());
            networkManager.updateDateTime();
            restServerRouting();
            WebServer &server = networkManager.getServer();
            server.onNotFound(handleNotFound);
            server.begin();
        } else
            Serial.println(
                "setup -> can't connect to Wi-Fi - probably no data or corrupted or wrong!");
    }
    display_current_config();

    screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);

    delay(500);
}

void loop()
{
    displayManager.handleTasks();
    networkManager.handleServerClient();
    delay(5);
}

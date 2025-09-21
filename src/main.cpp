#include <Arduino.h>
#include <HTTPClient.h>
#include <time.h>

#include <managers/stylemanager.h>
#include <managers/taskmanager.h>
#include "GlobalVariables.hpp"
// #include <LVGLTasks.hpp> // Commented out - functions moved to TaskManager

// ! --------------------------------------------REST WebServer config
void setAppIp()
{
    WebServer &server = networkManager.getServer();
    String postBody = server.arg("plain");
    Serial.print("[" + String(millis()) + "] ");
    Serial.println(postBody);
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, postBody);
    if (error) {
        Serial.print("[" + String(millis()) + "] ");
        Serial.print(F(error.c_str()));

        server.send(400,
                    F("text/html"),
                    "Error while parsing json body! <br>" + (String)error.c_str());
    } else {
        JsonObject postObj = doc.as<JsonObject>();

        Serial.print("[" + String(millis()) + "] ");
        Serial.print(F("HTTP Method: "));
        Serial.print("[" + String(millis()) + "] ");
        Serial.println(server.method());

        if (server.method() == HTTP_POST) {
            if (postObj.containsKey("ip")) {

                appIpAddress = postObj["ip"].as<String>();

                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                String buf;
                serializeJson(doc, buf);

                taskManager.updateGetAppLastRecordAndSynchronizeTaskPrio(LV_TASK_PRIO_MID);

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
            networkManager.updateDateTime();
            restServerRouting();
            WebServer &server = networkManager.getServer();
            server.onNotFound(handleNotFound);
            server.begin();
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

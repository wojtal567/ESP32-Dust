#include <Arduino.h>
#include <WiFi.h>
#include <rtc.hpp>
#include <HTTPClient.h>
#include <time.h>

#include <Functions.hpp>

// ! --------------------------------------------REST WebServer config
void setAppIp()
{
    String postBody = server.arg("plain");
    Serial.println(postBody);
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, postBody);
    if (error)
    {
        Serial.print(F(error.c_str()));

        server.send(400, F("text/html"),
                    "Error while parsing json body! <br>" + (String)error.c_str());
    }
    else
    {
        JsonObject postObj = doc.as<JsonObject>();

        Serial.print(F("HTTP Method: "));
        Serial.println(server.method());

        if (server.method() == HTTP_POST)
        {
            if (postObj.containsKey("ip"))
            {

                appIpAddress = postObj["ip"].as<String>();

                DynamicJsonDocument doc(512);
                doc["status"] = "OK";
                String buf;
                serializeJson(doc, buf);
                // lv_task_set_prio(getAppLastRecordAndSynchronize, LV_TASK_PRIO_MID);

                server.send(201, F("application/json"), buf);
            }
            else
            {
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
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
                    F("You have entered the wrong neighbourhood"));
    });
    server.on(F("/setAppIp"), HTTP_POST, setAppIp);
}

void handleNotFound()
{
    String message = "File Not Found \n\n" + (String) "URI: " + server.uri() + "\n Method: " + (server.method() == HTTP_GET) ? "GET" : "POST" + (String) "\n Arguments: " + server.args() + "\n";

    for (uint8_t i = 0; i < server.args(); i++)
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";

    server.send(404, "text/plain", message);
}

void setup()
{
    pinMode(FAN_PIN, OUTPUT);
    digitalWrite(FAN_PIN, LOW);
    sqlite3_initialize();
    //Serial debug
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, 16, 17);
    //PMS sensor initialization
    pmsSensor = new PMS5003(&Serial2, &Serial);

    mySDCard.loadConfig(config, configFilePath);
    delay(1000);

    if (config.ssid != "")
    {
        mySDCard.printConfig(configFilePath);
        Serial.print(getMainTimestamp(Rtc).c_str());
        WiFi.begin(config.ssid.c_str(), config.password.c_str());
        volatile int attempts = 0;
        while (WiFi.status() != WL_CONNECTED and attempts != 20)
        {
            delay(500);
            Serial.print(".");
            attempts++;
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("setup -> connected to Wi-Fi provided by data from configuration file! IP: " + WiFi.localIP().toString());
            config_time();
            restServerRouting();
            server.onNotFound(handleNotFound);
            server.begin();
        }
        else if (WiFi.status() == WL_DISCONNECTED)
            Serial.println("setup -> can't connect to Wi-Fi - probably no data or corrupted or wrong!");
    }
    measureSaveDateTime = Rtc.GetDateTime();
    Serial.println(measureSaveDateTime);
}

void loop()
{
    server.handleClient();
    if(Rtc.GetDateTime() - measureSaveDateTime > RtcDateTime(config.timeBetweenSavingSamples/1000))
        getSampleFunc();
    if(Rtc.GetDateTime() - measureSaveDateTime == RtcDateTime(config.turnFanTime/1000))
    {
        Serial.println("Turning fan on");
        digitalWrite(FAN_PIN, HIGH);
    }
    delay(5);
}

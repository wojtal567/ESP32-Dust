#include "api/httpapi.h"
#include "GlobalVariables.hpp"

namespace HttpApi {
    
    void setAppIp() {
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

    void handleNotFound() {
        WebServer &server = networkManager.getServer();
        String message = "File Not Found \n\n" + (String) "URI: " + server.uri()
                                 + "\n Method: " + (server.method() == HTTP_GET)
                             ? "GET"
                             : "POST" + (String) "\n Arguments: " + server.args() + "\n";

        for (uint8_t i = 0; i < server.args(); i++)
            message += " " + server.argName(i) + ": " + server.arg(i) + "\n";

        server.send(404, "text/plain", message);
    }

    void setupRouting() {
        WebServer &server = networkManager.getServer();
        server.on("/", HTTP_GET, []() {
            WebServer &server = networkManager.getServer();
            server.send(200, F("text/html"), F("You have entered the wrong neighbourhood"));
        });
        server.on(F("/setAppIp"), HTTP_POST, setAppIp);
    }
    
}
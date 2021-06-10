#include <Arduino.h>
#include <WiFi.h>
#include <rtc.hpp>
#include <HTTPClient.h>
#include <time.h>
#include <Functions.hpp>

//for request from website
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) { 
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
  }
}

//on websocket event
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
 void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

//init of websocket
void initWebSocket() {
  socket.onEvent(onEvent);
  server.addHandler(&socket);
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
        Serial.print(getMainTimestamp(currentDateTime).c_str());
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
        }
        else if (WiFi.status() == WL_DISCONNECTED)
            Serial.println("setup -> can't connect to Wi-Fi - probably no data or corrupted or wrong!");
    }
    measureSaveDateTime = Rtc.GetDateTime();

    initWebSocket();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SD, "/web/index.html", "text/html");
    });
    server.on("/src/bootstrap.bundle.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SD, "/web/src/bootstrap.bundle.min.js", "text/javascript");
    });
    
    server.on("/src/jquery-3.3.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SD, "/web/src/jquery-3.3.1.min.js", "text/javascript");
    });
    
    server.on("/src/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SD, "/web/src/bootstrap.min.css", "text/css");
    });
    server.on("/timestamp", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", getMainTimestamp(currentDateTime).c_str());
    });
    server.begin();
    
}

void loop()
{
    currentDateTime = Rtc.GetDateTime();
    if(currentDateTime - measureSaveDateTime > RtcDateTime(config.timeBetweenSavingSamples/1000))
        getSampleFunc();
    if(currentDateTime - measureSaveDateTime == RtcDateTime(config.turnFanTime/1000))
    {
        Serial.println("Turning fan on");
        digitalWrite(FAN_PIN, HIGH);
    }
    socket.cleanupClients();
    delay(5);
}

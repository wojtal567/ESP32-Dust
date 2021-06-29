#include <Arduino.h>
#include <WiFi.h>
#include <rtc.hpp>
#include <HTTPClient.h>
#include <time.h>
#include <Functions.hpp>

TaskHandle_t task;
void setup()
{
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
  sqlite3_initialize();
  Wire.begin();
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
  
  currentDateTime = Rtc.GetDateTime();
    Serial.print(getMainTimestamp(currentDateTime).c_str());
  initWebSocket();
  if(mySDCard.begin())
    initWebServer();
  mySDCard.end();
  xTaskCreatePinnedToCore(secondCore, "task", 10000, NULL, 1, &task, 0);
  delay(500);
  Serial.println("dupzgo");
}
void loop()
{
  if(!isSampleTaking)
    currentDateTime = Rtc.GetDateTime();
  socket.cleanupClients();
  delay(500);
}

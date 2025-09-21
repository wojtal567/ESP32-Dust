#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include <MySD.hpp>
#include <WebServer.h>
#include <string>
#include "utils/types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class NetworkManager {
public:
    NetworkManager(MySD* sdCard);
    bool connect();
    bool isConnected() const;
    String getIpAddress() const;
    void setCredentials(const String& ssid, const String& password);
    
    void connectAsync(const String& ssid, const String& password);
    
    // Web Server functionality
    WebServer& getServer();
    void setupServer();
    void handleServerClient();
    
    // Configuration management (delegated to SD card)
    void loadConfig(Types::ConfigData& config, const std::string& configPath);
    void saveConfig(const Types::ConfigData& config, const std::string& configPath);
    void printConfig(const std::string& configPath);

private:
    MySD* sdCard_;
    String m_ssid, m_password;
    WebServer m_server;
    
    // WiFi connection state management
    TaskHandle_t m_wifiTaskHandle;
    
    // Static task function for FreeRTOS
    static void wifiConnectionTask(void* parameter);
};

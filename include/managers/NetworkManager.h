#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <string>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "MySD.hpp"
#include "utils/types.h"

// Forward declaration
class TaskManager;

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
    
    // API endpoint handlers
    void handleSetAppIp();
    void handleNotFound();
    
    // Configuration management (delegated to SD card)
    void loadConfig(Types::ConfigData& config, const std::string& configPath);
    void saveConfig(const Types::ConfigData& config, const std::string& configPath);
    void printConfig(const std::string& configPath);
    
    // Dependency injection
    void setTaskManager(TaskManager* taskManager);

private:
    MySD* sdCard_;
    TaskManager* m_taskManager;
    String m_ssid, m_password;
    WebServer m_server;
    
    // WiFi connection state management
    TaskHandle_t m_wifiTaskHandle;
    
    // Static task function for FreeRTOS
    static void wifiConnectionTask(void* parameter);
};

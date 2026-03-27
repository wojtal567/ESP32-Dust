#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <string>

#include "MySD.hpp"
#include "utils/types.h"

// Forward declaration
class TaskManager;

class NetworkManager
{
public:
    NetworkManager(MySD &sdCard);
    bool connect();
    bool isConnected() const;
    String getIpAddress() const;
    void setCredentials(const String &ssid, const String &password);

    void connectAsync(const String &ssid, const String &password);

    // Web Server functionality
    WebServer &getServer();
    void setupServer();
    void handleServerClient();

    // API endpoint handlers
    void handleSetAppIp();
    void handleNotFound();

    // Dependency injection
    void setTaskManager(TaskManager *taskManager);

private:
    MySD &m_sdCard;
    TaskManager *m_taskManager;
    String m_ssid, m_password;
    WebServer m_server;

    // WiFi connection state management
    TaskHandle_t m_wifiTaskHandle;
    SemaphoreHandle_t m_wifiTaskMutex;

    // Static task function for FreeRTOS
    static void wifiConnectionTask(void *parameter);
};

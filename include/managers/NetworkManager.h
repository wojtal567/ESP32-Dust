#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include <MySD.hpp>
#include <WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <string>
#include "utils/types.h"

class NetworkManager {
public:
    NetworkManager(MySD* sdCard);
    bool connect();
    bool isConnected() const;
    String getIpAddress() const;
    void setCredentials(const String& ssid, const String& password);
    
    // Web Server functionality
    WebServer& getServer();
    void setupServer();
    void handleServerClient();
    
    // NTP functionality
    void updateDateTime();
    bool isNTPConnected() const;
    
    // Configuration management (delegated to SD card)
    void loadConfig(Types::ConfigData& config, const std::string& configPath);
    void saveConfig(const Types::ConfigData& config, const std::string& configPath);
    void printConfig(const std::string& configPath);

private:
    MySD* sdCard_;
    String m_ssid, m_password;
    WebServer m_server;
    WiFiUDP m_ntpUDP;
    NTPClient m_dateTimeClient;
};

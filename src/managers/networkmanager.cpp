#include "managers/networkmanager.h"
#include "GlobalVariables.hpp"
#include "utils/constants.h"
#include "utils/stringConstants.h"

NetworkManager::NetworkManager(MySD *sdCard)
    : sdCard_(sdCard)
    , m_server(80)
    , m_dateTimeClient(m_ntpUDP, StringConstants::NTP_SERVER, Constants::GMT_OFFSET_SEC)
    , m_wifiTaskHandle(nullptr)
{}

bool NetworkManager::connect()
{
    WiFi.begin(m_ssid.c_str(), m_password.c_str());
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        attempts++;
    }
    return WiFi.status() == WL_CONNECTED;
}

bool NetworkManager::isConnected() const
{
    return WiFi.status() == WL_CONNECTED;
}

String NetworkManager::getIpAddress() const
{
    if (isConnected())
        return WiFi.localIP().toString();
    return "";
}

void NetworkManager::setCredentials(const String &ssid, const String &password)
{
    m_ssid = ssid;
    m_password = password;
}

void NetworkManager::connectAsync(const String &ssid, const String &password)
{
    // Store credentials
    m_ssid = ssid;
    m_password = password;

    // If already connecting, stop the previous task
    if (m_wifiTaskHandle != nullptr) {
        vTaskDelete(m_wifiTaskHandle);
        m_wifiTaskHandle = nullptr;
    }

    // Create FreeRTOS task for WiFi connection
    xTaskCreate(wifiConnectionTask, "WiFiConnection", 4096, this, 1, &m_wifiTaskHandle);
}

// Static task function that runs in separate thread
void NetworkManager::wifiConnectionTask(void *parameter)
{
    NetworkManager *networkManager = static_cast<NetworkManager *>(parameter);

    // Start WiFi connection
    WiFi.begin(networkManager->m_ssid.c_str(), networkManager->m_password.c_str());

    // Wait for connection with timeout
    int attempts = 0;
    const int maxAttempts = 20; // 10 seconds timeout (500ms * 20)

    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        vTaskDelay(500 / portTICK_PERIOD_MS); // Non-blocking delay
        attempts++;
    }

    // Clean up task handle
    networkManager->m_wifiTaskHandle = nullptr;

    // Delete this task
    vTaskDelete(NULL);
}

WebServer &NetworkManager::getServer()
{
    return m_server;
}

void NetworkManager::setupServer()
{
    // This will be implemented when we move server setup from main.cpp
}

void NetworkManager::handleServerClient()
{
    m_server.handleClient();
}

void NetworkManager::updateDateTime()
{
    if (isConnected()) {
        m_dateTimeClient.begin();
        m_dateTimeClient.update();
    }
}

bool NetworkManager::isNTPConnected() const
{
    return isConnected() && m_dateTimeClient.isTimeSet();
}

void NetworkManager::loadConfig(Types::ConfigData &config, const std::string &configPath)
{
    if (sdCard_) {
        // Convert Types::ConfigData to MySD's Config format for loading
        Config tempConfig;
        sdCard_->loadConfig(tempConfig, configPath);

        // Convert back to Types::ConfigData
        config.ssid = String(tempConfig.ssid.c_str());
        config.password = String(tempConfig.password.c_str());
        config.lcdLockTime = tempConfig.lcdLockTime;
        config.timeBetweenSavingSamples = tempConfig.timeBetweenSavingSamples;
        config.measurePeriod = tempConfig.measurePeriod;
        config.numberOfSamples = tempConfig.numberOfSamples;
        config.turnFanTime = tempConfig.turnFanTime;
    }
}

void NetworkManager::saveConfig(const Types::ConfigData &config, const std::string &configPath)
{
    if (sdCard_) {
        // Convert Types::ConfigData to MySD's Config format for saving
        Config tempConfig;
        tempConfig.ssid = config.ssid.c_str();
        tempConfig.password = config.password.c_str();
        tempConfig.lcdLockTime = config.lcdLockTime;
        tempConfig.timeBetweenSavingSamples = config.timeBetweenSavingSamples;
        tempConfig.measurePeriod = config.measurePeriod;
        tempConfig.numberOfSamples = config.numberOfSamples;
        tempConfig.currentSampleNumber = currentSampleNumber; // Use global variable
        tempConfig.turnFanTime = config.turnFanTime;

        sdCard_->saveConfig(tempConfig, configPath);
    }
}

void NetworkManager::printConfig(const std::string &configPath)
{
    if (sdCard_) {
        sdCard_->printConfig(configPath);
    }
}

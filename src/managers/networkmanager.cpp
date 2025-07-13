#include "managers/networkmanager.h"
#include "GlobalVariables.hpp"
#include "utils/constants.h"
#include "utils/stringConstants.h"

NetworkManager::NetworkManager(MySD *sdCard)
    : sdCard_(sdCard)
    , m_server(80)
    , m_dateTimeClient(m_ntpUDP, StringConstants::NTP_SERVER, Constants::GMT_OFFSET_SEC)
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
    WiFi.begin(m_ssid.c_str(), m_password.c_str());
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

#include "managers/networkmanager.h"

NetworkManager::NetworkManager(MySD *sdCard)
    : sdCard_(sdCard)
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

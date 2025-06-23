#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include <MySD.hpp>

#include "utils/types.h"

class NetworkManager {
public:
    NetworkManager(MySD* sdCard);
    bool connect();
    bool isConnected() const;
    String getIpAddress() const;
    void setCredentials(const String& ssid, const String& password);

private:
    MySD* sdCard_;
    String m_ssid, m_password;
};

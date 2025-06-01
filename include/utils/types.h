#pragma once

#include <Arduino.h>

#include <map>
#include <string>

namespace Types {

// Environmental data structure for temperature, humidity, etc.
struct EnvironmentData {
    float temperature;  // Temperature in Celsius
    float humidity;     // Relative humidity in %
};

// Configuration data structure for device settings
struct ConfigData {
    // WiFi settings
    String ssid;
    String password;
    String appIpAddress;

    // Measurement settings
    int measurePeriod;             // Period between measurements in ms
    int numberOfSamples;           // Number of samples to average
    int timeBetweenSavingSamples;  // Time between saving samples to storage in ms
    int turnFanTime;               // Time to turn fan on before measurement in ms

    // Display settings
    int lcdLockTime;  // Screen lock timeout in ms

    // Fan control settings
    int fanMode;  // 0=auto, 1=on, 2=off
};
}  // namespace Types

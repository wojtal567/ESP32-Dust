#pragma once

#include <Arduino.h>

#include <map>
#include <string>

namespace Types {
// Particle data structure to store PMS5003 sensor readings
struct ParticleData {
    float pm1_0;           // PM1.0 concentration (μg/m³)
    float pm2_5;           // PM2.5 concentration (μg/m³)
    float pm10_0;          // PM10.0 concentration (μg/m³)
    float particles_0_3;   // Particles > 0.3μm count per 0.1L air
    float particles_0_5;   // Particles > 0.5μm count per 0.1L air
    float particles_1_0;   // Particles > 1.0μm count per 0.1L air
    float particles_2_5;   // Particles > 2.5μm count per 0.1L air
    float particles_5_0;   // Particles > 5.0μm count per 0.1L air
    float particles_10_0;  // Particles > 10.0μm count per 0.1L air
};

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

// Device status structure
struct DeviceStatus {
    bool fanActive;
    bool sensorActive;
    bool wifiConnected;
    bool sdCardPresent;
    bool dbInitialized;
};
}  // namespace Types

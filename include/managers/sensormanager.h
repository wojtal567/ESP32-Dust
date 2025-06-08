#pragma once

#include <Arduino.h>
#include <WEMOS_SHT3X.h>
#include <map>
#include <string>
#include "sensors/pms.h"

class SensorManager
{
public:
    SensorManager();
    ~SensorManager();

    void initialize(HardwareSerial *debugger = &Serial, HardwareSerial *reader = &Serial2);

    bool readSensors();
    bool readDustSensor();
    bool readTemperatureHumiditySensor();

    bool isDustSensorWorking() const;
    bool isTemperatureHumiditySensorWorking() const;
    bool isFanOn() const;

    float getTemperature() const;
    float getHumidity() const;
    float getPM10() const;
    float getPM25() const;
    float getPM100() const;
    float getParticles03um() const;
    float getParticles05um() const;
    float getParticles10um() const;
    float getParticles25um() const;
    float getParticles50um() const;
    float getParticles100um() const;

    const std::map<std::string, float> &getDustData() const;

    void sleepDustSensor();
    void wakeDustSensor();

private:
    // Sensors
    PMS *m_pmsSensor;
    SHT3X m_sht30;

    // Sensor data
    std::map<std::string, float> m_dustData;
    float m_temperature;
    float m_humidity;

    // State variables
    bool m_dustSensorWorking;
    bool m_temperatureSensorWorking;
    bool m_isFanOn;
};

// Global instance
extern SensorManager sensorManager;

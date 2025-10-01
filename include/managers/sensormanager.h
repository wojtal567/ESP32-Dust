#pragma once

#include <Arduino.h>
#include <WEMOS_SHT3X.h>
#include <map>
#include <string>

class PMS;

class SensorManager
{
public:
    SensorManager();
    ~SensorManager();

    void initialize(HardwareSerial *reader = &Serial2);

    bool readDustSensor();
    bool readTemperatureHumiditySensor();

    float getTemperature() const;
    float getHumidity() const;

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
    bool m_isFanOn;
};

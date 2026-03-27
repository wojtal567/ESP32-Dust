#include "managers/sensormanager.h"

#include <Arduino.h>

#include "sensors/pms.h"
#include "utils/config.h"
#include "utils/constants.h"

SensorManager::SensorManager()
    : m_pmsSensor(nullptr)
    , m_temperature(0)
    , m_sht30(HardwareConfig::SHT30_I2C_ADDRESS)
    , m_humidity(0)
    , m_isFanOn(false)
{
    // Initialize dust data map with default values
    for (const auto &label : PMS::SENSOR_LABELS) {
        m_dustData[label] = 0.0f;
    }
}

SensorManager::~SensorManager()
{
    if (m_pmsSensor != nullptr) {
        delete m_pmsSensor;
        m_pmsSensor = nullptr;
    }
}

void SensorManager::initialize(HardwareSerial *reader)
{
    // Initialize PMS sensor with correct parameter types
    if (m_pmsSensor == nullptr) {
        // Create PMS with HardwareSerial pointers
        m_pmsSensor = new PMS(reader);
    }

    // Set up fan control pin
    pinMode(HardwareConfig::FAN_PIN, OUTPUT);
    digitalWrite(HardwareConfig::FAN_PIN, LOW);
    m_isFanOn = false;
}

bool SensorManager::readDustSensor()
{
    if (m_pmsSensor == nullptr) {
        return false;
    }

    bool success = m_pmsSensor->readData();
    if (success) {
        m_dustData = m_pmsSensor->returnData();
    }
    return success;
}

bool SensorManager::readTemperatureHumiditySensor()
{
    auto success = m_sht30.get();
    if (success == 0) { // SHT30 returns 0 on success
        m_temperature = m_sht30.cTemp;
        m_humidity = m_sht30.humidity;
    } else {
        LOG_SENSOR("Failed to read temperature and humidity from SHT30 sensor.");
    }
    return success;
}

float SensorManager::getTemperature() const
{
    return m_temperature;
}

float SensorManager::getHumidity() const
{
    return m_humidity;
}

const std::map<std::string, float> &SensorManager::getDustData() const
{
    return m_dustData;
}

void SensorManager::sleepDustSensor()
{
    if (!m_isFanOn) {

        Serial.println("Dust sensor fan is already OFF.");
        return;
    }

    digitalWrite(HardwareConfig::FAN_PIN, LOW);
    m_isFanOn = false;
}

void SensorManager::wakeDustSensor()
{
    if (m_isFanOn) {
        LOG_SENSOR("Dust sensor fan is already ON.");
        return;
    }

    digitalWrite(HardwareConfig::FAN_PIN, HIGH);
    m_isFanOn = true;
    LOG_SENSOR("Dust sensor fan turned ON.");
}

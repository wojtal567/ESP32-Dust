#include "managers/SensorManager.h"
#include <Arduino.h>
#include "utils/constants.h"

// Initialize the global instance
SensorManager sensorManager;

SensorManager::SensorManager()
    : m_pmsSensor(nullptr)
    , m_temperature(0)
    , m_sht30(0x44) // TODO extract address to config
    , m_humidity(0)
    , m_dustSensorWorking(false)
    , m_temperatureSensorWorking(false)
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

void SensorManager::initialize(HardwareSerial *debugger, HardwareSerial *reader)
{
    // Initialize PMS sensor with correct parameter types
    if (m_pmsSensor == nullptr) {
        // Create PMS with HardwareSerial pointers
        m_pmsSensor = new PMS(debugger, reader);
    }

    // Set up fan control pin
    pinMode(Constants::FAN_PIN, OUTPUT);
    digitalWrite(Constants::FAN_PIN, LOW);
    m_isFanOn = false;

    // Try reading from the PMS sensor to see if it's working
    m_dustSensorWorking = readDustSensor();

    // Check SHT30 sensor
    m_temperatureSensorWorking = m_sht30.get();
}

bool SensorManager::readSensors()
{
    bool dustResult = readDustSensor();
    bool tempResult = readTemperatureHumiditySensor();
    return dustResult && tempResult;
}

bool SensorManager::readDustSensor()
{
    if (m_pmsSensor == nullptr) {
        return false;
    }

    bool success = m_pmsSensor->readData();
    if (success) {
        m_dustData = m_pmsSensor->returnData();
        m_dustSensorWorking = true;
    }
    return success;
}

bool SensorManager::readTemperatureHumiditySensor()
{
    auto success = m_sht30.get();
    if (success == 0) { // SHT30 returns 0 on success
        m_temperature = m_sht30.cTemp;
        m_humidity = m_sht30.humidity;
        m_temperatureSensorWorking = true;
    } else {
        Serial.println("Failed to read temperature and humidity from SHT30 sensor.");
        m_temperatureSensorWorking = false;
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

float SensorManager::getPM10() const
{
    return m_dustData.at("pm10_standard");
}

float SensorManager::getPM25() const
{
    return m_dustData.at("pm25_standard");
}

float SensorManager::getPM100() const
{
    return m_dustData.at("pm100_standard");
}

float SensorManager::getParticles03um() const
{
    return m_dustData.at("particles_03um");
}

float SensorManager::getParticles05um() const
{
    return m_dustData.at("particles_05um");
}

float SensorManager::getParticles10um() const
{
    return m_dustData.at("particles_10um");
}

float SensorManager::getParticles25um() const
{
    return m_dustData.at("particles_25um");
}

float SensorManager::getParticles50um() const
{
    return m_dustData.at("particles_50um");
}

float SensorManager::getParticles100um() const
{
    return m_dustData.at("particles_100um");
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

    digitalWrite(Constants::FAN_PIN, LOW);
    m_isFanOn = false;
}

void SensorManager::wakeDustSensor()
{
    if (m_isFanOn) {
        Serial.println("Dust sensor fan is already ON.");
        return;
    }

    digitalWrite(Constants::FAN_PIN, HIGH);
    m_isFanOn = true;
    Serial.println("Dust sensor fan turned ON.");
}

bool SensorManager::isDustSensorWorking() const
{
    return m_dustSensorWorking;
}

bool SensorManager::isTemperatureHumiditySensorWorking() const
{
    return m_temperatureSensorWorking;
}

bool SensorManager::isFanOn() const
{
    return m_isFanOn;
}

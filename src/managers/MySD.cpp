#include "managers/MySD.hpp"

#include "utils/config.h"

MySD::MySD(int port)
    : m_sampleDB("/sd/database.db", "/database.db", "samples")
{
    m_port = port;
    if (!m_mutex) {
        m_mutex = xSemaphoreCreateMutex();
    }
}

bool MySD::begin()
{
    return SD.begin(m_port);
}

void MySD::end()
{
    SD.end();
}

bool MySD::start()
{
    bool result = false;
    if (m_mutex && xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE) {
        result = begin();
        if (result) {
            LOG_SD("MySD::start -> Initializing.");
            m_sampleDB.init();
            m_sampleDB.open();
            m_sampleDB.createTable();
            m_sampleDB.close();
            m_sampleDB.kill();
        } else {
            LOG_ERROR("MySD::start -> Can't initilize - no access to SD card.");
        }
        end();
        xSemaphoreGive(m_mutex);
    } else {
        LOG_ERROR("MySD::start -> SD mutex not acquired");
    }
    return result;
}

void MySD::save(std::map<std::string, float> data,
                float temperature,
                float humidity,
                String timestamp)
{
    LOG_SD("MySD::save");
    if (m_mutex && xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE) {
        if (begin()) {
            LOG_SD("MySD::save -> SD Card detected");
            if (SD.exists(m_sampleDB.getRelativePath())) {
                LOG_SD("MySD::save -> File exists. Trying to save data.");
                LOG_SD("Database " + m_sampleDB.getLocalPath() + " exists. Saving data...");
                m_sampleDB.init();
                m_sampleDB.open();
                m_sampleDB.save(data, temperature, humidity, timestamp);
                m_sampleDB.close();
                m_sampleDB.kill();
            } else {
                LOG_SD("MySD::save -> No database file. Trying to create one and saving data.");
                LOG_SD("Database " + m_sampleDB.getLocalPath() + " don't exist. Saving data...");
                m_sampleDB.init();
                m_sampleDB.open();
                m_sampleDB.createTable();
                m_sampleDB.save(data, temperature, humidity, timestamp);
                m_sampleDB.close();
                m_sampleDB.kill();
            }
        } else {
            LOG_ERROR("MySD::save -> Can't save - no access to SD card.");
        }
        end();
        xSemaphoreGive(m_mutex);
    } else {
        LOG_ERROR("MySD::save -> SD mutex not acquired");
    }
}

void MySD::select(String datetime, JsonArray *array)
{
    LOG_SD("MySD::select");
    if (m_mutex && xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE) {
        if (begin()) {
            LOG_SD("MySD::select -> SD Card detected");
            if (SD.exists(m_sampleDB.getRelativePath())) {
                LOG_SD("MySD::select -> File exists. Trying to select data.");
                LOG_SD("Database " + m_sampleDB.getLocalPath() + " exists.");
                m_sampleDB.init();
                m_sampleDB.open();
                m_sampleDB.select(datetime, array);
                m_sampleDB.close();
                m_sampleDB.kill();
            } else
                LOG_SD("MySD::select -> File don't exist.");
        } else {
            LOG_ERROR("MySD::select -> Can't access SD card.");
        }
        end();
        xSemaphoreGive(m_mutex);
    } else {
        LOG_ERROR("MySD::select -> SD mutex not acquired");
    }
}

void MySD::getLastRecord(JsonArray *array)
{
    LOG_SD("MySD::getLastRecord");
    if (m_mutex && xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE) {
        if (begin()) {
            LOG_SD("MySD::getLastRecord -> SD Card detected");
            if (SD.exists(m_sampleDB.getRelativePath())) {
                LOG_SD(
                    "MySD::getLastRecord -> File exists. Trying to get last saved sample record.");
                LOG_SD("Database " + m_sampleDB.getLocalPath() + " exists.");
                m_sampleDB.init();
                m_sampleDB.open();
                m_sampleDB.getLastRecord(array);
                m_sampleDB.close();
                m_sampleDB.kill();
            } else
                LOG_SD("MySD::getLastRecord -> File don't exist.");
        } else {
            LOG_ERROR("MySD::getLastRecord -> Can't access SD card.");
        }
        end();
        xSemaphoreGive(m_mutex);
    } else {
        LOG_ERROR("MySD::getLastRecord -> SD mutex not acquired");
    }
}

void MySD::saveConfig(const Types::ConfigData &config, const std::string &filePath)
{
    if (m_mutex && xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE) {
        if (begin()) {
            SD.remove(filePath.c_str());
            File configurationFile = SD.open(filePath.c_str(), FILE_WRITE);
            if (!configurationFile) {
                LOG_ERROR("MySD::saveConfig -> Failed to create configuration file.");
            } else {
                LOG_SD("Configuration file exists. Trying to save...");
                StaticJsonDocument<512> doc;
                doc["ssid"] = config.ssid.c_str();
                doc["password"] = config.password.c_str();
                doc["lcdLockTime"] = config.lcdLockTime;
                doc["timeBetweenSavingSamples"] = config.timeBetweenSavingSamples;
                doc["measurePeriod"] = config.measurePeriod;
                doc["numberOfSamples"] = config.numberOfSamples;
                doc["turnFanTime"] = config.turnFanTime;
                doc["timeOffset"] = config.timeOffset;
                if (serializeJson(doc, configurationFile) == 0) {
                    LOG_ERROR("MySD::saveConfig -> Failed to write to file.");
                }
                configurationFile.close();
            }
        } else {
            LOG_ERROR("MySD::saveConfig -> Can't access SD card.");
        }
        end();
        xSemaphoreGive(m_mutex);
    } else {
        LOG_ERROR("MySD::saveConfig -> SD mutex not acquired");
    }
}

void MySD::loadConfig(Types::ConfigData &config, const std::string &filePath)
{
    if (m_mutex && xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE) {
        if (begin()) {
            File configurationFile = SD.open(filePath.c_str(), FILE_READ);
            if (!configurationFile) {
                LOG_ERROR(
                    "MySD::loadConfig -> Failed to read configuration file. Creating file...");
                // Create default file inline to avoid recursion while holding mutex
                File newFile = SD.open(filePath.c_str(), FILE_WRITE);
                if (newFile) {
                    StaticJsonDocument<512> doc;
                    doc["ssid"] = config.ssid.c_str();
                    doc["password"] = config.password.c_str();
                    doc["lcdLockTime"] = config.lcdLockTime;
                    doc["timeBetweenSavingSamples"] = config.timeBetweenSavingSamples;
                    doc["measurePeriod"] = config.measurePeriod;
                    doc["numberOfSamples"] = config.numberOfSamples;
                    doc["turnFanTime"] = config.turnFanTime;
                    doc["timeOffset"] = config.timeOffset;
                    serializeJson(doc, newFile);
                    newFile.close();
                }
            } else {
                LOG_SD("MySD::loadConfig -> Configuration file exists. Reading...");
                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, configurationFile);
                if (!error) {
                    const char *ssid = doc["ssid"];
                    const char *password = doc["password"];
                    config.ssid = ssid ? String(ssid) : "";
                    config.password = password ? String(password) : "";
                    config.lcdLockTime = doc["lcdLockTime"];
                    config.timeBetweenSavingSamples = doc["timeBetweenSavingSamples"];
                    config.measurePeriod = doc["measurePeriod"];
                    config.numberOfSamples = doc["numberOfSamples"];
                    config.turnFanTime = doc["turnFanTime"];
                    config.timeOffset = doc["timeOffset"];
                } else {
                    LOG_ERROR(
                        "MySD::loadConfig -> Failed to read file, using default configuration");
                }
                configurationFile.close();
            }
        } else {
            LOG_ERROR("MySD::loadConfig -> Can't access SD card.");
        }
        end();
        xSemaphoreGive(m_mutex);
    } else {
        LOG_ERROR("MySD::loadConfig -> SD mutex not acquired");
    }
}

void MySD::printConfig(const std::string &filePath)
{
    if (m_mutex && xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE) {
        if (begin()) {
            File configurationFile = SD.open(filePath.c_str(), FILE_READ);
            if (!configurationFile) {
                LOG_ERROR("MySD::printConfig -> Failed to read configuration file.");
            } else {
                LOG_SD("MySD::printConfig -> Configuration file exists. Reading...");
                LOG_SD("MySD::printConfig -> Printing configuration file:");
                while (configurationFile.available()) {
                    char c = (char)configurationFile.read();
                    LOG_SD_PRINTF("%c", c);
                }
                LOG_SD("");
                configurationFile.close();
            }
        } else {
            LOG_ERROR("MySD::printConfig -> Can't access SD card.");
        }
        end();
        xSemaphoreGive(m_mutex);
    } else {
        LOG_ERROR("MySD::printConfig -> SD mutex not acquired");
    }
}
#include "managers/MySD.hpp"

#include "utils/config.h"

MySD::MySD(int port)
    : m_sampleDB("/sd/database.db", "/database.db", "samples")
{
    _port = port;
}

bool MySD::begin()
{
    return SD.begin(_port);
}

void MySD::end()
{
    SD.end();
}

bool MySD::start()
{
    bool result = begin();

    if (result) {
        LOG_SD("MySD::start -> Initializing.");
        m_sampleDB.init();
        m_sampleDB.open();
        m_sampleDB.createTable();
        m_sampleDB.close();
        m_sampleDB.kill();
    } else
        LOG_ERROR("MySD::start -> Can't initilize - no access to SD card.");
    end();
    return result;
}

void MySD::save(std::map<std::string, float> data,
                float temperature,
                float humidity,
                String timestamp)
{
    LOG_SD("MySD::save");
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
    } else
        LOG_ERROR("MySD::save -> Can't save - no access to SD card.");
    end();
}

void MySD::select(String datetime, JsonArray *array)
{
    LOG_SD("MySD::select");
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
    } else
        LOG_ERROR("MySD::select -> Can't access SD card.");
    end();
}

void MySD::getLastRecord(JsonArray *array)
{
    LOG_SD("MySD::getLastRecord");
    if (begin()) {
        LOG_SD("MySD::getLastRecord -> SD Card detected");
        if (SD.exists(m_sampleDB.getRelativePath())) {
            LOG_SD("MySD::getLastRecord -> File exists. Trying to get last saved sample record.");
            LOG_SD("Database " + m_sampleDB.getLocalPath() + " exists.");
            m_sampleDB.init();
            m_sampleDB.open();
            m_sampleDB.getLastRecord(array);
            m_sampleDB.close();
            m_sampleDB.kill();
        } else
            LOG_SD("MySD::getLastRecord -> File don't exist.");
    } else
        LOG_ERROR("MySD::getLastRecord -> Can't access SD card.");
    end();
}

void MySD::saveConfig(const Types::ConfigData &config, const std::string &filePath)
{
    if (begin()) {
        SD.remove(filePath.c_str());
        File configurationFile = SD.open(filePath.c_str(), FILE_WRITE);
        if (!configurationFile) {
            LOG_ERROR("MySD::saveConfig -> Failed to create configuration file.");
            return;
        } else
            LOG_SD("Configuration file exists. Trying to save...");
        StaticJsonDocument<512> doc;
        doc["ssid"] = config.ssid.c_str();
        doc["password"] = config.password.c_str();
        doc["lcdLockTime"] = config.lcdLockTime;
        doc["timeBetweenSavingSamples"] = config.timeBetweenSavingSamples;
        doc["measurePeriod"] = config.measurePeriod;
        doc["numberOfSamples"] = config.numberOfSamples;
        doc["turnFanTime"] = config.turnFanTime;
        if (serializeJson(doc, configurationFile) == 0) {
            LOG_ERROR("MySD::saveConfig -> Failed to write to file.");
        }
        configurationFile.close();
    }
    end();
}

void MySD::loadConfig(Types::ConfigData &config, const std::string &filePath)
{
    if (begin()) {
        File configurationFile = SD.open(filePath.c_str(), FILE_READ);
        if (!configurationFile) {
            LOG_ERROR("MySD::loadConfig -> Failed to read configuration file. Creating file...");
            configurationFile.close();
            end();
            saveConfig(config, filePath);
            return;
        } else
            LOG_SD("MySD::loadConfig -> Configuration file exists. Reading...");

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, configurationFile);

        if (error) {
            LOG_ERROR("MySD::loadConfig -> Failed to read file, using default configuration");
            return;
        }

        const char *ssid = doc["ssid"];
        const char *password = doc["password"];
        config.ssid = ssid ? String(ssid) : "";
        config.password = password ? String(password) : "";
        config.lcdLockTime = doc["lcdLockTime"];
        config.timeBetweenSavingSamples = doc["timeBetweenSavingSamples"];
        config.measurePeriod = doc["measurePeriod"];
        config.numberOfSamples = doc["numberOfSamples"];
        config.turnFanTime = doc["turnFanTime"];
        configurationFile.close();
    }
    end();
}

void MySD::printConfig(const std::string &filePath)
{
    if (begin()) {
        File configurationFile = SD.open(filePath.c_str(), FILE_READ);
        if (!configurationFile) {
            LOG_ERROR("MySD::printConfig -> Failed to read configuration file.");
            return;
        } else
            LOG_SD("MySD::printConfig -> Configuration file exists. Reading...");
        LOG_SD("MySD::printConfig -> Printing configuration file:");
        while (configurationFile.available())
            LOG_SD_PRINTF("%c", (char)configurationFile.read());
        LOG_SD("");
        configurationFile.close();
    }
    end();
}
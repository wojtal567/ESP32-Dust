#include <MySD.hpp>

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

bool MySD::start(Stream *debugger)
{
    bool result = begin();

    if (result)
    {
        Serial.println("MySD::start -> Initializing.");
        m_sampleDB.init();
        m_sampleDB.open();
        m_sampleDB.createTable(debugger);
        m_sampleDB.close();
        m_sampleDB.kill();
    }
    else
        Serial.println("MySD::start -> Can't initilize - no access to SD card.");
    end();
    return result;
}

void MySD::save(std::map<std::string, float> data,
                float temperature,
                float humidity,
                String timestamp,
                Stream *debugger)
{
    debugger->println("MySD::save");
    if (begin())
    {
        debugger->println("SD Card detected");
        if (SD.exists(m_sampleDB.getRelativePath())) {
            Serial.println("MySD::save -> File exists. Trying to save data.");
            debugger->println("Database " + m_sampleDB.getLocalPath() + " exists. Saving data...");
            m_sampleDB.init();
            m_sampleDB.open();
            m_sampleDB.save(data, temperature, humidity, timestamp, debugger);
            m_sampleDB.close();
            m_sampleDB.kill();
        } else {
            Serial.println("MySD::save -> No database file. Trying to create one and saving data.");
            debugger->println("Database " + m_sampleDB.getLocalPath()
                              + " don't exist. Saving data...");
            m_sampleDB.init();
            m_sampleDB.open();
            m_sampleDB.createTable(debugger);
            m_sampleDB.save(data, temperature, humidity, timestamp, debugger);
            m_sampleDB.close();
            m_sampleDB.kill();
        }
    }
    else
        Serial.println("MySD::save -> Cannot access SD card.");
    end();
}

void MySD::select(Stream *debugger, String datetime, JsonArray *array)
{
    debugger->println("MySD::select");
    if (begin())
    {
        debugger->println("SD Card detected");
        if (SD.exists(m_sampleDB.getRelativePath())) {
            Serial.println("MySD::select -> File exists. Trying to select data.");
            debugger->println("Database " + m_sampleDB.getLocalPath() + " exists.");
            m_sampleDB.init();
            m_sampleDB.open();
            m_sampleDB.select(debugger, datetime, array);
            m_sampleDB.close();
            m_sampleDB.kill();
        } else
            Serial.println("MySD::select -> File don't exist.");
    }
    else
        Serial.println("MySD::select -> Cannot access SD card.");
    end();
}

void MySD::getLastRecord(Stream *debugger, JsonArray *array)
{
    debugger->println("MySD::getLastRecord");
    if (begin())
    {
        debugger->println("SD Card detected");
        if (SD.exists(m_sampleDB.getRelativePath())) {
            Serial.println("MySD::getLastRecord -> File exists. Trying to get last saved sample record.");
            debugger->println("Database " + m_sampleDB.getLocalPath() + " exists.");
            m_sampleDB.init();
            m_sampleDB.open();
            m_sampleDB.getLastRecord(debugger, array);
            m_sampleDB.close();
            m_sampleDB.kill();
        } else
            Serial.println("MySD::getLastRecord -> File don't exist.");
    }
    else
        Serial.println("MySD::getLastRecord -> Cannot access SD card.");
    end();
}

void MySD::saveConfig(Config config, std::string filePath)
{
    if (begin())
    {
        SD.remove(filePath.c_str());
        File configurationFile = SD.open(filePath.c_str(), FILE_WRITE);
        if (!configurationFile)
        {
            Serial.println("Failed to create configuration file.");
            return;
        }
        else
            Serial.println("Configuration file exists. Trying to save...");
        StaticJsonDocument<512> doc;
        doc["ssid"] = config.ssid.c_str();
        doc["password"] = config.password.c_str();
        doc["lcdLockTime"] = config.lcdLockTime;
        doc["timeBetweenSavingSamples"] = config.timeBetweenSavingSamples;
        doc["measurePeriod"] = config.measurePeriod;
        doc["numberOfSamples"] = config.numberOfSamples;
        doc["turnFanTime"] = config.turnFanTime;
        if (serializeJson(doc, configurationFile) == 0)
        {
            Serial.println("Failed to write to file.");
        }
        configurationFile.close();
    }
    end();
}

void MySD::loadConfig(Config &config, std::string filePath)
{
    if (begin())
    {
        File configurationFile = SD.open(filePath.c_str(), FILE_READ);
        if (!configurationFile)
        {
            Serial.print("Failed to read configuration file. Creating file...");
            configurationFile.close();
            end();
            saveConfig(config, filePath);
            return;
        }
        else
            Serial.println("Configuration file exists. Reading...");

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, configurationFile);

        if (error)
        {
            Serial.println("Failed to read file, using default Configuration");
            return;
        }

        char tmp[64];
        strlcpy(tmp, doc["ssid"], sizeof(tmp));
        config.ssid = tmp;
        strlcpy(tmp, doc["password"], sizeof(tmp));
        config.password = tmp;
        config.lcdLockTime = doc["lcdLockTime"];
        config.timeBetweenSavingSamples = doc["timeBetweenSavingSamples"];
        config.measurePeriod = doc["measurePeriod"];
        config.numberOfSamples = doc["numberOfSamples"];
        config.turnFanTime = doc["turnFanTime"];
        configurationFile.close();
        Serial.println("Reading config and making changes.");
    }
    end();
}

void MySD::printConfig(std::string filePath)
{
    if (begin())
    {
        File configurationFile = SD.open(filePath.c_str(), FILE_READ);
        if (!configurationFile)
        {
            Serial.print("Failed to read configuration file.");
            return;
        }
        else
            Serial.println("Configuration file exists. Reading...");
        Serial.println("Current config.json file:");
        while (configurationFile.available())
            Serial.print((char)configurationFile.read());
        Serial.println();
        configurationFile.close();
    }
    end();
}

void MySD::loadWiFi(Config &config, std::string filePath)
{
    if (begin())
    {
        File configurationFile = SD.open(filePath.c_str(), FILE_READ);
        if (!configurationFile)
        {
            Serial.print("Failed to read configuration file. Creating file...");
            configurationFile.close();
            end();
            saveConfig(config, filePath);
            return;
        }
        else
            Serial.println("Configuration file exists. Reading...");

        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, configurationFile);

        if (error)
        {
            Serial.println("Failed to read file, using default Configuration");
            return;
        }

        char tmp[64];
        strlcpy(tmp, doc["ssid"], sizeof(tmp));
        config.ssid = tmp;
        strlcpy(tmp, doc["password"], sizeof(tmp));
        config.password = tmp;
        configurationFile.close();
        Serial.println("Succesfully loaded Wi-Fi credentials.");
    }
    end();
}
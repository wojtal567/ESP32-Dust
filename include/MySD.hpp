#pragma once

#include <SD.h>
#include <SQLiteDb.hpp>
#include <string>
#include "Utils/types.h"

// TODO replace by Types::ConfigData
struct Config
{
    std::string ssid;            // * WiFi ssid
    std::string password;        // * WiFi password
    int lcdLockTime;             // * time after
    int timeBetweenSavingSamples; // * time between sampling @measurePeriod
    int measurePeriod;           // * time of taking samples to calculate average
    uint numberOfSamples;        // * count of samples used to calculate average sample
    int turnFanTime;             // * time of turning fan on
};

class MySD
{
private:
    int _port;
    SQLiteDb m_sampleDB;

public:
    MySD(int port);
    bool begin();
    void end();
    bool start(Stream *debugger);
    void save(std::map<std::string, float> data,
              float temperature,
              float humidity,
              String timestamp,
              Stream *debugger);
    void select(Stream *debugger, String datetime, JsonArray *array);
    void getLastRecord(Stream *debugger, JsonArray *array);
    void saveConfig(Config config, std::string filePath);
    void loadConfig(Config &config, std::string filePath);
    void printConfig(std::string filePath);
    void loadWiFi(Config &config, std::string filePath);
};

#pragma once

#include <SD.h>
#include <string>
#include "SQLiteDb.hpp"
#include "../utils/types.h"

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
    void saveConfig(const Types::ConfigData &config, const std::string &filePath);
    void loadConfig(Types::ConfigData &config, const std::string &filePath);
    void printConfig(const std::string &filePath);
};

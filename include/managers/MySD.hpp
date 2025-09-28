#pragma once

#include <SD.h>
#include <string>
#include "../utils/types.h"
#include "SQLiteDb.hpp"

class MySD
{
public:
    MySD(int port);
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

private:
    bool begin();
    void end();

    int _port;
    SQLiteDb m_sampleDB;
};

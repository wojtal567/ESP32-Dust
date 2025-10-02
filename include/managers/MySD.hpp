#pragma once

#include <SD.h>
#include <string>
#include "SQLiteDb.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "utils/types.h"

class MySD
{
public:
    MySD(int port);
    bool start();
    void save(std::map<std::string, float> data,
              float temperature,
              float humidity,
              String timestamp);
    void select(String datetime, JsonArray *array);
    void getLastRecord(JsonArray *array);
    void saveConfig(const Types::ConfigData &config, const std::string &filePath);
    void loadConfig(Types::ConfigData &config, const std::string &filePath);
    void printConfig(const std::string &filePath);

private:
    bool begin();
    void end();

    int m_port;
    SQLiteDb m_sampleDB;
    SemaphoreHandle_t m_mutex{nullptr};
};

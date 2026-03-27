#pragma once

#include <map>
#include "utils/types.h"

class HardwareSerial;

class PMS
{
public:
    PMS(HardwareSerial *reader);

    bool readData();
    std::map<std::string, float> returnData();
    void dumpSamples();

    static const std::array<std::string, 15> SENSOR_LABELS;

private:
    HardwareSerial *m_reader;
    std::map<std::string, float> m_data;
};
#pragma once

#include <HardwareSerial.h>
#include <map>
#include "utils/types.h"

class PMS
{
public:
    PMS(HardwareSerial *debugger, HardwareSerial *reader = nullptr);

    bool readData();
    std::map<std::string, float> returnData();
    void dumpSamples();

    static const std::array<std::string, 15> SENSOR_LABELS;

private:
    HardwareSerial *m_reader, *m_debugger;
    std::map<std::string, float> m_data;
};
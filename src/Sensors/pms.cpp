#include "sensors/pms.h"
#include "utils/constants.h"

const std::array<std::string, 15> PMS::SENSOR_LABELS = {"framelen",
                                                        "pm10_standard",
                                                        "pm25_standard",
                                                        "pm100_standard",
                                                        "pm10_env",
                                                        "pm25_env",
                                                        "pm100_env",
                                                        "particles_03um",
                                                        "particles_05um",
                                                        "particles_10um",
                                                        "particles_25um",
                                                        "particles_50um",
                                                        "particles_100um",
                                                        "unused",
                                                        "checksum"};

PMS::PMS(HardwareSerial *debugger, HardwareSerial *reader)
    : m_reader(reader)
    , m_debugger(debugger)
{}

bool PMS::readData()
{
    /*
    if (!_debugger->available())
    {
        _reader->println("Debugger unavailable");
        return false;
    }*/
    if (m_reader->peek() != Constants::PMS_START_BYTE) {
        m_reader->read();
        m_debugger->println("PMS5003 -> Reading bytes...");
        return false;
    }

    if (m_reader->available() < Constants::PMS_BUFFER_SIZE) {
        m_debugger->println("PMS5003 -> Can't read all bytes from PMS. Exiting. False.");
        return false;
    }

    uint8_t buffer[Constants::PMS_BUFFER_SIZE];    int32_t checkSum{0};

    m_reader->readBytes(buffer, Constants::PMS_BUFFER_SIZE);

    for (uint8_t i = 0; i < 30; i++) {
        checkSum += buffer[i];
    }

    m_debugger->println();

    int32_t buffer_u16[15];

    for (uint8_t i = 0; i < SENSOR_LABELS.size(); i++) {
        buffer_u16[i] = buffer[2 + i * 2 + 1];
        buffer_u16[i] += (buffer[2 + i * 2] << 8);
        m_data[SENSOR_LABELS[i]] = buffer_u16[i];
    }

    if (checkSum != m_data["checksum"]) {
        m_debugger->println("PMS5003 -> Checksum failure. False.");
        return false;
    }
    return true;
}

std::map<std::string, float> PMS::returnData()
{
    return m_data;
}

void PMS::dumpSamples()
{
    for (uint16_t i = 0; i < SENSOR_LABELS.size(); i++) {
        m_debugger->println((String)SENSOR_LABELS[i].c_str() + " "
                            + (String)m_data[SENSOR_LABELS[i]]);
    }
}

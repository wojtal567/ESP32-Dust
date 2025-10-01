#include "sensors/pms.h"

#include <HardwareSerial.h>

#include "utils/config.h"
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

PMS::PMS(HardwareSerial *reader)
    : m_reader(reader)
{}

bool PMS::readData()
{
    if (m_reader->peek() != Constants::PMS_START_BYTE) {
        // Clear multiple bytes if needed to find sync
        uint8_t attempts = 0;
        while (m_reader->available() > 0 && m_reader->peek() != Constants::PMS_START_BYTE
               && attempts < 50) {
            m_reader->read();
            attempts++;
        }
        LOG_SENSOR("PMS5003 -> Reading bytes...");
        return false;
    }

    if (m_reader->available() < Constants::PMS_BUFFER_SIZE) {
        LOG_SENSOR("PMS5003 -> Can't read all bytes from PMS. Exiting. False.");
        return false;
    }

    uint8_t buffer[Constants::PMS_BUFFER_SIZE];
    int32_t checkSum{0};

    m_reader->readBytes(buffer, Constants::PMS_BUFFER_SIZE);

    for (uint8_t i = 0; i < 30; i++) {
        checkSum += buffer[i];
    }

    int32_t buffer_u16[15];

    for (uint8_t i = 0; i < SENSOR_LABELS.size(); i++) {
        buffer_u16[i] = buffer[2 + i * 2 + 1];
        buffer_u16[i] += (buffer[2 + i * 2] << 8);
        m_data[SENSOR_LABELS[i]] = buffer_u16[i];
    }

    if (checkSum != m_data["checksum"]) {
        LOG_SENSOR("PMS5003 -> Checksum failure. False.");

        // Clear buffer after checksum failure to prevent cascading failures
        while (m_reader->available() > 0) {
            m_reader->read();
        }

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
        LOG_SENSOR((String)SENSOR_LABELS[i].c_str() + " " + (String)m_data[SENSOR_LABELS[i]]);
    }
}

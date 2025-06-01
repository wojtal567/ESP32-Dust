#include <PMS5003.hpp>

PMS5003::PMS5003(HardwareSerial *reader, HardwareSerial *debugger)
{
    _reader = reader;
    _debugger = debugger;
    for (uint16_t i = 0; i < VALUES; i++)
        data.insert(std::pair<std::string, int32_t>(std::string(labels[i]), i));
}

bool PMS5003::readData()
{
    /*
    if (!_debugger->available()) 
    {
        _reader->println("Debugger unavailable");
        return false;
    }*/
    if (_reader->peek() != 0x42)
    {
        _reader->read();
        _debugger->println("PMS5003 -> Reading bytes...");
        return false;
    }

    if (_reader->available() < 32)
    {
        _debugger->println("PMS5003 -> Can't read all bytes from PMS. Exiting. False.");
        return false;
    }

    uint8_t buffer[BUFFERSIZE];
    int32_t checkSum{0};

    _reader->readBytes(buffer, BUFFERSIZE);

    for (uint8_t i = 0; i < 30; i++)
    {
        checkSum += buffer[i];
    }

    _debugger->println();

    int32_t buffer_u16[15];
    for (uint8_t i = 0; i < VALUES; i++)
    {
        buffer_u16[i] = buffer[2 + i * 2 + 1];
        buffer_u16[i] += (buffer[2 + i * 2] << 8);
        data[labels[i]] = buffer_u16[i];
    }

    if (checkSum != data["checksum"])
    {
        _debugger->println("PMS5003 -> Checksum failure. False.");
        return false;
    }
    return true;
}

std::map<std::string, float> PMS5003::returnData()
{
    return data;
}

void PMS5003::dumpSamples()
{
    for (uint16_t i = 0; i < VALUES; i++)
        _debugger->println((String)labels[i] + " " + (String)data[labels[i]]);
}

Types::ParticleData PMS5003::getParticleData()
{
    Types::ParticleData particleData;
    
    // Fill the structured data from the map
    particleData.pm1_0 = data["pm10_standard"];
    particleData.pm2_5 = data["pm25_standard"];
    particleData.pm10_0 = data["pm100_standard"];
    particleData.particles_0_3 = data["particles_03um"];
    particleData.particles_0_5 = data["particles_05um"];
    particleData.particles_1_0 = data["particles_10um"];
    particleData.particles_2_5 = data["particles_25um"];
    particleData.particles_5_0 = data["particles_50um"];
    particleData.particles_10_0 = data["particles_100um"];
    
    return particleData;
}

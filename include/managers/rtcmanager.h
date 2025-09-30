#pragma once

#include <RtcDS1307.h>
#include <Wire.h>
#include <NTPClient.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class RTCManager {
public:
    RTCManager() = default; 
    RTCManager(long gmtOffsetInSeconds, int daylightOffsetInSeconds, char* ntpServer);
    
    ~RTCManager() = default;
    
    bool syncWithNTP(const char* ntpServer, long gmtOffset);
    
    String getDate();
    String getTime();
    
    RtcDateTime getCurrentDateTime();
    void setDateTime(const RtcDateTime& dateTime);
    bool isRunning();
    void setIsRunning(bool running);

private:

    RtcDS1307<TwoWire> m_rtcDevice{RtcDS1307<TwoWire>(Wire)};
    SemaphoreHandle_t m_i2cMutex{nullptr};

    long m_gmtOffsetInSeconds;
    int m_daylightOffsetInSeconds;
    char *m_ntpServer;
};

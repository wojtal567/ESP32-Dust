#include "managers/rtcmanager.h"
#include <WiFi.h>
#include <WiFiUdp.h>

RTCManager::RTCManager(long gmtOffsetInSeconds, int daylightOffsetInSeconds, char *ntpServer)
    : m_gmtOffsetInSeconds(gmtOffsetInSeconds)
    , m_daylightOffsetInSeconds(daylightOffsetInSeconds)
    , m_ntpServer(ntpServer)
{
    m_rtcDevice.Begin();

    // Check if RTC is already running and has a reasonable date
    if (m_rtcDevice.GetIsRunning()) {
        RtcDateTime currentTime = m_rtcDevice.GetDateTime();
        Serial.println("RTC is already running with valid time - not overriding");
        Serial.printf("Current RTC time: %02u.%02u.%04u %02u:%02u:%02u\n",
                      currentTime.Day(),
                      currentTime.Month(),
                      currentTime.Year(),
                      currentTime.Hour(),
                      currentTime.Minute(),
                      currentTime.Second());
    }
}

bool RTCManager::syncWithNTP(const char *ntpServer, long gmtOffset)
{
    if (!WiFi.isConnected()) {
        Serial.println("WiFi not connected - cannot sync time with NTP.");
        return false;
    }

    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, ntpServer, gmtOffset);

    timeClient.begin();
    bool success = timeClient.update();

    if (success) {
        time_t rawtime = timeClient.getEpochTime();
        struct tm *ti;
        ti = localtime(&rawtime);

        uint16_t year = ti->tm_year + 1900;
        uint8_t month = ti->tm_mon + 1;
        uint8_t day = ti->tm_mday;
        uint8_t hours = ti->tm_hour;
        uint8_t minutes = ti->tm_min;
        uint8_t seconds = ti->tm_sec;

        RtcDateTime date = RtcDateTime(year, month, day, hours, minutes, seconds);
        m_rtcDevice.SetDateTime(date);
        m_rtcDevice.SetIsRunning(true);
        Serial.println("Successfully updated time on RTC from NTP server.");
        Serial.printf("Timezone offset applied: %ld seconds (%ld hours)\n",
                      gmtOffset,
                      gmtOffset / 3600);
    } else if (!success) {
        Serial.println("Failed to get time from NTP server.");
    } else {
        Serial.println("RTC device not available.");
    }

    timeClient.end();
    return success;
}

String RTCManager::getDate()
{
    RtcDateTime dt = m_rtcDevice.GetDateTime();
    char datestring[20];
    snprintf_P(datestring, 20, PSTR("%02u.%02u.%04u"), dt.Day(), dt.Month(), dt.Year());
    return String(datestring);
}

String RTCManager::getTime()
{
    RtcDateTime dt = m_rtcDevice.GetDateTime();
    char timestring[20];
    snprintf_P(timestring, 20, PSTR("%02u:%02u:%02u"), dt.Hour(), dt.Minute(), dt.Second());
    return String(timestring);
}


RtcDateTime RTCManager::getCurrentDateTime()
{
    return m_rtcDevice.GetDateTime();
}

void RTCManager::setDateTime(const RtcDateTime &dateTime)
{
    m_rtcDevice.SetDateTime(dateTime);
}

bool RTCManager::isRunning()
{
    return m_rtcDevice.GetIsRunning();
}

void RTCManager::setIsRunning(bool running)
{
    m_rtcDevice.SetIsRunning(running);
}

#include "managers/rtcmanager.h"

#include <WiFi.h>
#include <WiFiUdp.h>

#include "utils/config.h"

RTCManager::RTCManager(long gmtOffsetInSeconds, int daylightOffsetInSeconds, char *ntpServer)
    : m_gmtOffsetInSeconds(gmtOffsetInSeconds)
    , m_daylightOffsetInSeconds(daylightOffsetInSeconds)
    , m_ntpServer(ntpServer)
{
    m_i2cMutex = xSemaphoreCreateMutex();
    m_rtcDevice.Begin();

    // Check if RTC is already running and has a reasonable date
    if (m_rtcDevice.GetIsRunning()) {
        RtcDateTime currentTime = m_rtcDevice.GetDateTime();
        LOG_INFO("RTC is already running with valid time - not overriding");
        char msg[64];
        snprintf(msg,
                 sizeof(msg),
                 "Current RTC time: %02u.%02u.%04u %02u:%02u:%02u",
                 currentTime.Day(),
                 currentTime.Month(),
                 currentTime.Year(),
                 currentTime.Hour(),
                 currentTime.Minute(),
                 currentTime.Second());
        LOG_INFO(msg);
    }
}

bool RTCManager::syncWithNTP(const char *ntpServer, long gmtOffset)
{
    if (!WiFi.isConnected()) {
        LOG_DEBUG("WiFi not connected - cannot sync time with NTP.");
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
        if (m_i2cMutex && xSemaphoreTake(m_i2cMutex, portMAX_DELAY)) {
            m_rtcDevice.SetDateTime(date);
            m_rtcDevice.SetIsRunning(true);
            xSemaphoreGive(m_i2cMutex);
        } else {
            m_rtcDevice.SetDateTime(date);
            m_rtcDevice.SetIsRunning(true);
        }
        LOG_DEBUG("Successfully updated time on RTC from NTP server.");
        LOG_DEBUG("Timezone offset applied: " + String(gmtOffset) + " seconds ("
                  + String(gmtOffset / 3600) + " hours)");
    } else if (!success) {
        LOG_ERROR("Failed to get time from NTP server.");
    } else {
        LOG_ERROR("RTC device not available.");
    }

    timeClient.end();
    return success;
}

String RTCManager::getDate()
{
    RtcDateTime dt;
    if (m_i2cMutex && xSemaphoreTake(m_i2cMutex, portMAX_DELAY)) {
        dt = m_rtcDevice.GetDateTime();
        xSemaphoreGive(m_i2cMutex);
    } else {
        dt = m_rtcDevice.GetDateTime();
    }
    char datestring[20];
    snprintf_P(datestring, 20, PSTR("%02u.%02u.%04u"), dt.Day(), dt.Month(), dt.Year());
    return String(datestring);
}

String RTCManager::getTime()
{
    RtcDateTime dt;
    if (m_i2cMutex && xSemaphoreTake(m_i2cMutex, portMAX_DELAY)) {
        dt = m_rtcDevice.GetDateTime();
        xSemaphoreGive(m_i2cMutex);
    } else {
        dt = m_rtcDevice.GetDateTime();
    }
    char timestring[20];
    snprintf_P(timestring, 20, PSTR("%02u:%02u:%02u"), dt.Hour(), dt.Minute(), dt.Second());
    return String(timestring);
}

RtcDateTime RTCManager::getCurrentDateTime()
{
    RtcDateTime dt;
    if (m_i2cMutex && xSemaphoreTake(m_i2cMutex, portMAX_DELAY)) {
        dt = m_rtcDevice.GetDateTime();
        xSemaphoreGive(m_i2cMutex);
    } else {
        dt = m_rtcDevice.GetDateTime();
    }
    return dt;
}

void RTCManager::setDateTime(const RtcDateTime &dateTime)
{
    if (m_i2cMutex && xSemaphoreTake(m_i2cMutex, portMAX_DELAY)) {
        m_rtcDevice.SetDateTime(dateTime);
        xSemaphoreGive(m_i2cMutex);
    } else {
        m_rtcDevice.SetDateTime(dateTime);
    }
}

bool RTCManager::isRunning()
{
    bool running = false;
    if (m_i2cMutex && xSemaphoreTake(m_i2cMutex, portMAX_DELAY)) {
        running = m_rtcDevice.GetIsRunning();
        xSemaphoreGive(m_i2cMutex);
    } else {
        running = m_rtcDevice.GetIsRunning();
    }
    return running;
}

void RTCManager::setIsRunning(bool running)
{
    if (m_i2cMutex && xSemaphoreTake(m_i2cMutex, portMAX_DELAY)) {
        m_rtcDevice.SetIsRunning(running);
        xSemaphoreGive(m_i2cMutex);
    } else {
        m_rtcDevice.SetIsRunning(running);
    }
}

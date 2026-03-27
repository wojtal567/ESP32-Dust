#pragma once

#include <Arduino.h>
#include <time.h>

// Define countof macro for RTC library compatibility
#ifndef countof
#define countof(a) (sizeof(a) / sizeof(a[0]))
#endif

#include <RtcDateTime.h>

namespace Utils {
// time formatting functions
inline String formatTime(const struct tm &timeInfo)
{
    char timeString[9];
    sprintf(timeString, "%02d:%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
    return String(timeString);
}

inline String formatDate(const struct tm &timeInfo)
{
    char dateString[11];
    sprintf(dateString,
            "%02d-%02d-%04d",
            timeInfo.tm_mday,
            timeInfo.tm_mon + 1,
            timeInfo.tm_year + 1900);
    return String(dateString);
}

inline String formatDateTime(const struct tm &timeInfo)
{
    return formatDate(timeInfo) + " " + formatTime(timeInfo);
}

// AQI calculation functions
float calculatePM25AQI(float pm25);
float calculatePM10AQI(float pm10);
float calculateOverallAQI(float pm25, float pm10);

String formatMainTimestamp(const RtcDateTime &timestamp);
} // namespace Utils

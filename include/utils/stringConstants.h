#pragma once

namespace StringConstants {
// UI symbols
constexpr const char *LOCK_SYMBOL = "\xEF\x80\xA3";
constexpr const char *UNLOCK_SYMBOL = "\xEF\x82\x9C";
constexpr const char *INFO_SYMBOL = "\xEF\x81\x9A";
constexpr const char *CLOCK_SYMBOL = "\xEF\x80\x97";
constexpr const char *WIFI_SYMBOL = "\xEF\x87\xAB";
constexpr const char *COGS_SYMBOL = "\xEF\x82\x85";

// File paths
constexpr const char *CONFIG_FILE_PATH = "/settings.json";

// Time
constexpr const char *NTP_SERVER = "europe.pool.ntp.org";

// Air quality states
constexpr const char *AIR_QUALITY_EXCELLENT = "Excellent";
constexpr const char *AIR_QUALITY_GOOD = "Good";
constexpr const char *AIR_QUALITY_MODERATE = "Moderate";
constexpr const char *AIR_QUALITY_UNHEALTHY = "Unhealthy";
constexpr const char *AIR_QUALITY_BAD = "Bad";
constexpr const char *AIR_QUALITY_HAZARDOUS = "Hazardous";

inline const String PARTICLES_SIZE[7] = {"0.0", "0.3", "0.5", "1.0", "2.5", "5.0", "10.0"};

} // namespace StringConstants

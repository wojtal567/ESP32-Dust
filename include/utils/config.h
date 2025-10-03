#pragma once

namespace HardwareConfig {
// PMS5003 Dust Sensor Pins
constexpr int FAN_PIN = 26; // Fan control pin

// I2C Bus Configuration (shared by SHT30 and RTC)
constexpr int I2C_SDA_PIN = 21;
constexpr int I2C_SCL_PIN = 22;

// SHT30 Temperature/Humidity Sensor
constexpr int SHT30_I2C_ADDRESS = 0x44;

// SD Card Pin
constexpr int SD_CARD_PIN = 27; // SD_CS pin
} // namespace HardwareConfig

// Logging Configuration
namespace LogConfig {
// Set to false to disable all serial logging (for production builds)
constexpr bool ENABLE_LOGGING = true;

// Log level control (only applies when ENABLE_LOGGING = true)
constexpr bool ENABLE_DEBUG_LOGS = true;
constexpr bool ENABLE_INFO_LOGS = true;
constexpr bool ENABLE_ERROR_LOGS = true;

// Component-specific logging (only applies when ENABLE_LOGGING = true)
constexpr bool ENABLE_SENSOR_LOGS = true;
constexpr bool ENABLE_NETWORK_LOGS = true;
constexpr bool ENABLE_DATABASE_LOGS = true;
constexpr bool ENABLE_UI_LOGS = true;
constexpr bool ENABLE_TASK_LOGS = true;
constexpr bool ENABLE_SD_LOGS = true;
} // namespace LogConfig

// Usage Examples:
// For production build: Set ENABLE_LOGGING to false
// For debugging sensors only: Set ENABLE_SENSOR_LOGS to true, others to false
// For minimal logging: Set only ENABLE_ERROR_LOGS to true

// Logging Macros - Use these instead of Serial.println
#define LOG_PRINT(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING) { \
            Serial.print(msg); \
        } \
    } while (0)

#define LOG_PRINTLN(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING) { \
            Serial.println(msg); \
        } \
    } while (0)

#define LOG_PRINTF(fmt, ...) \
    do { \
        if (LogConfig::ENABLE_LOGGING) { \
            Serial.printf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

// Component-specific logging macros
#define LOG_DEBUG(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_DEBUG_LOGS) { \
            Serial.println("[DEBUG] " + String(msg)); \
        } \
    } while (0)

#define LOG_INFO(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_INFO_LOGS) { \
            Serial.println("[INFO] " + String(msg)); \
        } \
    } while (0)

#define LOG_ERROR(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_ERROR_LOGS) { \
            Serial.println("[ERROR] " + String(msg)); \
        } \
    } while (0)

#define LOG_SENSOR(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_SENSOR_LOGS) { \
            Serial.println("[SENSOR] " + String(msg)); \
        } \
    } while (0)

#define LOG_NETWORK(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_NETWORK_LOGS) { \
            Serial.println("[NETWORK] " + String(msg)); \
        } \
    } while (0)

#define LOG_DATABASE(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_DATABASE_LOGS) { \
            Serial.println("[DATABASE] " + String(msg)); \
        } \
    } while (0)

#define LOG_UI(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_UI_LOGS) { \
            Serial.println("[UI] " + String(msg)); \
        } \
    } while (0)

#define LOG_TASK(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_TASK_LOGS) { \
            Serial.println("[TASK] " + String(msg)); \
        } \
    } while (0)

#define LOG_SD(msg) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_SD_LOGS) { \
            Serial.println("[SD] " + String(msg)); \
        } \
    } while (0)

#define LOG_SD_PRINTF(fmt, ...) \
    do { \
        if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_SD_LOGS) { \
            Serial.printf(fmt, ##__VA_ARGS__); \
        } \
    } while (0)

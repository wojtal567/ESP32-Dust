#pragma once

namespace HardwareConfig {
    // PMS5003 Dust Sensor Pins
    constexpr int FAN_PIN = 26;      // Fan control pin
    
    // SHT30 Temperature/Humidity Sensor (I2C)
    constexpr int SHT30_SDA_PIN = 21;
    constexpr int SHT30_SCL_PIN = 22;
    constexpr int SHT30_I2C_ADDRESS = 0x44;
    
    // SD Card Pin
    constexpr int SD_CARD_PIN = 27;   // SD_CS pin
    
    // RTC I2C Address
    constexpr int RTC_I2C_ADDRESS = 0x68;
}

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
}

// Usage Examples:
// For production build: Set ENABLE_LOGGING to false
// For debugging sensors only: Set ENABLE_SENSOR_LOGS to true, others to false
// For minimal logging: Set only ENABLE_ERROR_LOGS to true

// Logging Macros - Use these instead of Serial.println
#define LOG_PRINT(msg) ({ if (LogConfig::ENABLE_LOGGING) { Serial.print(msg); } })
#define LOG_PRINTLN(msg) ({ if (LogConfig::ENABLE_LOGGING) { Serial.println(msg); } })
#define LOG_PRINTF(fmt, ...) ({ if (LogConfig::ENABLE_LOGGING) { Serial.printf(fmt, ##__VA_ARGS__); } })

// Component-specific logging macros
#define LOG_DEBUG(msg) ({ if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_DEBUG_LOGS) { Serial.println("[DEBUG] " + String(msg)); } })
#define LOG_INFO(msg) ({ if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_INFO_LOGS) { Serial.println("[INFO] " + String(msg)); } })
#define LOG_ERROR(msg) ({ if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_ERROR_LOGS) { Serial.println("[ERROR] " + String(msg)); } })

#define LOG_SENSOR(msg) ({ if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_SENSOR_LOGS) { Serial.println("[SENSOR] " + String(msg)); } })
#define LOG_NETWORK(msg) ({ if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_NETWORK_LOGS) { Serial.println("[NETWORK] " + String(msg)); } })
#define LOG_DATABASE(msg) ({ if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_DATABASE_LOGS) { Serial.println("[DATABASE] " + String(msg)); } })
#define LOG_UI(msg) ({ if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_UI_LOGS) { Serial.println("[UI] " + String(msg)); } })
#define LOG_TASK(msg) ({ if (LogConfig::ENABLE_LOGGING && LogConfig::ENABLE_TASK_LOGS) { Serial.println("[TASK] " + String(msg)); } })
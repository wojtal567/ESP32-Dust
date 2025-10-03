# ESP32-Dust

Air quality monitoring station with PM sensor, temperature/humidity sensing, and data logging on ESP32 with TFT display.

## Features

- **Hardware**: ESP32 DevKit v1, ILI9341 TFT (320×240), PMS5003 dust sensor, SHT30 temp/humidity sensor, DS1307/DS3231 RTC, SD card
- **UI**: LVGL v7-based interface with multiple screens (Main, Lock, Settings, WiFi setup, Time config, Sampling, Info, Loading)
- **Data logging**: SQLite database on SD card for long-term sample storage
- **Connectivity**: WiFi for NTP time sync and optional data export
- **Configurability**: Hardware pins and logging levels via `include/utils/config.h`

## Getting Started

### Prerequisites

- [Visual Studio Code](https://code.visualstudio.com/) with [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode)
- ESP32 USB drivers installed (CP210x or CH340, depending on your board)

### Initial Setup

#### 1. LVGL Configuration

After the first build, the LVGL library will be downloaded. Copy the config file:

```bash
cp configs/lv_conf.h .pio/libdeps/esp32doit-devkit-v1/lvgl/lv_conf.h
```

**Note**: This file may be overwritten when updating libraries. Re-copy if needed.

#### 2. TFT_eSPI Configuration

After the first build, the TFT_eSPI library will be downloaded to:

```
.pio/libdeps/esp32doit-devkit-v1/TFT_eSPI/
```

Copy the display configuration:

```bash
cp configs/TFTSetup.h .pio/libdeps/esp32doit-devkit-v1/TFT_eSPI/User_Setup.h
```

**Note**: If you update libraries via PlatformIO, you may need to re-copy this file.

### Build & Upload

1. **Build**: Click the PlatformIO "Build" button (checkmark icon) or run:
   ```bash
   pio run -e esp32doit-devkit-v1
   ```

2. **Upload**: Connect your ESP32 via USB, then click "Upload" (arrow icon) or:
   ```bash
   pio run -e esp32doit-devkit-v1 -t upload
   ```

3. **Monitor**: View serial output at 115200 baud:
   ```bash
   pio device monitor -b 115200
   ```

## Pinout

### Sensors & Peripherals

| Component | Pin/Address |
|-----------|-------------|
| **PMS5003/PMS7003 UART RX** | GPIO 16 |
| **PMS5003/PMS7003 UART TX** | GPIO 17 |
| **PMS Fan Control** | GPIO 26 (configurable) |
| **I²C Bus SDA** | GPIO 21 (configurable) |
| **I²C Bus SCL** | GPIO 22 (configurable) |
| **SHT30 I²C Address** | 0x44 (configurable) |
| **RTC (DS1307/DS3231)** | Uses I²C bus above |
| **SD Card CS** | GPIO 27 (configurable) |

**Note**: PMS7003 sensor is also compatible with this project.

### TFT Display & Touch (ILI9341)

Configured in `configs/TFTSetup.h`:

| Signal | Pin |
|--------|-----|
| **MISO** | GPIO 19 |
| **MOSI** | GPIO 23 |
| **SCLK** | GPIO 18 |
| **CS** | GPIO 5 |
| **DC** | GPIO 15 |
| **RST** | GPIO 2 |
| **TOUCH_CS** | GPIO 14 |

## Configurable Settings

### Hardware Pins (`include/utils/config.h`)

Most hardware pins can be changed in `include/utils/config.h`:

```cpp
namespace HardwareConfig {
    constexpr int FAN_PIN = 26;               // PMS5003/PMS7003 fan control
    constexpr int I2C_SDA_PIN = 21;           // I²C SDA (shared by SHT30 and RTC)
    constexpr int I2C_SCL_PIN = 22;           // I²C SCL (shared by SHT30 and RTC)
    constexpr int SHT30_I2C_ADDRESS = 0x44;   // SHT30 I²C address
    constexpr int SD_CARD_PIN = 27;           // SD card chip select (CS)
}
```

**Note**: 
- PMS5003/PMS7003 UART pins (RX=16, TX=17) are fixed in `include/utils/constants.h` and tied to Serial2
- RTC and SHT30 share the same I²C bus (SDA/SCL pins)

### Logging System (`include/utils/config.h`)

Control debug output via `include/utils/config.h` in the `LogConfig` namespace:

```cpp
namespace LogConfig {
    constexpr bool ENABLE_LOGGING = true;     // Master switch
    constexpr bool ENABLE_DEBUG_LOGS = true;
    constexpr bool ENABLE_INFO_LOGS = true;
    constexpr bool ENABLE_ERROR_LOGS = true;
    
    // Subsystem-specific logging
    constexpr bool ENABLE_SENSOR_LOGS = true;
    constexpr bool ENABLE_NETWORK_LOGS = true;
    constexpr bool ENABLE_DATABASE_LOGS = true;
    constexpr bool ENABLE_UI_LOGS = true;
    constexpr bool ENABLE_TASK_LOGS = true;
    constexpr bool ENABLE_SD_LOGS = true;
}
```

Use these macros in your code instead of raw `Serial.print`:

- `LOG_DEBUG(msg)`
- `LOG_INFO(msg)`
- `LOG_ERROR(msg)`
- `LOG_SENSOR(msg)`
- `LOG_NETWORK(msg)`
- `LOG_DATABASE(msg)`
- `LOG_UI(msg)`
- `LOG_TASK(msg)`
- `LOG_SD(msg)`

### Touch Calibration (`include/utils/constants.h`)

If your touchscreen needs calibration, adjust values:

```cpp
namespace Constants {
    constexpr uint16_t TOUCH_CALIBRATION[5] = {395, 3470, 320, 3470, 2};
}
```

## UI Overview

### Status LED Indicator

Both **Main Screen** and **Lock Screen** display a status LED indicator:

- **Green**: Last sensor sample was successfully saved to the SD card database
- **Red**: Save failed or sample not yet saved

Additional status icons show WiFi connection and SD card availability (red "X" overlay when disconnected/unavailable).

### Screen Navigation

- **Main Screen**: Shows current PM data, temperature and humidity
- **Lock Screen**: Minimal display with status LED and basic info
- **Settings**: Configure sampling intervals, WiFi credentials, time settings
- **WiFi Screen**: Connect to a WiFi network for NTP time sync
- **Time Settings**: Set RTC time manually or sync via NTP, configure UTC offset
- **Sampling Settings**: Configure measurement intervals, fan run time, and data storage frequency
- **Info Screen**: Display system information, network status, and IP address
- **Loading Screen**: Shown during system startup and initialization

## Known Issues

1. **RTC "not running" status**: Occasionally the RTC may report as not running even when functioning correctly. This is usually a transient I²C communication issue and resolves on next read.

2. **LED turns red after timezone offset change backwards**: When changing the UTC offset to an earlier time, the status LED may temporarily show red. This happens because the last sample timestamp appears to be "in the future" relative to the new RTC time. The LED will return to green after the next successful sample save.

3. **NTP sync failures**: NTP synchronization may occasionally fail. The system will continue operating with the current RTC time. Retry the sync or check WiFi connection if this persists.

## License

This project uses multiple open-source libraries. See `platformio.ini` for dependencies and their respective licenses.

---

**Project Structure:**
```
ESP32-Dust/
├── configs/          # LVGL and TFT_eSPI configuration templates
├── include/          # Project headers (utils/, managers/, screens/, sensors/)
├── src/              # Implementation files
├── platformio.ini    # PlatformIO build configuration
└── README.md         # This file
```
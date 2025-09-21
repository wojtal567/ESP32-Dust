#pragma once

// ============================================
// External Library Includes
// ============================================
#include <NTPClient.h>
#include <RtcDS1307.h>
#include <TFT_eSPI.h>
#include <WEMOS_SHT3X.h>
#include <WebServer.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <lvgl.h>

// ============================================
// Project Includes
// ============================================
#include "managers/rtcmanager.h"

// ============================================
// Project Includes - Legacy Components
// ============================================
#include <sensors/pms.h>
#include <MySD.hpp>

// ============================================
// Project Includes - Manager Classes
// ============================================
#include "managers/SensorManager.h"
#include "managers/displaymanager.h"
#include "managers/networkmanager.h"
#include "managers/taskmanager.h"

// ============================================
// Project Includes - Utilities
// ============================================
#include "screens/infoscreen.h"
#include "screens/lockscreen.h"
#include "screens/mainscreen.h"
#include "screens/samplingsettingsscreen.h"
#include "screens/screenmanager.h"
#include "screens/settingsscreen.h"
#include "screens/timesettingsscreen.h"
#include "screens/wifiscreen.h"
#include "utils/constants.h"
#include "utils/stringConstants.h"
// ============================================
// Network Components - Application IP Address
// ============================================
extern String appIpAddress;

// ============================================
// Storage Components (Legacy - should migrate to StorageManager)
// ============================================
// SD Card and sqlite database objects declaration
extern MySD mySDCard;
extern SQLiteDb sampleDB;

// ============================================
// Manager Instances
// ============================================
extern NetworkManager networkManager;
extern DisplayManager displayManager;
extern RTCManager rtcManager;
extern ScreenManager screenManager;
extern TaskManager taskManager;
extern MainScreen *mainScreen;
extern WifiScreen *wifiScreen;
extern InfoScreen *infoScreen;
extern SettingsScreen *settingsScreen;
extern TimeSettingsScreen *timeSettingsScreen;
extern SamplingSettingsScreen *samplingSettingsScreen;
extern LockScreen *lockScreen;

// ============================================
// Application State Variables
// ============================================
extern Types::ConfigData config;
extern String lastSampleTimestamp;

// WiFi credentials
extern String ssid;
extern String password;

// Application state flags
extern bool sdCardStatus;
extern bool isDefaultTimeOnDisplay;
extern bool lockTimeChanged;
extern bool samplingChanged;

//Temperature, relative humidity and pm2.5 per ug/m3 variables
extern float temp, humi;

// Runtime sampling state
extern int currentSampleNumber;

// Get single sample and set text
void getSampleFunc(lv_task_t *task);

bool isLastSampleSaved();
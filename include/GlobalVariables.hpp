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

// ============================================
// Project Includes - Utilities
// ============================================
#include "screens/infoscreen.h"
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
extern MainScreen *mainScreen;
extern WifiScreen *wifiScreen;
extern InfoScreen *infoScreen;
extern SettingsScreen *settingsScreen;
extern TimeSettingsScreen *timeSettingsScreen;
extern SamplingSettingsScreen *samplingSettingsScreen;

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

// ============================================
// LVGL UI Objects - Lock Screen
// ============================================
extern lv_obj_t *lockScr;
extern lv_obj_t *contDateTimeAtLock;
extern lv_obj_t *labelUnlockButton;
extern lv_obj_t *unlockButton;
extern lv_obj_t *labelDateLock;
extern lv_obj_t *labelTimeLock;
extern lv_obj_t *wifiStatusAtLock;
extern lv_obj_t *sdStatusAtLock;
extern lv_obj_t *wifiStatusAtLockWarning;
extern lv_obj_t *sdStatusAtLockWarning;
extern lv_obj_t *ledAtLock;
// ============================================
// LVGL Task Objects (Legacy - should migrate to TaskManager)
// ============================================
extern lv_task_t *turnFanOn;
extern lv_task_t *getSample;
extern lv_task_t *getAppLastRecordAndSynchronize;
extern lv_task_t *inactiveTime;
extern lv_task_t *date;
extern lv_task_t *status;

// temporarily here - will be moved to TaskManager

// Get single sample and set text
void getSampleFunc(lv_task_t *task);

bool isLastSampleSaved();

// Function that turns fan on
void turnFanOnFunc(lv_task_t *task);
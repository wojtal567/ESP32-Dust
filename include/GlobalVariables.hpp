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
// LVGL UI Objects - Sampling Settings Screen
// ============================================
extern lv_obj_t *samplingSettingsScr;
extern lv_obj_t *backSamplingSettingsBtn;
extern lv_obj_t *backSamplingSettingsLabel;
extern lv_obj_t *SamplingSettingsLabelAtBar;

extern lv_obj_t *measurePeriodlabel;

extern lv_obj_t *measurePeriodHour;
extern lv_obj_t *measurePeriodHourIncrement;
extern lv_obj_t *measurePeriodHourDecrement;

extern lv_obj_t *measureColonLabel;

extern lv_obj_t *measurePeriodMinute;
extern lv_obj_t *measurePeriodMinuteIncrement;
extern lv_obj_t *measurePeriodMinuteDecrement;

extern lv_obj_t *measureColonLabel2;

extern lv_obj_t *measurePeriodsecond;
extern lv_obj_t *measurePeriodsecondIncrement;
extern lv_obj_t *measurePeriodsecondDecrement;

extern lv_obj_t *measureNumberLabel;
extern lv_obj_t *measureNumber;
extern lv_obj_t *measureNumberIncrement;
extern lv_obj_t *measureNumberDecrement;

extern lv_obj_t *measureAvPeriodLabel;
extern lv_obj_t *measureAvPeriod;
extern lv_obj_t *measureAvPeriodIncrement;
extern lv_obj_t *measureAvPeriodDecrement;

extern lv_obj_t *turnFanOnTimeLabel;
extern lv_obj_t *turnFanOnTime;
extern lv_obj_t *turnFanOnTimeIncrement;
extern lv_obj_t *turnFanOnTimeDecrement;

extern lv_obj_t *samplingSaveBtn;
extern lv_obj_t *samplingSaveLabel;

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
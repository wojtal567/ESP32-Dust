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
#include "screens/mainscreen.h"
#include "screens/screenmanager.h"
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
extern bool inTimeSettings;
extern bool timeChanged;
extern bool dateChanged;
extern bool isDefaultTimeOnDisplay;
extern bool lockTimeChanged;
extern bool samplingChanged;

//Temperature, relative humidity and pm2.5 per ug/m3 variables
extern float temp, humi;

// Runtime sampling state
extern int currentSampleNumber;

// ============================================
// LVGL UI Objects - Info Screen
// ============================================
extern lv_obj_t *infoScr;
extern lv_obj_t *backInfoBtn;
extern lv_obj_t *backInfoLabel;
extern lv_obj_t *lcdLabelAtBar;
extern lv_obj_t *infoWifiLabel;
extern lv_obj_t *infoWifiAddressLabel;
extern lv_obj_t *configLabel;

// ============================================
// LVGL UI Objects - Settings Screen
// ============================================
extern lv_obj_t *settingsScr;
extern lv_obj_t *backSettingsBtn;
extern lv_obj_t *backSettingsLabel;
extern lv_obj_t *settingsLabelAtBar;
extern lv_obj_t *wifiBtn;
extern lv_obj_t *infoBtn;
extern lv_obj_t *timeBtn;
extern lv_obj_t *wifiBtnLabel;
extern lv_obj_t *infoBtnLabel;
extern lv_obj_t *timeBtnLabel;
extern lv_obj_t *tempBtn;
extern lv_obj_t *tempBtnLabel;
extern lv_obj_t *wifiBtnName;
extern lv_obj_t *infoBtnName;
extern lv_obj_t *timeBtnName;
extern lv_obj_t *tempBtnName;

// ============================================
// LVGL UI Objects - Time Settings Screen
// ============================================
extern lv_obj_t *timeSettingsScr;

extern lv_obj_t *backTimeSettingsBtn;
extern lv_obj_t *backTimeSettingsLabel;
extern lv_obj_t *timeSettingsLabelAtBar;

extern lv_obj_t *timeScrollPage;
extern lv_obj_t *timeLabel;
extern lv_obj_t *timeHour;
extern lv_obj_t *timeHourIncrement;
extern lv_obj_t *timeHourDecrement;

extern lv_obj_t *timeColonLabel;

extern lv_obj_t *timeMinute;
extern lv_obj_t *timeMinuteIncrement;
extern lv_obj_t *timeMinuteDecrement;

extern lv_obj_t *dateLabel;
extern lv_obj_t *dateBtn;
extern lv_obj_t *dateBtnLabel;

extern lv_obj_t *calendar;

extern lv_obj_t *lockScreenLabel;
extern lv_obj_t *lockScreenDDlist;

extern lv_obj_t *timeSettingsBtn;
extern lv_obj_t *timeSettingsLabel;
extern lv_obj_t *syncRtcBtn;
extern lv_obj_t *syncRtcLabel;
extern lv_obj_t *alertBox;
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
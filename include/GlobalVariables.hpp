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
#include "utils/constants.h"
#include "utils/stringConstants.h"

// ============================================
// External Fonts
// ============================================
//Include additional font with lock and unlock symbol
extern lv_font_t monte16lock;
extern lv_font_t hugeSymbolsFont48;

// ============================================
// Hardware Components (Legacy - should migrate to RTCManager)
// ============================================
// RTC object declaration (used by legacy rtc.hpp functions)
extern RtcDS1307<TwoWire> Rtc;

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
extern float temp, humi, pm25Aqi;

// Runtime sampling state
extern int currentSampleNumber;

// ============================================
// LVGL Styles (to be migrated to DisplayManager/UIManager)
// ============================================
//Basic container with white border and transparent background
static lv_style_t containerStyle;

//Different font sizes using lvgl styles
static lv_style_t whiteFontStyle;
static lv_style_t font12Style;
static lv_style_t font16Style;
static lv_style_t font20Style;
static lv_style_t font22Style;

//Additional styles with initalization functions
static lv_style_t warningStyle;
static lv_style_t whiteButtonStyle;
static lv_style_t lineStyle;
static lv_style_t transparentBackgroundStyle;
static lv_style_t borderlessStyle;
static lv_style_t hugeFontStyle;
// ============================================
// LVGL UI Objects - Main Screen
// ============================================
//Main screen objects declaration
extern lv_obj_t *mainScr; // LVGL Object that represents main screen
extern lv_obj_t *wifiStatusAtMain;
extern lv_obj_t *sdStatusAtMain;
extern lv_obj_t *wifiStatusAtMainWarning;
extern lv_obj_t *sdStatusAtMainWarning;
extern lv_obj_t *dateAndTimeAtBar;
extern lv_obj_t *contTemp;
extern lv_obj_t *contHumi;
extern lv_obj_t *contPM10;
extern lv_obj_t *contPM25;
extern lv_obj_t *contPM100;
extern lv_obj_t *contAQI;
extern lv_obj_t *contAQIColorBar;
extern lv_obj_t *labelLockButton;
extern lv_obj_t *lockButton;
extern lv_obj_t *labelSetButton;
extern lv_obj_t *setButton;
extern lv_obj_t *labelTemp;
extern lv_obj_t *labelHumi;
extern lv_obj_t *labelTempValue;
extern lv_obj_t *labelHumiValue;
extern lv_obj_t *labelPM10;
extern lv_obj_t *labelPM10UpperIndex;
extern lv_obj_t *labelPM25;
extern lv_obj_t *labelPM25UpperIndex;
extern lv_obj_t *labelPM100;
extern lv_obj_t *labelPM100UpperIndex;
extern lv_obj_t *labelPM10Data;
extern lv_obj_t *labelPM25Data;
extern lv_obj_t *labelPM100Data;
extern lv_obj_t *labelAQI;
extern lv_obj_t *labelAQIColorBar;
extern lv_obj_t *labelParticleSizeum[7];
extern lv_obj_t *labelParticlesNumber[6];
extern lv_obj_t *contParticlesNumber[6];
extern lv_obj_t *ledAtMain;

// LVGL UI runtime objects
extern lv_obj_t *mainLine;
extern lv_obj_t *dividingLines[7];
extern lv_color_t airQualityColors[6];

// ============================================
// LVGL UI Objects - WiFi Screen
// ============================================
extern lv_obj_t *wifiLabelAtBar;
extern lv_obj_t *wifiScr;
extern lv_obj_t *keyboard;
extern lv_obj_t *ssidTA;
extern lv_obj_t *pwdTA;
extern lv_obj_t *ssidLabel;
extern lv_obj_t *pwdLabel;
extern lv_obj_t *applyBtn;
extern lv_obj_t *applyLabel;
extern lv_obj_t *cancelBtn;
extern lv_obj_t *cancelLabel;
extern lv_obj_t *showHideBtn;
extern lv_obj_t *showHideBtnLabel;

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
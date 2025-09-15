#include "GlobalVariables.hpp"

// ============================================
// Configuration
// ============================================
Types::ConfigData config = {.ssid = "",
                            .password = "",
                            .measurePeriod = 30000,
                            .numberOfSamples = 5,
                            .timeBetweenSavingSamples = 3600000,
                            .turnFanTime = 30000,
                            .lcdLockTime = 60000};

// ============================================
// Hardware Components
// ============================================
String appIpAddress = "192.168.1.1";

// ============================================
// Storage Components
// ============================================
MySD mySDCard(Constants::SD_CARD_PIN);
SQLiteDb sampleDB("/sd/database.db", "/database.db", "samples");

// ============================================
// Manager Objects
// ============================================
NetworkManager networkManager(&mySDCard);
DisplayManager displayManager;
RTCManager rtcManager;
ScreenManager screenManager;
MainScreen *mainScreen = nullptr;
WifiScreen *wifiScreen = nullptr;
InfoScreen *infoScreen = nullptr;
SettingsScreen *settingsScreen = nullptr;
TimeSettingsScreen *timeSettingsScreen = nullptr;

// ============================================
// Application State Variables
// ============================================
String ssid;
String password;
bool sdCardStatus = false;
bool lockTimeChanged = false;
bool samplingChanged = false;
int currentSampleNumber = 1;

// Legacy variables still used in main.cpp
String lastSampleTimestamp;
float temp, humi, pm25Aqi;

// ============================================
// LVGL UI Objects - Sampling Settings Screen
// ============================================
lv_obj_t *samplingSettingsScr = nullptr;
lv_obj_t *backSamplingSettingsBtn = nullptr;
lv_obj_t *backSamplingSettingsLabel = nullptr;
lv_obj_t *SamplingSettingsLabelAtBar = nullptr;
lv_obj_t *measurePeriodlabel = nullptr;
lv_obj_t *measurePeriodHour = nullptr;
lv_obj_t *measurePeriodHourIncrement = nullptr;
lv_obj_t *measurePeriodHourDecrement = nullptr;
lv_obj_t *measureColonLabel = nullptr;
lv_obj_t *measurePeriodMinute = nullptr;
lv_obj_t *measurePeriodMinuteIncrement = nullptr;
lv_obj_t *measurePeriodMinuteDecrement = nullptr;
lv_obj_t *measureColonLabel2 = nullptr;
lv_obj_t *measurePeriodsecond = nullptr;
lv_obj_t *measurePeriodsecondIncrement = nullptr;
lv_obj_t *measurePeriodsecondDecrement = nullptr;
lv_obj_t *measureNumberLabel = nullptr;
lv_obj_t *measureNumber = nullptr;
lv_obj_t *measureNumberIncrement = nullptr;
lv_obj_t *measureNumberDecrement = nullptr;
lv_obj_t *measureAvPeriodLabel = nullptr;
lv_obj_t *measureAvPeriod = nullptr;
lv_obj_t *measureAvPeriodIncrement = nullptr;
lv_obj_t *measureAvPeriodDecrement = nullptr;
lv_obj_t *turnFanOnTimeLabel = nullptr;
lv_obj_t *turnFanOnTime = nullptr;
lv_obj_t *turnFanOnTimeIncrement = nullptr;
lv_obj_t *turnFanOnTimeDecrement = nullptr;
lv_obj_t *samplingSaveBtn = nullptr;
lv_obj_t *samplingSaveLabel = nullptr;

// ============================================
// LVGL UI Objects - Lock Screen
// ============================================
lv_obj_t *lockScr = nullptr;
lv_obj_t *contDateTimeAtLock = nullptr;
lv_obj_t *labelUnlockButton = nullptr;
lv_obj_t *unlockButton = nullptr;
lv_obj_t *labelDateLock = nullptr;
lv_obj_t *labelTimeLock = nullptr;
lv_obj_t *wifiStatusAtLock = nullptr;
lv_obj_t *sdStatusAtLock = nullptr;
lv_obj_t *wifiStatusAtLockWarning = nullptr;
lv_obj_t *sdStatusAtLockWarning = nullptr;
lv_obj_t *ledAtLock = nullptr;

// ============================================
// LVGL Task Objects (Legacy - should migrate to TaskManager)
// ============================================
lv_task_t *turnFanOn = nullptr;
lv_task_t *getSample = nullptr;
lv_task_t *getAppLastRecordAndSynchronize = nullptr;
lv_task_t *inactiveTime = nullptr;
lv_task_t *date = nullptr;
lv_task_t *status = nullptr;

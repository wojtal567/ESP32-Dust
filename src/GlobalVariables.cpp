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

// ============================================
// Application State Variables
// ============================================
String ssid;
String password;
bool sdCardStatus = false;
bool timeChanged = false;
bool dateChanged = false;
bool lockTimeChanged = false;
bool samplingChanged = false;
int currentSampleNumber = 1;

// Legacy variables still used in main.cpp
String lastSampleTimestamp;
bool inTimeSettings = false;
float temp, humi, pm25Aqi;

// ============================================
// LVGL UI Objects - WiFi Screen
// ============================================
lv_obj_t *wifiLabelAtBar = nullptr;
lv_obj_t *wifiScr = nullptr;
lv_obj_t *keyboard = nullptr;
lv_obj_t *ssidTA = nullptr;
lv_obj_t *pwdTA = nullptr;
lv_obj_t *ssidLabel = nullptr;
lv_obj_t *pwdLabel = nullptr;
lv_obj_t *applyBtn = nullptr;
lv_obj_t *applyLabel = nullptr;
lv_obj_t *cancelBtn = nullptr;
lv_obj_t *cancelLabel = nullptr;
lv_obj_t *showHideBtn = nullptr;
lv_obj_t *showHideBtnLabel = nullptr;

// ============================================
// LVGL UI Objects - Info Screen
// ============================================
lv_obj_t *infoScr = nullptr;
lv_obj_t *backInfoBtn = nullptr;
lv_obj_t *backInfoLabel = nullptr;
lv_obj_t *lcdLabelAtBar = nullptr;
lv_obj_t *infoWifiLabel = nullptr;
lv_obj_t *infoWifiAddressLabel = nullptr;
lv_obj_t *configLabel = nullptr;

// ============================================
// LVGL UI Objects - Settings Screen
// ============================================
lv_obj_t *settingsScr = nullptr;
lv_obj_t *backSettingsBtn = nullptr;
lv_obj_t *backSettingsLabel = nullptr;
lv_obj_t *settingsLabelAtBar = nullptr;
lv_obj_t *wifiBtn = nullptr;
lv_obj_t *infoBtn = nullptr;
lv_obj_t *timeBtn = nullptr;
lv_obj_t *wifiBtnLabel = nullptr;
lv_obj_t *infoBtnLabel = nullptr;
lv_obj_t *timeBtnLabel = nullptr;
lv_obj_t *tempBtn = nullptr;
lv_obj_t *tempBtnLabel = nullptr;
lv_obj_t *wifiBtnName = nullptr;
lv_obj_t *infoBtnName = nullptr;
lv_obj_t *timeBtnName = nullptr;
lv_obj_t *tempBtnName = nullptr;

// ============================================
// LVGL UI Objects - Time Settings Screen
// ============================================
lv_obj_t *timeSettingsScr = nullptr;
lv_obj_t *backTimeSettingsBtn = nullptr;
lv_obj_t *backTimeSettingsLabel = nullptr;
lv_obj_t *timeSettingsLabelAtBar = nullptr;
lv_obj_t *timeScrollPage = nullptr;
lv_obj_t *timeLabel = nullptr;
lv_obj_t *timeHour = nullptr;
lv_obj_t *timeHourIncrement = nullptr;
lv_obj_t *timeHourDecrement = nullptr;
lv_obj_t *timeColonLabel = nullptr;
lv_obj_t *timeMinute = nullptr;
lv_obj_t *timeMinuteIncrement = nullptr;
lv_obj_t *timeMinuteDecrement = nullptr;
lv_obj_t *dateLabel = nullptr;
lv_obj_t *dateBtn = nullptr;
lv_obj_t *dateBtnLabel = nullptr;
lv_obj_t *calendar = nullptr;
lv_obj_t *lockScreenLabel = nullptr;
lv_obj_t *lockScreenDDlist = nullptr;
lv_obj_t *timeSettingsBtn = nullptr;
lv_obj_t *timeSettingsLabel = nullptr;
lv_obj_t *syncRtcBtn = nullptr;
lv_obj_t *syncRtcLabel = nullptr;
lv_obj_t *alertBox = nullptr;

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

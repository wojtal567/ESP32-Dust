#include "GlobalVariables.hpp"

// ============================================
// Configuration
// ============================================
Types::ConfigData config = {
    .ssid = "",
    .password = "",
    .appIpAddress = "192.168.1.1",
    .measurePeriod = 30000,
    .numberOfSamples = 5,
    .timeBetweenSavingSamples = 3600000,
    .turnFanTime = 30000,
    .lcdLockTime = 60000,
    .fanMode = 0
};

// ============================================
// Hardware Components
// ============================================
RtcDS1307<TwoWire> Rtc(Wire);
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
bool isDefaultTimeOnDisplay = false;
float temp, humi, pm25Aqi;

// ============================================
// LVGL UI Objects - Main Screen
// ============================================
lv_obj_t *mainScr = nullptr;
lv_obj_t *wifiStatusAtMain = nullptr;
lv_obj_t *sdStatusAtMain = nullptr;
lv_obj_t *wifiStatusAtMainWarning = nullptr;
lv_obj_t *sdStatusAtMainWarning = nullptr;
lv_obj_t *dateAndTimeAtBar = nullptr;
lv_obj_t *contTemp = nullptr;
lv_obj_t *contHumi = nullptr;
lv_obj_t *contPM10 = nullptr;
lv_obj_t *contPM25 = nullptr;
lv_obj_t *contPM100 = nullptr;
lv_obj_t *contAQI = nullptr;
lv_obj_t *contAQIColorBar = nullptr;
lv_obj_t *labelLockButton = nullptr;
lv_obj_t *lockButton = nullptr;
lv_obj_t *labelSetButton = nullptr;
lv_obj_t *setButton = nullptr;
lv_obj_t *labelTemp = nullptr;
lv_obj_t *labelHumi = nullptr;
lv_obj_t *labelTempValue = nullptr;
lv_obj_t *labelHumiValue = nullptr;
lv_obj_t *labelPM10 = nullptr;
lv_obj_t *labelPM10UpperIndex = nullptr;
lv_obj_t *labelPM25 = nullptr;
lv_obj_t *labelPM25UpperIndex = nullptr;
lv_obj_t *labelPM100 = nullptr;
lv_obj_t *labelPM100UpperIndex = nullptr;
lv_obj_t *labelPM10Data = nullptr;
lv_obj_t *labelPM25Data = nullptr;
lv_obj_t *labelPM100Data = nullptr;
lv_obj_t *labelAQI = nullptr;
lv_obj_t *labelAQIColorBar = nullptr;
lv_obj_t *labelParticleSizeum[7] = {nullptr};
lv_obj_t *labelParticlesNumber[6] = {nullptr};
lv_obj_t *contParticlesNumber[6] = {nullptr};
lv_obj_t *ledAtMain = nullptr;

// ============================================
// Arrays and Static Data
// ============================================
String particlesSize[7] = {"0.0", "0.3", "0.5", "1.0", "2.5", "5.0", "10.0"};
int labelParticleSizePosX[7] = {9, 56, 103, 153, 198, 245, 288};
int contParticleNumberPosX[6] = {18, 65, 112, 159, 206, 253};
lv_point_t mainLinePoints[] = {{18, 210}, {300, 210}};
lv_point_t dividingLinesPoints[][7] = {{{18, 205} , {18, 215}}, 
                                      {{65, 205} , {65, 215}},
                                      {{112, 205}, {112, 215}},
                                      {{159, 205}, {159, 215}},
                                      {{206, 205}, {206, 215}},
                                      {{253, 205}, {253, 215}},
                                      {{300, 205}, {300, 215}}};
lv_obj_t *mainLine = nullptr;
lv_obj_t *dividingLines[7] = {nullptr};
lv_color_t airQualityColors[6] = {LV_COLOR_GREEN, LV_COLOR_GREEN, LV_COLOR_YELLOW, LV_COLOR_ORANGE, LV_COLOR_RED, LV_COLOR_RED};

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

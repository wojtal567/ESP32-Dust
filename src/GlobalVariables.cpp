#include "GlobalVariables.hpp"
#include <screens/samplingsettingsscreen.h>
#include <utils/timeUtils.h>

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
SamplingSettingsScreen *samplingSettingsScreen = nullptr;
LockScreen *lockScreen = nullptr;

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
// LVGL Task Objects (Legacy - should migrate to TaskManager)
// ============================================
lv_task_t *turnFanOn = nullptr;
lv_task_t *getSample = nullptr;
lv_task_t *getAppLastRecordAndSynchronize = nullptr;
lv_task_t *inactiveTime = nullptr;
lv_task_t *date = nullptr;
lv_task_t *status = nullptr;

void getSampleFunc(lv_task_t *task)
{
    // Static variables to accumulate samples for averaging
    static std::map<std::string, float> accumulatedData;
    static float accumulatedTemp = 0.0f;
    static float accumulatedHumi = 0.0f;

    sensorManager.readTemperatureHumiditySensor();
    if (currentSampleNumber != 0 && currentSampleNumber < config.numberOfSamples) {
        if (sensorManager.readDustSensor()) {
            Serial.println("Successfully read data from dust sensor.");
            const std::map<std::string, float> &tmpData = sensorManager.getDustData();
            for (const auto &pair : tmpData) {
                accumulatedData[pair.first] += pair.second;
            }
            currentSampleNumber++;
            accumulatedTemp += sensorManager.getTemperature();
            accumulatedHumi += sensorManager.getHumidity();
        } else {
            Serial.println("Failed to read data from dust sensor.");
        }
    }
    if (currentSampleNumber == 0) {
        lv_task_set_period(getSample, config.measurePeriod);

        if (sensorManager.readDustSensor()) {
            Serial.println("Successfully read data from dust sensor.");
            accumulatedData = sensorManager.getDustData();
            currentSampleNumber++;
            accumulatedTemp = sensorManager.getTemperature();
            accumulatedHumi = sensorManager.getHumidity();
        } else {
            Serial.println("Failed to read data from dust sensor.");
        }
    }
    if (currentSampleNumber == config.numberOfSamples) {
        // Calculate averages
        std::map<std::string, float> averagedData;
        for (const auto &pair : accumulatedData) {
            averagedData[pair.first] = pair.second / config.numberOfSamples;
        }
        currentSampleNumber = 0;
        temp = accumulatedTemp / config.numberOfSamples;
        humi = accumulatedHumi / config.numberOfSamples;

        // Reset accumulators
        accumulatedData.clear();
        accumulatedTemp = 0.0f;
        accumulatedHumi = 0.0f;

        lv_task_set_period(getSample,
                           (config.timeBetweenSavingSamples
                            - (config.numberOfSamples - 1) * config.measurePeriod));
        mainScreen->updateSensorData(temp, humi, averagedData);

        if (rtcManager.isRunning()) {
            lastSampleTimestamp = Utils::formatMainTimestamp(rtcManager.getCurrentDateTime());
            Serial.print("lastSampleTimestamp before saving to database: " + lastSampleTimestamp);
            mySDCard.save(averagedData, temp, humi, lastSampleTimestamp, &sampleDB, &Serial);
        } else {
            Serial.println("RTC is not running, not saving");
        }
        lv_task_reset(turnFanOn);
        lv_task_set_prio(turnFanOn, LV_TASK_PRIO_HIGHEST);

        sensorManager.sleepDustSensor();

        bool lastSampleSaved = isLastSampleSaved();

        mainScreen->updateLedStatus(lastSampleSaved);
        lockScreen->updateLedStatus(lastSampleSaved);
    }
}

bool isLastSampleSaved()
{
    StaticJsonDocument<600> docA;
    JsonArray lastRecordToCheck = docA.to<JsonArray>();
    mySDCard.getLastRecord(&sampleDB, &Serial, &lastRecordToCheck);
    Serial.print("Global: ");
    Serial.print(lastSampleTimestamp);
    Serial.print(" Baza: ");
    Serial.print(lastRecordToCheck[0]["timestamp"].as<String>());
    if (lastSampleTimestamp == lastRecordToCheck[0]["timestamp"].as<String>()) {
        Serial.println("Last sample has been saved correctly - return true.");
        return true;
    } else {
        Serial.println("Something went wrong saving last sample - return false");
        return false;
    }
}

// Function that turns fan on
void turnFanOnFunc(lv_task_t *task)
{
    sensorManager.wakeDustSensor();
    lv_task_set_prio(turnFanOn, LV_TASK_PRIO_OFF);
}

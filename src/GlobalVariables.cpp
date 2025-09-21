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
String appIpAddress = "";

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
TaskManager taskManager;
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
int currentSampleNumber = 0;

// Legacy variables still used in main.cpp
String lastSampleTimestamp;
float temp, humi, pm25Aqi;

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

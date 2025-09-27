#include "managers/taskmanager.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <lvgl.h>
#include <map>
#include <string>

#include "managers/networkmanager.h"
#include "managers/rtcmanager.h"
#include "managers/sensormanager.h"
#include "screens/lockscreen.h"
#include "screens/mainscreen.h"
#include "screens/screenmanager.h"
#include "utils/timeUtils.h"

TaskManager *TaskManager::s_instance = nullptr;

TaskManager::TaskManager(const Types::ConfigData &config,
                         NetworkManager *networkManager,
                         SensorManager *sensorManager,
                         MySD &sdCard,
                         RTCManager *rtcManager,
                         ScreenManager *screenManager,
                         MainScreen *mainScreen,
                         LockScreen *lockScreen)
    : m_config(config)
    , m_networkManager(networkManager)
    , m_sensorManager(sensorManager)
    , m_sdCard(sdCard)
    , m_rtcManager(rtcManager)
    , m_screenManager(screenManager)
    , m_mainScreen(mainScreen)
    , m_lockScreen(lockScreen)
{
    s_instance = this;
}

void TaskManager::initialize()
{
    m_dateTime = lv_task_create(dateTimeFuncWrapper, 800, LV_TASK_PRIO_MID, this);

    m_status = lv_task_create(statusFuncWrapper, 5000, LV_TASK_PRIO_LOW, this);

    uint32_t getSamplePeriod = (m_config.timeBetweenSavingSamples
                                - (m_config.numberOfSamples - 1) * m_config.measurePeriod);
    m_getSample = lv_task_create(getSampleFuncWrapper, getSamplePeriod, LV_TASK_PRIO_HIGH, this);

    uint32_t turnFanOnPeriod = getSamplePeriod - m_config.turnFanTime;
    m_turnFanOn = lv_task_create(turnFanOnFuncWrapper, turnFanOnPeriod, LV_TASK_PRIO_HIGHEST, this);

    m_inactiveTime = lv_task_create(inactiveScreenFuncWrapper, 1, LV_TASK_PRIO_HIGH, this);

    m_getAppLastRecordAndSynchronize = lv_task_create(fetchLastRecordAndSynchronizeWrapper,
                                                      30000,
                                                      LV_TASK_PRIO_MID,
                                                      this);
}

void TaskManager::recreateSampleTasksFromConfig()
{
    // delete old tasks if they exist
    if (m_getSample) {
        lv_task_del(m_getSample);
        m_getSample = nullptr;
    }
    if (m_turnFanOn) {
        lv_task_del(m_turnFanOn);
        m_turnFanOn = nullptr;
    }

    // create new tasks with updated config
    uint32_t getSamplePeriod = (m_config.timeBetweenSavingSamples
                                - (m_config.numberOfSamples - 1) * m_config.measurePeriod);
    m_getSample = lv_task_create(getSampleFuncWrapper, getSamplePeriod, LV_TASK_PRIO_HIGH, this);

    uint32_t turnFanOnPeriod = getSamplePeriod - m_config.turnFanTime;
    m_turnFanOn = lv_task_create(turnFanOnFuncWrapper, turnFanOnPeriod, LV_TASK_PRIO_HIGHEST, this);
}

void TaskManager::getSampleFuncWrapper(lv_task_t *task)
{
    if (task && task->user_data) {
        static_cast<TaskManager *>(task->user_data)->getSampleFunc(task);
    }
}

void TaskManager::turnFanOnFuncWrapper(lv_task_t *task)
{
    if (task && task->user_data) {
        static_cast<TaskManager *>(task->user_data)->turnFanOnFunc(task);
    }
}

void TaskManager::dateTimeFuncWrapper(lv_task_t *task)
{
    if (task && task->user_data) {
        static_cast<TaskManager *>(task->user_data)->dateTimeFunc(task);
    }
}

void TaskManager::statusFuncWrapper(lv_task_t *task)
{
    if (task && task->user_data) {
        static_cast<TaskManager *>(task->user_data)->statusFunc(task);
    }
}

void TaskManager::fetchLastRecordAndSynchronizeWrapper(lv_task_t *task)
{
    if (task && task->user_data) {
        static_cast<TaskManager *>(task->user_data)->fetchLastRecordAndSynchronize(task);
    }
}

void TaskManager::inactiveScreenFuncWrapper(lv_task_t *task)
{
    if (task && task->user_data) {
        static_cast<TaskManager *>(task->user_data)->inactiveScreenFunc(task);
    }
}

void TaskManager::getSampleFunc(lv_task_t *task)
{
    static std::map<std::string, float> accumulatedData;
    static float accumulatedTemp = 0.0f;
    static float accumulatedHumi = 0.0f;
    static int currentSampleNumber = 0;

    m_sensorManager->readTemperatureHumiditySensor();
    if (currentSampleNumber != 0 && currentSampleNumber < m_config.numberOfSamples) {
        if (m_sensorManager->readDustSensor()) {
            Serial.println("Successfully read data from dust sensor.");
            const std::map<std::string, float> &tmpData = m_sensorManager->getDustData();
            for (const auto &pair : tmpData) {
                accumulatedData[pair.first] += pair.second;
            }
            currentSampleNumber++;
            accumulatedTemp += m_sensorManager->getTemperature();
            accumulatedHumi += m_sensorManager->getHumidity();
        } else {
            Serial.println("Failed to read data from dust sensor.");
        }
    }
    if (currentSampleNumber == 0) {
        lv_task_set_period(m_getSample, m_config.measurePeriod);
        if (m_sensorManager->readDustSensor()) {
            Serial.println("Successfully read data from dust sensor.");
            accumulatedData = m_sensorManager->getDustData();
            currentSampleNumber++;
            accumulatedTemp = m_sensorManager->getTemperature();
            accumulatedHumi = m_sensorManager->getHumidity();
        } else {
            Serial.println("Failed to read data from dust sensor.");
        }
    }
    if (currentSampleNumber == m_config.numberOfSamples) {
        Serial.println("TaskManager: All samples collected (" + String(m_config.numberOfSamples)
                       + "), calculating averages...");

        // calculate averages
        std::map<std::string, float> averagedData;
        for (const auto &pair : accumulatedData) {
            averagedData[pair.first] = pair.second / m_config.numberOfSamples;
        }
        currentSampleNumber = 0;
        float temp = accumulatedTemp / m_config.numberOfSamples;
        float humi = accumulatedHumi / m_config.numberOfSamples;

        // reset accumulators
        accumulatedData.clear();
        accumulatedTemp = 0.0f;
        accumulatedHumi = 0.0f;

        uint32_t newPeriod = (m_config.timeBetweenSavingSamples
                              - (m_config.numberOfSamples - 1) * m_config.measurePeriod);
        lv_task_set_period(m_getSample, newPeriod);
        m_mainScreen->updateSensorData(temp, humi, averagedData);

        if (m_rtcManager->isRunning()) {
            m_lastSampleTimestamp = Utils::formatMainTimestamp(m_rtcManager->getCurrentDateTime());
            Serial.println("lastSampleTimestamp before saving to database: "
                           + m_lastSampleTimestamp);
            m_sdCard.save(averagedData, temp, humi, m_lastSampleTimestamp, &Serial);
        } else {
            Serial.println("RTC is not running, not saving");
        }
        lv_task_reset(m_turnFanOn);
        lv_task_set_prio(m_turnFanOn, LV_TASK_PRIO_HIGHEST);

        m_sensorManager->sleepDustSensor();

        bool lastSampleSaved = isLastSampleSaved();

        m_mainScreen->updateLedStatus(lastSampleSaved);
        m_lockScreen->updateLedStatus(lastSampleSaved);
    }
}

void TaskManager::turnFanOnFunc(lv_task_t *task)
{
    m_sensorManager->wakeDustSensor();
    lv_task_set_prio(m_turnFanOn, LV_TASK_PRIO_OFF);
}

void TaskManager::dateTimeFunc(lv_task_t *task)
{
    if (m_rtcManager->isRunning()) {
        m_mainScreen->updateDateTimeLabel(
            Utils::formatMainTimestamp(m_rtcManager->getCurrentDateTime()).c_str());
        m_lockScreen->updateDateTime(m_rtcManager->getDate().c_str(),
                                     m_rtcManager->getTime().c_str());
    } else {
        m_mainScreen->updateDateTimeLabel("\0");
        m_lockScreen->updateDateTime("", "");
    }
}

void TaskManager::statusFunc(lv_task_t *task)
{
    const bool isNetworkConnected = m_networkManager->isConnected();
    m_mainScreen->updateWiFiStatus(isNetworkConnected);

    const bool isSDCardConnected = m_sdCard.start(&Serial2);

    if (isSDCardConnected) {
        if (!m_networkManager->isConnected() && (m_config.ssid != "" && m_config.password != "")) {
            m_networkManager->connectAsync(m_config.ssid.c_str(), m_config.password.c_str());
        }
    }

    m_mainScreen->updateSDStatus(isSDCardConnected);
    m_lockScreen->updateWifiSdStatus(isNetworkConnected, isSDCardConnected);
}

void TaskManager::fetchLastRecordAndSynchronize(lv_task_t *task)
{
    if (WiFi.status() == WL_CONNECTED && m_appIpAddress != "") {
        Serial.println("Starting synchronizing process...");
        HTTPClient getHttp;
        String url = "http://" + m_appIpAddress + "/fetch/last";
        Serial.println(url);
        if (getHttp.begin(url.c_str())) {
            Serial.println(getHttp.getString());
            uint8_t responseCode = getHttp.GET();

            if (responseCode == 200) {
                Serial.println("GET /last succesful.");
                Serial.println("HTTP RESPONSE CODE: " + (String)responseCode);
                StaticJsonDocument<600> response, doc1;

                DeserializationError err = deserializeJson(response, getHttp.getString());
                Serial.println("Deserialization error: " + (String)err.c_str());
                JsonArray lastRecord = doc1.to<JsonArray>();

                m_sdCard.getLastRecord(&Serial, &lastRecord);
                DynamicJsonDocument doc(33000);
                if ((response[0]["timestamp"].as<String>()
                     != lastRecord[0]["timestamp"].as<String>())
                    || (response[0]["timestamp"].as<String>() == "null")) {
                    Serial.println("Got last record that looks good. Parsing and sending data to "
                                   "Server App...");
                    JsonArray records = doc.to<JsonArray>();
                    m_sdCard.select(&Serial, response[0]["timestamp"].as<String>(), &records);
                    String json = "";

                    serializeJson(doc, json);
                    getHttp.begin("http://" + m_appIpAddress + "/submit");
                    getHttp.addHeader("Content-Type", "application/json");
                    getHttp.POST(json);
                    Serial.println("POST RESPONSE:" + getHttp.getString());
                    getHttp.end();
                }
            } else {
                Serial.println("ERROR FETCHING DATA. CODE: " + (String)responseCode);
                lv_task_set_prio(m_getAppLastRecordAndSynchronize, LV_TASK_PRIO_OFF);
            }
        } else {
            Serial.println("Wrong url");
            lv_task_set_prio(m_getAppLastRecordAndSynchronize, LV_TASK_PRIO_OFF);
        }
        getHttp.end();
    }
}

void TaskManager::inactiveScreenFunc(lv_task_t *task)
{
    if (m_config.lcdLockTime != -1) {
        if (lv_disp_get_inactive_time(NULL) > m_config.lcdLockTime) {
            if (m_screenManager->getCurrentScreen() != m_lockScreen) {
                m_screenManager->switchToScreen(ScreenType::LOCK);
            }
        }
    }
}

void TaskManager::updateGetAppLastRecordAndSynchronizeTaskPrio(lv_task_prio_t prio)
{
    if (m_getAppLastRecordAndSynchronize) {
        lv_task_set_prio(m_getAppLastRecordAndSynchronize, prio);
    }
}

bool TaskManager::isLastSampleSaved() const
{
    StaticJsonDocument<600> docA;
    JsonArray lastRecordToCheck = docA.to<JsonArray>();
    m_sdCard.getLastRecord(&Serial, &lastRecordToCheck);
    Serial.print("Global: ");
    Serial.print(m_lastSampleTimestamp);
    Serial.print(" Baza: ");
    Serial.print(lastRecordToCheck[0]["timestamp"].as<String>());
    if (m_lastSampleTimestamp == lastRecordToCheck[0]["timestamp"].as<String>()) {
        Serial.println("Last sample has been saved correctly - return true.");
        return true;
    } else {
        Serial.println("Something went wrong saving last sample - return false");
        return false;
    }
}

void TaskManager::setAppIpAddress(const String &ipAddress)
{
    m_appIpAddress = ipAddress;
}
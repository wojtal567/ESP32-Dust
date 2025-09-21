#include "managers/taskmanager.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <lvgl.h>
#include <map>
#include <string>
#include "GlobalVariables.hpp"
#include "utils/timeUtils.h"

TaskManager *TaskManager::s_instance = nullptr;

TaskManager::TaskManager()
{
    s_instance = this;
}

void TaskManager::initialize()
{
    m_dateTime = lv_task_create(dateTimeFuncWrapper, 800, LV_TASK_PRIO_MID, this);

    m_status = lv_task_create(statusFuncWrapper, 5000, LV_TASK_PRIO_LOW, this);

    uint32_t getSamplePeriod = (config.timeBetweenSavingSamples
                                - (config.numberOfSamples - 1) * config.measurePeriod);
    m_getSample = lv_task_create(getSampleFuncWrapper, getSamplePeriod, LV_TASK_PRIO_HIGH, this);

    uint32_t turnFanOnPeriod = getSamplePeriod - config.turnFanTime;
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
    uint32_t getSamplePeriod = (config.timeBetweenSavingSamples
                                - (config.numberOfSamples - 1) * config.measurePeriod);
    m_getSample = lv_task_create(getSampleFuncWrapper, getSamplePeriod, LV_TASK_PRIO_HIGH, this);

    uint32_t turnFanOnPeriod = getSamplePeriod - config.turnFanTime;
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
        Serial.println("TaskManager: First sample - setting period to measurePeriod: "
                       + String(config.measurePeriod) + "ms");
        lv_task_set_period(m_getSample, config.measurePeriod);
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
        Serial.println("TaskManager: All samples collected (" + String(config.numberOfSamples)
                       + "), calculating averages...");

        // calculate averages
        std::map<std::string, float> averagedData;
        for (const auto &pair : accumulatedData) {
            averagedData[pair.first] = pair.second / config.numberOfSamples;
        }
        currentSampleNumber = 0;
        temp = accumulatedTemp / config.numberOfSamples;
        humi = accumulatedHumi / config.numberOfSamples;

        // reset accumulators
        accumulatedData.clear();
        accumulatedTemp = 0.0f;
        accumulatedHumi = 0.0f;

        uint32_t newPeriod = (config.timeBetweenSavingSamples
                              - (config.numberOfSamples - 1) * config.measurePeriod);
        Serial.println("TaskManager: Resetting getSample period to: " + String(newPeriod) + "ms");
        lv_task_set_period(m_getSample, newPeriod);
        mainScreen->updateSensorData(temp, humi, averagedData);

        if (rtcManager.isRunning()) {
            lastSampleTimestamp = Utils::formatMainTimestamp(rtcManager.getCurrentDateTime());
            Serial.println("lastSampleTimestamp before saving to database: " + lastSampleTimestamp);
            mySDCard.save(averagedData, temp, humi, lastSampleTimestamp, &sampleDB, &Serial);
        } else {
            Serial.println("RTC is not running, not saving");
        }
        lv_task_reset(m_turnFanOn);
        lv_task_set_prio(m_turnFanOn, LV_TASK_PRIO_HIGHEST);

        sensorManager.sleepDustSensor();

        bool lastSampleSaved = isLastSampleSaved();

        mainScreen->updateLedStatus(lastSampleSaved);
        lockScreen->updateLedStatus(lastSampleSaved);
    }
}

void TaskManager::turnFanOnFunc(lv_task_t *task)
{
    sensorManager.wakeDustSensor();
    lv_task_set_prio(m_turnFanOn, LV_TASK_PRIO_OFF);
}

void TaskManager::dateTimeFunc(lv_task_t *task)
{
    if (rtcManager.isRunning()) {
        mainScreen->updateDateTimeLabel(
            Utils::formatMainTimestamp(rtcManager.getCurrentDateTime()).c_str());
        lockScreen->updateDateTime(rtcManager.getDate().c_str(), rtcManager.getTime().c_str());
    } else {
        mainScreen->updateDateTimeLabel("\0");
        lockScreen->updateDateTime("", "");
    }
}

void TaskManager::statusFunc(lv_task_t *task)
{
    const bool isNetworkConnected = networkManager.isConnected();
    mainScreen->updateWiFiStatus(isNetworkConnected);

    const bool isSDCardConnected = mySDCard.start(&sampleDB, &Serial2);

    if (isSDCardConnected) {
        if (!networkManager.isConnected() && (config.ssid != "" && config.password != "")) {
            networkManager.connectAsync(config.ssid.c_str(), config.password.c_str());
        }
    }

    mainScreen->updateSDStatus(isSDCardConnected);
    lockScreen->updateWifiSdStatus(isNetworkConnected, isSDCardConnected);
}

void TaskManager::fetchLastRecordAndSynchronize(lv_task_t *task)
{
    if (WiFi.status() == WL_CONNECTED && appIpAddress != "") {
        Serial.println("Starting synchronizing process...");
        HTTPClient getHttp;
        String url = "http://" + appIpAddress + "/fetch/last";
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

                mySDCard.getLastRecord(&sampleDB, &Serial, &lastRecord);
                DynamicJsonDocument doc(33000);
                if ((response[0]["timestamp"].as<String>()
                     != lastRecord[0]["timestamp"].as<String>())
                    || (response[0]["timestamp"].as<String>() == "null")) {
                    Serial.println("Got last record that looks good. Parsing and sending data to "
                                   "Server App...");
                    JsonArray records = doc.to<JsonArray>();
                    mySDCard.select(&sampleDB,
                                    &Serial,
                                    response[0]["timestamp"].as<String>(),
                                    &records);
                    String json = "";

                    serializeJson(doc, json);
                    getHttp.begin("http://" + appIpAddress + "/submit");
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
    if (config.lcdLockTime != -1) {
        if (lv_disp_get_inactive_time(NULL) > config.lcdLockTime) {
            if (screenManager.getCurrentScreen() != lockScreen) {
                screenManager.switchToScreen(BaseScreen::ScreenType::LOCK);
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
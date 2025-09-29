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

    m_sensorUIQueue = xQueueCreate(5, sizeof(SensorUIUpdateMessage));
    if (m_sensorUIQueue == nullptr) {
        Serial.println("Failed to create sensor UI queue.");
    }

    m_statusQueue = xQueueCreate(5, sizeof(StatusUpdateMessage));
    if (m_statusQueue == nullptr) {
        Serial.println("Failed to create status queue.");
    }
}

void TaskManager::initialize()
{
    m_dateTime = lv_task_create(dateTimeFuncWrapper, 100, LV_TASK_PRIO_HIGH, this);

    uint32_t getSamplePeriod = (m_config.timeBetweenSavingSamples
                                - (m_config.numberOfSamples - 1) * m_config.measurePeriod);
    uint32_t turnFanOnPeriod = getSamplePeriod - m_config.turnFanTime;

    m_turnFanOn = lv_task_create(turnFanOnFuncWrapper, turnFanOnPeriod, LV_TASK_PRIO_HIGHEST, this);

    m_inactiveTime = lv_task_create(inactiveScreenFuncWrapper, 1, LV_TASK_PRIO_HIGH, this);

    m_getAppLastRecordAndSynchronize = lv_task_create(fetchLastRecordAndSynchronizeWrapper,
                                                      30000,
                                                      LV_TASK_PRIO_MID,
                                                      this);

    m_sensorUIProcessor = lv_task_create(sensorUIUpdateWrapper, 100, LV_TASK_PRIO_MID, this);

    BaseType_t sensorTaskResult
        = xTaskCreate(sensorDataCollectionTask, "SensorTask", 8192, this, 2, &m_sensorTaskHandle);

    if (sensorTaskResult != pdPASS) {
        Serial.println("Failed to create sensor data collection task.");
    }

    m_statusProcessor = lv_task_create(statusUIUpdateWrapper, 1000, LV_TASK_PRIO_LOW, this);

    BaseType_t statusTaskResult
        = xTaskCreate(statusDataCollectionTask, "StatusTask", 8192, this, 1, &m_statusTaskHandle);

    if (statusTaskResult != pdPASS) {
        Serial.println("Failed to create status data collection task.");
    }
}

void TaskManager::recreateSampleTasksFromConfig()
{
    if (m_sensorTaskHandle != nullptr) {
        vTaskDelete(m_sensorTaskHandle);
        m_sensorTaskHandle = nullptr;
    }

    BaseType_t sensorTaskResult
        = xTaskCreate(sensorDataCollectionTask, "SensorTask", 8192, this, 2, &m_sensorTaskHandle);

    if (sensorTaskResult != pdPASS) {
        Serial.println("Failed to create sensor data collection task.");
    }

    if (m_turnFanOn) {
        lv_task_del(m_turnFanOn);
        m_turnFanOn = nullptr;
    }

    // create new tasks with updated config
    uint32_t getSamplePeriod = (m_config.timeBetweenSavingSamples
                                - (m_config.numberOfSamples - 1) * m_config.measurePeriod);

    uint32_t turnFanOnPeriod = getSamplePeriod - m_config.turnFanTime;
    m_turnFanOn = lv_task_create(turnFanOnFuncWrapper, turnFanOnPeriod, LV_TASK_PRIO_HIGHEST, this);
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

void TaskManager::sensorDataCollectionTask(void *parameters)
{
    TaskManager *taskManager = static_cast<TaskManager *>(parameters);

    uint32_t fullCyclePeriod = (taskManager->m_config.timeBetweenSavingSamples
                                - (taskManager->m_config.numberOfSamples - 1)
                                      * taskManager->m_config.measurePeriod);
    vTaskDelay(pdMS_TO_TICKS(fullCyclePeriod));

    TickType_t xLastWakeTime = xTaskGetTickCount();
    std::map<std::string, float> accumulatedData;
    float accumulatedTemp = 0.0f;
    float accumulatedHumi = 0.0f;
    int currentSampleNumber = 0;

    uint32_t measurePeriod = taskManager->m_config.measurePeriod;

    while (true) {
        Serial.printf("Sensor task: Reading sample %d/%d\n",
                      currentSampleNumber + 1,
                      taskManager->m_config.numberOfSamples);

        taskManager->m_sensorManager->readTemperatureHumiditySensor();
        // Read dust sensor and accumulate data
        if (currentSampleNumber == 0) {
            if (taskManager->m_sensorManager->readDustSensor()) {
                Serial.println("Successfully read data from dust sensor.");
                accumulatedData = taskManager->m_sensorManager->getDustData();
                accumulatedTemp = taskManager->m_sensorManager->getTemperature();
                accumulatedHumi = taskManager->m_sensorManager->getHumidity();
                currentSampleNumber++;
            } else {
                Serial.println("Failed to read data from dust sensor.");
            }
        } else if (currentSampleNumber < taskManager->m_config.numberOfSamples) {
            if (taskManager->m_sensorManager->readDustSensor()) {
                Serial.println("Successfully read data from dust sensor.");
                const std::map<std::string, float> &tmpData = taskManager->m_sensorManager
                                                                  ->getDustData();
                for (const auto &pair : tmpData) {
                    accumulatedData[pair.first] += pair.second;
                }
                accumulatedTemp += taskManager->m_sensorManager->getTemperature();
                accumulatedHumi += taskManager->m_sensorManager->getHumidity();
                currentSampleNumber++;
            } else {
                Serial.println("Failed to read data from dust sensor.");
            }
        }

        // Check if we have all samples
        if (currentSampleNumber == taskManager->m_config.numberOfSamples) {
            Serial.println("TaskManager: All samples collected ("
                           + String(taskManager->m_config.numberOfSamples)
                           + "), calculating averages...");

            // calculate averages
            std::map<std::string, float> averagedData;
            for (const auto &pair : accumulatedData) {
                averagedData[pair.first] = pair.second / taskManager->m_config.numberOfSamples;
            }
            currentSampleNumber = 0;
            float temp = accumulatedTemp / taskManager->m_config.numberOfSamples;
            float humi = accumulatedHumi / taskManager->m_config.numberOfSamples;

            // reset accumulators
            accumulatedData.clear();
            accumulatedTemp = 0.0f;
            accumulatedHumi = 0.0f;

            bool saveSuccess = false;
            if (taskManager->m_rtcManager->isRunning()) {
                taskManager->m_lastSampleTimestamp = Utils::formatMainTimestamp(
                    taskManager->m_rtcManager->getCurrentDateTime());
                Serial.println("lastSampleTimestamp before saving to database: "
                               + taskManager->m_lastSampleTimestamp);
                taskManager->m_sdCard.save(averagedData,
                                           temp,
                                           humi,
                                           taskManager->m_lastSampleTimestamp,
                                           &Serial);
                saveSuccess = taskManager->isLastSampleSaved();
            } else {
                Serial.println("RTC is not running, not saving");
            }

            // Send UI update message
            SensorUIUpdateMessage msg;
            msg.temperature = temp;
            msg.humidity = humi;
            msg.averagedData = averagedData;
            msg.isLastSampleSaved = saveSuccess;

            if (xQueueSend(taskManager->m_sensorUIQueue, &msg, 0) != pdTRUE) {
                Serial.println("Sensor task: Failed to send UI update");
            }

            // Turn off fan after all samples collected
            taskManager->m_sensorManager->sleepDustSensor();
            lv_task_set_prio(taskManager->m_turnFanOn, LV_TASK_PRIO_HIGHEST);

            // Wait for the full period before next cycle
            uint32_t fullCyclePeriod = taskManager->m_config.timeBetweenSavingSamples
                                       - (taskManager->m_config.numberOfSamples - 1)
                                             * taskManager->m_config.measurePeriod;
            vTaskDelay(pdMS_TO_TICKS(fullCyclePeriod));
            xLastWakeTime = xTaskGetTickCount(); // Reset timing base
        } else {
            // Wait for next measurement in the averaging cycle
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(measurePeriod));
        }
    }
}

void TaskManager::sensorUIUpdateWrapper(lv_task_t *task)
{
    if (task && task->user_data) {
        static_cast<TaskManager *>(task->user_data)->processSensorUIUpdates();
    }
}

void TaskManager::processSensorUIUpdates()
{
    if (m_sensorUIQueue == nullptr) {
        return;
    }

    SensorUIUpdateMessage msg;
    while (xQueueReceive(m_sensorUIQueue, &msg, 0) == pdTRUE) {
        // Update the main screen with new sensor data
        m_mainScreen->updateSensorData(msg.temperature, msg.humidity, msg.averagedData);

        // Update LED status based on whether the last sample was saved successfully
        m_mainScreen->updateLedStatus(msg.isLastSampleSaved);
        m_lockScreen->updateLedStatus(msg.isLastSampleSaved);
    }
}

void TaskManager::statusDataCollectionTask(void *parameters)
{
    TaskManager *taskManager = static_cast<TaskManager *>(parameters);

    const TickType_t xDelay = pdMS_TO_TICKS(5000);
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (true) {
        StatusUpdateMessage msg;
        msg.wifiConnected = taskManager->m_networkManager->isConnected();
        msg.sdCardConnected = taskManager->m_sdCard.start(&Serial2);

        if (xQueueSend(taskManager->m_statusQueue, &msg, 0) != pdTRUE) {
            Serial.println("Status task: Failed to send UI update");
        }

        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}

void TaskManager::statusUIUpdateWrapper(lv_task_t *task)
{
    if (task && task->user_data) {
        static_cast<TaskManager *>(task->user_data)->processStatusUIUpdates();
    }
}

void TaskManager::processStatusUIUpdates()
{
    if (m_statusQueue == nullptr) {
        return;
    }

    StatusUpdateMessage msg;
    while (xQueueReceive(m_statusQueue, &msg, 0) == pdTRUE) {
        // Update the screens with new status data
        m_mainScreen->updateWiFiStatus(msg.wifiConnected);
        m_mainScreen->updateSDStatus(msg.sdCardConnected);
        m_lockScreen->updateWifiSdStatus(msg.wifiConnected, msg.sdCardConnected);
    }
}
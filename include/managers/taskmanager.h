#pragma once

#include <lvgl.h>

#include "utils/types.h"

class NetworkManager;
class SensorManager;
class MySD;
class MainScreen;
class LockScreen;
class ScreenManager;
class RTCManager;
struct SensorUIUpdateMessage
{
    float temperature;
    float humidity;
    std::map<std::string, float> *averagedData;
    bool isLastSampleSaved;
};

struct StatusUpdateMessage
{
    bool wifiConnected;
    bool sdCardConnected;
};

class TaskManager
{
public:
    TaskManager(const Types::ConfigData &config,
                NetworkManager *networkManager,
                SensorManager *sensorManager,
                MySD &sdCard,
                RTCManager *rtcManager,
                ScreenManager *screenManager,
                MainScreen *mainScreen,
                LockScreen *lockScreen);
    void initialize();
    void recreateSampleTasksFromConfig();
    void updateGetAppLastRecordAndSynchronizeTaskPrio(lv_task_prio_t prio);
    void setAppIpAddress(const String &ipAddress);

private:
    void turnFanOnFunc(lv_task_t *task);
    void dateTimeFunc(lv_task_t *task);
    void fetchLastRecordAndSynchronize(lv_task_t *task);
    void inactiveScreenFunc(lv_task_t *task);
    bool isLastSampleSaved() const;

    // Static wrappers for LVGL
    static void turnFanOnFuncWrapper(lv_task_t *task);
    static void dateTimeFuncWrapper(lv_task_t *task);
    static void fetchLastRecordAndSynchronizeWrapper(lv_task_t *task);
    static void inactiveScreenFuncWrapper(lv_task_t *task);

    static void sensorUIUpdateWrapper(lv_task_t *task);
    void processSensorUIUpdates();
    static void sensorDataCollectionTask(void *parameters);

    static void statusUIUpdateWrapper(lv_task_t *task);
    void processStatusUIUpdates();
    static void statusDataCollectionTask(void *parameters);

    TaskHandle_t m_sensorTaskHandle = nullptr, m_statusTaskHandle = nullptr;
    QueueHandle_t m_sensorUIQueue = nullptr, m_statusQueue = nullptr;
    lv_task_t *m_sensorUIProcessor = nullptr, *m_statusProcessor = nullptr;

    Types::ConfigData m_config;
    NetworkManager *m_networkManager;
    SensorManager *m_sensorManager;
    MySD &m_sdCard;
    RTCManager *m_rtcManager;
    ScreenManager *m_screenManager;
    MainScreen *m_mainScreen;
    LockScreen *m_lockScreen;

    lv_task_t *m_turnFanOn = nullptr;
    lv_task_t *m_dateTime = nullptr;
    lv_task_t *m_getAppLastRecordAndSynchronize = nullptr;
    lv_task_t *m_inactiveTime = nullptr;
    String m_appIpAddress{""};
    String m_lastSampleTimestamp{""};

    static TaskManager *s_instance;
};
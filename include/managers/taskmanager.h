#pragma once

#include <lvgl.h>

class TaskManager
{
public:
    TaskManager();
    void initialize();
    void recreateSampleTasksFromConfig();
    void updateGetAppLastRecordAndSynchronizeTaskPrio(lv_task_prio_t prio);

private:
    lv_task_t *m_getSample = nullptr;
    lv_task_t *m_turnFanOn = nullptr;
    lv_task_t *m_dateTime = nullptr;
    lv_task_t *m_status = nullptr;
    lv_task_t *m_getAppLastRecordAndSynchronize = nullptr;
    lv_task_t *m_inactiveTime = nullptr;

    void getSampleFunc(lv_task_t *task);
    void turnFanOnFunc(lv_task_t *task);
    void dateTimeFunc(lv_task_t *task);
    void statusFunc(lv_task_t *task);
    void fetchLastRecordAndSynchronize(lv_task_t *task);
    void inactiveScreenFunc(lv_task_t *task);

    // Static wrappers for LVGL
    static void getSampleFuncWrapper(lv_task_t *task);
    static void turnFanOnFuncWrapper(lv_task_t *task);
    static void dateTimeFuncWrapper(lv_task_t *task);
    static void statusFuncWrapper(lv_task_t *task);
    static void fetchLastRecordAndSynchronizeWrapper(lv_task_t *task);
    static void inactiveScreenFuncWrapper(lv_task_t *task);

    static TaskManager *s_instance;
};
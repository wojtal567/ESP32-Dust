#pragma once

#include "screens/basescreen.h"

#include "utils/types.h"
#include "managers/rtcmanager.h"

class TimeSettingsScreen : public BaseScreen
{
public:
    TimeSettingsScreen(const Types::ConfigData& config, const RTCManager& rtc);
    ~TimeSettingsScreen() override;

    void initialize() override;

    void updateData();

private:
    int getDropdownIndex() const;

    void handleHourIncrement(lv_obj_t *btn, lv_event_t event);
    void handleHourDecrement(lv_obj_t *btn, lv_event_t event);
    void handleMinuteIncrement(lv_obj_t *btn, lv_event_t event);
    void handleMinuteDecrement(lv_obj_t *btn, lv_event_t event);
    void handleDateButton(lv_obj_t *btn, lv_event_t event);
    void handleCalendarEvent(lv_obj_t *calendar, lv_event_t event);
    void handleSyncNtpButton(lv_obj_t *btn, lv_event_t event);
    void handleSaveButton(lv_obj_t *btn, lv_event_t event);

    // Static wrappers for LVGL callbacks
    static void hourIncrementCallback(lv_obj_t *btn, lv_event_t event);
    static void hourDecrementCallback(lv_obj_t *btn, lv_event_t event);
    static void minuteIncrementCallback(lv_obj_t *btn, lv_event_t event);
    static void minuteDecrementCallback(lv_obj_t *btn, lv_event_t event);
    static void dateButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void calendarEventCallback(lv_obj_t *calendar, lv_event_t event);
    static void syncNtpButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void saveButtonCallback(lv_obj_t *btn, lv_event_t event);

    // Instance tracking for callbacks
    static TimeSettingsScreen* s_activeInstance;

    lv_obj_t *m_backButton{nullptr};
    lv_obj_t *m_backButtonLabel{nullptr};
    
    lv_obj_t *m_titleLabel{nullptr};
    
    lv_obj_t *m_timeLabel{nullptr};
    lv_obj_t *m_hourSpinbox{nullptr};
    lv_obj_t *m_minuteSpinbox{nullptr};
    lv_obj_t *m_hourIncrementButton{nullptr};
    lv_obj_t *m_hourDecrementButton{nullptr};
    lv_obj_t *m_minuteIncrementButton{nullptr};
    lv_obj_t *m_minuteDecrementButton{nullptr};
    lv_obj_t *m_colonLabel{nullptr};
    
    lv_obj_t *m_dateLabel{nullptr};
    lv_obj_t *m_dateButton{nullptr};
    lv_obj_t *m_dateButtonLabel{nullptr};
    lv_obj_t *m_calendar{nullptr};
    
    lv_obj_t *m_lockScreenLabel{nullptr};
    lv_obj_t *m_lockScreenDropdown{nullptr};

    lv_obj_t *m_syncNtpButton{nullptr};
    lv_obj_t *m_syncNtpLabel{nullptr};

    lv_obj_t *m_saveButton{nullptr};
    lv_obj_t *m_saveButtonLabel{nullptr};

    Types::ConfigData m_configData;
    RTCManager m_rtcManager;

    bool m_timeChanged{false}, m_dateChanged{false};
};
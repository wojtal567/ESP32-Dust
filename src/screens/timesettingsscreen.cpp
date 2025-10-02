#include "screens/timesettingsscreen.h"

#include "managers/networkmanager.h"
#include "managers/rtcmanager.h"
#include "managers/screenmanager.h"
#include "managers/stylemanager.h"

#include "utils/config.h"
#include "utils/constants.h"
#include "utils/stringConstants.h"

TimeSettingsScreen::TimeSettingsScreen(Types::ConfigData &config,
                                       RTCManager *rtc,
                                       NetworkManager *networkManager,
                                       ScreenManager *screenManager,
                                       MySD &sdCard)
    : BaseScreen<TimeSettingsScreen>(ScreenType::TIME_SETTINGS)
    , m_config(config)
    , m_rtcManager(rtc)
    , m_networkManager(networkManager)
    , m_screenManager(screenManager)
    , m_sdCard(sdCard)
{}

TimeSettingsScreen::~TimeSettingsScreen() {}

void TimeSettingsScreen::initialize()
{

    m_backButton = createButton(m_screenContainer, nullptr, 30, 15, 14, 10, backButtonCallback);

    StyleManager::applyTransparentButton(m_backButton);

    m_backButtonLabel = lv_label_create(m_backButton, NULL);
    lv_label_set_text(m_backButtonLabel, LV_SYMBOL_LEFT);

    m_titleLabel = createLabel(m_titleLabel, NULL, 195, 10, "Time Settings");

    m_timeLabel = createLabel(m_screenContainer, m_titleLabel, 5, 70, "Time [HH:MM]");

    m_hourSpinbox = lv_spinbox_create(m_screenContainer, NULL);
    lv_textarea_set_cursor_hidden(m_hourSpinbox, true);
    lv_textarea_set_text_align(m_hourSpinbox, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(m_hourSpinbox, 0, 23);
    lv_spinbox_set_digit_format(m_hourSpinbox, 2, 0);
    lv_obj_set_size(m_hourSpinbox, 40, 34);
    lv_obj_set_pos(m_hourSpinbox, 165, 61);
    lv_spinbox_set_rollover(m_hourSpinbox, true);

    m_hourIncrementButton
        = createButton(m_screenContainer, NULL, 20, 20, 175, 39, hourIncrementCallback);
    lv_theme_apply(m_hourIncrementButton, LV_THEME_SPINBOX_BTN);
    StyleManager::applyBorderlessContainer(m_hourIncrementButton);
    lv_obj_set_style_local_value_str(m_hourIncrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_PLUS);

    m_hourDecrementButton = createButton(m_screenContainer,
                                         m_hourIncrementButton,
                                         20,
                                         20,
                                         175,
                                         97,
                                         hourDecrementCallback);
    lv_obj_set_style_local_value_str(m_hourDecrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_MINUS);

    m_colonLabel = createLabel(m_screenContainer, m_timeLabel, 210, 70, ":");
    lv_obj_set_style_local_text_color(m_colonLabel,
                                      LV_OBJ_PART_MAIN,
                                      LV_STATE_DEFAULT,
                                      LV_COLOR_WHITE);

    m_minuteSpinbox = lv_spinbox_create(m_screenContainer, NULL);
    lv_textarea_set_cursor_hidden(m_minuteSpinbox, true);
    lv_textarea_set_text_align(m_minuteSpinbox, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(m_minuteSpinbox, 0, 59);
    lv_spinbox_set_digit_format(m_minuteSpinbox, 2, 0);
    lv_obj_set_size(m_minuteSpinbox, 40, 34);
    lv_obj_set_pos(m_minuteSpinbox, 219, 61);
    lv_spinbox_set_rollover(m_minuteSpinbox, true);

    m_minuteIncrementButton = createButton(m_screenContainer,
                                           m_hourIncrementButton,
                                           20,
                                           20,
                                           229,
                                           39,
                                           minuteIncrementCallback);
    lv_obj_set_style_local_value_str(m_minuteIncrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_PLUS);

    m_minuteDecrementButton = createButton(m_screenContainer,
                                           m_hourIncrementButton,
                                           20,
                                           20,
                                           229,
                                           97,
                                           minuteDecrementCallback);
    lv_obj_set_style_local_value_str(m_minuteDecrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_MINUS);

    m_timeOffsetButton
        = createButton(m_screenContainer, NULL, 85, 34, 145, 200, timeOffsetButtonCallback);
    m_timeOffsetButtonLabel = lv_label_create(m_timeOffsetButton, NULL);
    StyleManager::applyBorderlessContainer(m_timeOffsetButton);

    m_dateLabel = createLabel(m_screenContainer, NULL, 5, 129, "Date ");

    m_dateButton = createButton(m_screenContainer, NULL, 95, 43, 165, 119, dateButtonCallback);
    m_dateButtonLabel = lv_label_create(m_dateButton, NULL);
    lv_obj_set_style_local_border_opa(m_dateButton,
                                      LV_BTN_PART_MAIN,
                                      LV_STATE_DEFAULT,
                                      LV_OPA_TRANSP);
    lv_obj_set_style_local_text_color(m_dateButtonLabel,
                                      LV_OBJ_PART_MAIN,
                                      LV_STATE_DEFAULT,
                                      LV_COLOR_BLACK);

    m_lockScreenLabel = createLabel(m_screenContainer, NULL, 5, 170, "Lock screen after ");
    m_lockScreenDropdown = lv_dropdown_create(m_screenContainer, NULL);
    lv_dropdown_set_options(m_lockScreenDropdown,
                            "1 min\n"
                            "5 min\n"
                            "10 min\n"
                            "60 min\n"
                            "Never");
    lv_obj_set_size(m_lockScreenDropdown, 120, 34);
    lv_obj_set_pos(m_lockScreenDropdown, 165, 164);

    m_syncNtpButton = createButton(m_screenContainer, NULL, 130, 33, 5, 200, syncNtpButtonCallback);
    m_syncNtpLabel = lv_label_create(m_syncNtpButton, NULL);
    lv_label_set_text(m_syncNtpLabel, "Sync. Clock");
    StyleManager::applyWhiteButton(m_syncNtpButton);

    m_saveButton = createButton(m_screenContainer, NULL, 75, 33, 240, 200, saveButtonCallback);
    m_saveButtonLabel = lv_label_create(m_saveButton, NULL);
    lv_label_set_text(m_saveButtonLabel, "Save");
    StyleManager::applyWhiteButton(m_saveButton);
}

void TimeSettingsScreen::updateData()
{
    m_timeChanged = false;
    lv_dropdown_set_selected(m_lockScreenDropdown, getDropdownIndex());

    if (m_rtcManager->isRunning()) {
        RtcDateTime now = m_rtcManager->getCurrentDateTime();
        lv_label_set_text_fmt(m_dateButtonLabel, "%02d.%02d.%d", now.Day(), now.Month(), now.Year());
        lv_spinbox_set_value(m_hourSpinbox, now.Hour());
        lv_spinbox_set_value(m_minuteSpinbox, now.Minute());
    } else {
        lv_label_set_text(m_dateButtonLabel, "00.00.0000");
        lv_spinbox_set_value(m_hourSpinbox, 0);
        lv_spinbox_set_value(m_minuteSpinbox, 0);
    }

    m_timeOffset = m_config.timeOffset;
    char buf[16];
    if (m_timeOffset >= 0) {
        int hours = m_timeOffset / 3600;
        snprintf(buf, sizeof(buf), "UTC+%d", hours);
    } else {
        int hours = (-m_timeOffset) / 3600;
        snprintf(buf, sizeof(buf), "UTC-%d", hours);
    }
    lv_label_set_text(m_timeOffsetButtonLabel, buf);
}

int TimeSettingsScreen::getDropdownIndex() const
{
    switch (m_config.lcdLockTime) {
    case -1: {
        return 4;
        break;
    }

    case 60000: {
        return 0;
        break;
    }
    case 300000: {
        return 1;
        break;
    }
    case 600000: {
        return 2;
        break;
    }
    case 3600000: {
        return 3;
        break;
    }
    default: {
        return 2;
    }
    }
}

void TimeSettingsScreen::handleHourIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(m_hourSpinbox);
        m_timeChanged = true;
    }
}

void TimeSettingsScreen::handleHourDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(m_hourSpinbox) == 0)
            lv_spinbox_set_value(m_hourSpinbox, 23);
        else
            lv_spinbox_decrement(m_hourSpinbox);
        m_timeChanged = true;
    }
}

void TimeSettingsScreen::handleMinuteIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(m_minuteSpinbox);
        m_timeChanged = true;
    }
}

void TimeSettingsScreen::handleMinuteDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(m_minuteSpinbox);
        m_timeChanged = true;
    }
}

void TimeSettingsScreen::handleDateButton(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        m_calendar = lv_calendar_create(m_screenContainer, NULL);
        lv_obj_set_size(m_calendar, 235, 235);
        lv_obj_align(m_calendar, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_event_cb(m_calendar, calendarEventCallback);
        lv_obj_set_style_local_text_font(m_calendar,
                                         LV_CALENDAR_PART_DATE,
                                         LV_STATE_DEFAULT,
                                         lv_theme_get_font_small());
        if (m_rtcManager->isRunning()) {
            lv_calendar_date_t today;
            RtcDateTime now = m_rtcManager->getCurrentDateTime();
            today.year = now.Year();
            today.month = now.Month();
            today.day = now.Day();
            lv_calendar_set_today_date(m_calendar, &today);
            lv_calendar_set_showed_date(m_calendar, &today);
        } else {
            lv_calendar_date_t today;
            today.year = 2021;
            today.month = 1;
            today.day = 1;
            lv_calendar_set_today_date(m_calendar, &today);
            lv_calendar_set_showed_date(m_calendar, &today);
        }
    }
}

void TimeSettingsScreen::handleCalendarEvent(lv_obj_t *calendar, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_calendar_date_t *date = lv_calendar_get_pressed_date(calendar);
        if (date) {
            char logBuf[32];
            snprintf(logBuf,
                     sizeof(logBuf),
                     "Clicked date: %02d.%02d.%d",
                     date->day,
                     date->month,
                     date->year);
            LOG_UI(logBuf);
            lv_calendar_set_today_date(calendar, date);
            lv_calendar_set_showed_date(calendar, date);
            char buffer[16];
            itoa(date->day, buffer, 10);
            String label;
            if (atoi(buffer) < 10) {
                label = '0' + (String)buffer + '.';
            } else {
                label = (String)buffer + '.';
            }
            itoa(date->month, buffer, 10);
            if (atoi(buffer) < 10) {
                label += '0' + (String)buffer + '.';
            } else {
                label += (String)buffer + '.';
            }
            itoa(date->year, buffer, 10);
            label += (String)buffer;
            lv_label_set_text(m_dateButtonLabel, label.c_str());
            lv_obj_del(calendar);
            calendar = NULL;
            m_dateChanged = true;
        }
    }
}

void TimeSettingsScreen::handleSyncNtpButton(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        bool success = m_rtcManager->syncWithNTP(StringConstants::NTP_SERVER, m_config.timeOffset);
        if (!success) {
            LOG_UI("Time synchronization failed.");
        }
    }
}

void TimeSettingsScreen::handleSaveButton(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        switch (lv_dropdown_get_selected(m_lockScreenDropdown)) {
        case 0:
            m_config.lcdLockTime = 60000;
            break;
        case 1:
            m_config.lcdLockTime = 300000;
            break;
        case 2:
            m_config.lcdLockTime = 600000;
            break;
        case 3:
            m_config.lcdLockTime = 3600000;
            break;
        case 4:
            m_config.lcdLockTime = -1;
            break;
        default:
            m_config.lcdLockTime = 60000;
            break;
        }

        if (m_timeOffset != m_config.timeOffset) {
            m_config.timeOffset = m_timeOffset;
            // If time offset changed, we should re-sync with NTP server
            if (m_networkManager->isConnected()) {
                bool success = m_rtcManager->syncWithNTP(StringConstants::NTP_SERVER,
                                                         m_config.timeOffset);
                if (!success) {
                    LOG_UI("Time synchronization failed.");
                }
            }
        }

        m_sdCard.saveConfig(m_config, StringConstants::CONFIG_FILE_PATH);
        m_sdCard.printConfig(StringConstants::CONFIG_FILE_PATH);
        if (m_timeChanged == true) {
            String date = lv_label_get_text(m_dateButtonLabel)
                          + (String)lv_textarea_get_text(m_hourSpinbox) + ":"
                          + (String)lv_textarea_get_text(m_minuteSpinbox);
            RtcDateTime *dt = new RtcDateTime(atoi(date.substring(6, 10).c_str()),
                                              atoi(date.substring(3, 6).c_str()),
                                              atoi(date.substring(0, 2).c_str()),
                                              date.substring(10, 12).toDouble(),
                                              date.substring(13, 15).toDouble(),
                                              0);
            m_rtcManager->setDateTime(*dt);
            m_rtcManager->setIsRunning(true);
        }
        if (m_dateChanged == true) {
            RtcDateTime ori = m_rtcManager->getCurrentDateTime();
            String date = lv_label_get_text(m_dateButtonLabel);
            RtcDateTime *dt = new RtcDateTime(atoi(date.substring(6).c_str()),
                                              atoi(date.substring(3, 6).c_str()),
                                              atoi(date.substring(0, 2).c_str()),
                                              ori.Hour(),
                                              ori.Minute(),
                                              ori.Second());
            m_rtcManager->setDateTime(*dt);
            m_rtcManager->setIsRunning(true);
        }
        m_screenManager->switchToScreen(ScreenType::MAIN);
    }
}

// Static wrappers for LVGL callbacks
void TimeSettingsScreen::hourIncrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleHourIncrement(btn, event);
    }
}

void TimeSettingsScreen::hourDecrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleHourDecrement(btn, event);
    }
}

void TimeSettingsScreen::minuteIncrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleMinuteIncrement(btn, event);
    }
}

void TimeSettingsScreen::minuteDecrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleMinuteDecrement(btn, event);
    }
}

void TimeSettingsScreen::dateButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleDateButton(btn, event);
    }
}

void TimeSettingsScreen::calendarEventCallback(lv_obj_t *calendar, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleCalendarEvent(calendar, event);
    }
}

void TimeSettingsScreen::syncNtpButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleSyncNtpButton(btn, event);
    }
}

void TimeSettingsScreen::saveButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleSaveButton(btn, event);
    }
}

void TimeSettingsScreen::backButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleBackButton(btn, event);
    }
}

void TimeSettingsScreen::handleBackButton(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        m_screenManager->switchToScreen(ScreenType::SETTINGS);
    }
}

void TimeSettingsScreen::timeOffsetButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleTimeOffsetButton(btn, event);
    }
}

void TimeSettingsScreen::offsetOkCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleOffsetOk(btn, event);
    }
}

void TimeSettingsScreen::offsetCancelCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto *instance = getActiveInstance()) {
        instance->handleOffsetCancel(btn, event);
    }
}

void TimeSettingsScreen::handleTimeOffsetButton(lv_obj_t *btn, lv_event_t event)
{
    if (event != LV_EVENT_CLICKED)
        return;

    m_offsetPopup = createContainer(nullptr, nullptr, 220, 140, 0, 0);
    lv_obj_align(m_offsetPopup, NULL, LV_ALIGN_CENTER, 0, 0);

    m_timeOffsetDropdown = lv_dropdown_create(m_offsetPopup, NULL);
    lv_dropdown_set_options(m_timeOffsetDropdown,
                            "UTC-12\nUTC-11\nUTC-10\nUTC-9\nUTC-8\nUTC-7\nUTC-6\nUTC-5\nUTC-4\nUTC-"
                            "3\nUTC-2\nUTC-1\nUTC\nUTC+1\nUTC+2\nUTC+3\nUTC+4\nUTC+5\nUTC+6\nUTC+"
                            "7\nUTC+8\nUTC+9\nUTC+10\nUTC+11\nUTC+12");
    lv_obj_set_size(m_timeOffsetDropdown, 180, 40);
    lv_obj_align(m_timeOffsetDropdown, NULL, LV_ALIGN_CENTER, 0, -20);

    int preIdx = indexFromOffsetSeconds(m_timeOffset);
    if (preIdx >= 0 && preIdx <= 24) {
        lv_dropdown_set_selected(m_timeOffsetDropdown, preIdx);
    }

    m_offsetOkButton = createButton(m_offsetPopup, NULL, 80, 30, 20, 90, offsetOkCallback);
    lv_obj_t *okLbl = lv_label_create(m_offsetOkButton, NULL);
    lv_label_set_text(okLbl, "OK");
    StyleManager::applyWhiteButton(m_offsetOkButton);

    m_offsetCancelButton = createButton(m_offsetPopup, NULL, 80, 30, 120, 90, offsetCancelCallback);
    lv_obj_t *cancelLbl = lv_label_create(m_offsetCancelButton, NULL);
    lv_label_set_text(cancelLbl, "Cancel");
    StyleManager::applyWhiteButton(m_offsetCancelButton);
}

void TimeSettingsScreen::handleOffsetOk(lv_obj_t *btn, lv_event_t event)
{
    if (event != LV_EVENT_CLICKED)
        return;

    if (m_timeOffsetDropdown) {
        int idx = lv_dropdown_get_selected(m_timeOffsetDropdown);
        m_timeOffset = offsetSecondsFromIndex(idx);
    }

    if (m_timeOffsetDropdown && m_timeOffsetButtonLabel) {
        char buf[16];
        lv_dropdown_get_selected_str(m_timeOffsetDropdown, buf, sizeof(buf));
        lv_label_set_text(m_timeOffsetButtonLabel, buf);
    }

    if (m_offsetPopup) {
        lv_obj_del(m_offsetPopup);
        m_offsetPopup = nullptr;
    }
}

int TimeSettingsScreen::offsetSecondsFromIndex(int idx)
{
    if (idx < 0)
        idx = 12;
    if (idx > 24)
        idx = 24;
    int hours = idx - 12; // -12..+12
    return hours * 3600;
}

int TimeSettingsScreen::indexFromOffsetSeconds(int seconds)
{
    int hours = seconds / 3600;
    if (hours < -12)
        hours = -12;
    if (hours > 12)
        hours = 12;
    return hours + 12; // 0..24
}

void TimeSettingsScreen::handleOffsetCancel(lv_obj_t *btn, lv_event_t event)
{
    if (event != LV_EVENT_CLICKED)
        return;
    if (m_offsetPopup) {
        lv_obj_del(m_offsetPopup);
        m_offsetPopup = nullptr;
    }
}
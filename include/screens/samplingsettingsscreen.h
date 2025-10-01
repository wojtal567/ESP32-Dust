#pragma once

#include "screens/basescreen.h"

#include "utils/types.h"

class TaskManager;
class NetworkManager;
class MySD;
class ScreenManager;

class SamplingSettingsScreen : public BaseScreen<SamplingSettingsScreen>
{
public:
    SamplingSettingsScreen(Types::ConfigData &config,
                           TaskManager *taskManager,
                           NetworkManager *networkManager,
                           ScreenManager *screenManager,
                           MySD &sdCard);
    ~SamplingSettingsScreen() override;

    void initialize() override;
    void updateData();

private:
    void setSpinboxDigitFormat(lv_obj_t *spinbox, int min, int max, int offset);

    void handleSaveButton(lv_obj_t *btn, lv_event_t event);
    void handleHourIncrement(lv_obj_t *btn, lv_event_t event);
    void handleHourDecrement(lv_obj_t *btn, lv_event_t event);
    void handleMinuteIncrement(lv_obj_t *btn, lv_event_t event);
    void handleMinuteDecrement(lv_obj_t *btn, lv_event_t event);
    void handleSecondIncrement(lv_obj_t *btn, lv_event_t event);
    void handleSecondDecrement(lv_obj_t *btn, lv_event_t event);
    void handleSampleNumberIncrement(lv_obj_t *btn, lv_event_t event);
    void handleSampleNumberDecrement(lv_obj_t *btn, lv_event_t event);
    void handleMeasureAvPeriodIncrement(lv_obj_t *btn, lv_event_t event);
    void handleMeasureAvPeriodDecrement(lv_obj_t *btn, lv_event_t event);
    void handleTurnFanOnTimeIncrement(lv_obj_t *btn, lv_event_t event);
    void handleTurnFanOnTimeDecrement(lv_obj_t *btn, lv_event_t event);
    void handleBackButtonEvent(lv_obj_t *btn, lv_event_t event);

    // Static wrappers for LVGL callbacks
    static void backButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void saveButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void hourIncrementCallback(lv_obj_t *btn, lv_event_t event);
    static void hourDecrementCallback(lv_obj_t *btn, lv_event_t event);
    static void minuteIncrementCallback(lv_obj_t *btn, lv_event_t event);
    static void minuteDecrementCallback(lv_obj_t *btn, lv_event_t event);
    static void secondIncrementCallback(lv_obj_t *btn, lv_event_t event);
    static void secondDecrementCallback(lv_obj_t *btn, lv_event_t event);
    static void sampleNumberIncrementCallback(lv_obj_t *btn, lv_event_t event);
    static void sampleNumberDecrementCallback(lv_obj_t *btn, lv_event_t event);
    static void measureAvPeriodIncrementCallback(lv_obj_t *btn, lv_event_t event);
    static void measureAvPeriodDecrementCallback(lv_obj_t *btn, lv_event_t event);
    static void turnFanOnTimeIncrementCallback(lv_obj_t *btn, lv_event_t event);
    static void turnFanOnTimeDecrementCallback(lv_obj_t *btn, lv_event_t event);

    lv_obj_t *m_backButton{nullptr};
    lv_obj_t *m_backButtonLabel{nullptr};

    lv_obj_t *m_measurePeriodLabel{nullptr};
    lv_obj_t *m_measurePeriodHourSpinbox{nullptr};
    lv_obj_t *m_measurePeriodHourIncrementButton{nullptr};
    lv_obj_t *m_measurePeriodHourDecrementButton{nullptr};

    lv_obj_t *m_measurePeriodHourMinuteColonLabel{nullptr};

    lv_obj_t *m_measurePeriodMinuteSpinbox{nullptr};
    lv_obj_t *m_measurePeriodMinuteIncrementButton{nullptr};
    lv_obj_t *m_measurePeriodMinuteDecrementButton{nullptr};

    lv_obj_t *m_measurePeriodMinuteSecondColonLabel{nullptr};

    lv_obj_t *m_measurePeriodSecondSpinbox{nullptr};
    lv_obj_t *m_measurePeriodSecondIncrementButton{nullptr};
    lv_obj_t *m_measurePeriodSecondDecrementButton{nullptr};

    lv_obj_t *m_samplesNumberLabel{nullptr};
    lv_obj_t *m_samplesNumberSpinbox{nullptr};
    lv_obj_t *m_samplesNumberIncrementButton{nullptr};
    lv_obj_t *m_samplesNumberDecrementButton{nullptr};

    lv_obj_t *m_measureAvPeriodLabel{nullptr};
    lv_obj_t *m_measureAvPeriodSpinbox{nullptr};
    lv_obj_t *m_measureAvPeriodIncrementButton{nullptr};
    lv_obj_t *m_measureAvPeriodDecrementButton{nullptr};

    lv_obj_t *m_turnFanOnTimeLabel{nullptr};
    lv_obj_t *m_turnFanOnTimeSpinbox{nullptr};
    lv_obj_t *m_turnFanOnTimeIncrementButton{nullptr};
    lv_obj_t *m_turnFanOnTimeDecrementButton{nullptr};

    lv_obj_t *m_saveButton{nullptr};
    lv_obj_t *m_saveButtonLabel{nullptr};

    Types::ConfigData &m_config;
    TaskManager *m_taskManager;
    NetworkManager *m_networkManager;
    ScreenManager *m_screenManager;
    MySD &m_sdCard;
};
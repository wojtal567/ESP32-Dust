#include <GlobalVariables.hpp>

#include "Utils/constants.h"
#include "Utils/stringConstants.h"
#include "managers/SensorManager.h"
#include "managers/stylemanager.h"
#include "utils/timeUtils.h"

void set_spinbox_digit_format(lv_obj_t *spinbox, int32_t range_min, int32_t range_max, int offset)
{
    int _spinbox_value = lv_spinbox_get_value(spinbox) + offset;
    if (_spinbox_value > 9)
    {
        if (_spinbox_value > 99)
        {
            lv_spinbox_set_digit_format(spinbox, 3, 0);
        }
        else
        {
            lv_spinbox_set_digit_format(spinbox, 2, 0);
        }
    }
    else
    {
        lv_spinbox_set_digit_format(spinbox, 1, 0);
    }
    lv_spinbox_set_range(spinbox, range_min, range_max);
}

lv_obj_t *my_lv_btn_create(lv_obj_t *par, const lv_obj_t *copy, lv_coord_t width, lv_coord_t height, lv_coord_t x_position, lv_coord_t y_position, lv_event_cb_t event_cb)
{
    lv_obj_t *btn = lv_btn_create(par, copy);
    lv_obj_set_size(btn, width, height);
    lv_obj_set_pos(btn, x_position, y_position);
    lv_obj_set_event_cb(btn, event_cb);
    return btn;
}

lv_obj_t *my_lv_label_create(lv_obj_t *par, const lv_obj_t *copy, lv_coord_t x_position, lv_coord_t y_position, const char *text = "", lv_color_t color = LV_COLOR_WHITE)
{
    lv_obj_t *new_label = lv_label_create(par, copy);
    lv_obj_set_pos(new_label, x_position, y_position);
    lv_label_set_text(new_label, text);
    lv_obj_set_style_local_text_color(new_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, color);

    return new_label;
}

// Function that turns fan on
void turnFanOnFunc(lv_task_t *task)
{
    sensorManager.wakeDustSensor();
    lv_task_set_prio(turnFanOn, LV_TASK_PRIO_OFF);
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
    if (lastSampleTimestamp == lastRecordToCheck[0]["timestamp"].as<String>())
    {
        Serial.println("Last sample has been saved correctly - return true.");
        return true;
    }
    else
    {
        Serial.println("Something went wrong saving last sample - return false");
        return false;
    }
}

// Get single sample and set text
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
        if (lastSampleSaved) {
            lv_obj_set_style_local_bg_color(ledAtLock,
                                            LV_LED_PART_MAIN,
                                            LV_STATE_DEFAULT,
                                            LV_COLOR_GREEN);
            lv_obj_set_style_local_shadow_color(ledAtLock,
                                                LV_LED_PART_MAIN,
                                                LV_STATE_DEFAULT,
                                                LV_COLOR_GREEN);

        } else {
            lv_obj_set_style_local_bg_color(ledAtLock,
                                            LV_LED_PART_MAIN,
                                            LV_STATE_DEFAULT,
                                            LV_COLOR_RED);
            lv_obj_set_style_local_shadow_color(ledAtLock,
                                                LV_LED_PART_MAIN,
                                                LV_STATE_DEFAULT,
                                                LV_COLOR_RED);
        }

        mainScreen->updateLedStatus(isLastSampleSaved);
    }
}

// Locking button clicked
static void lockButton_task(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_disp_load_scr(lockScr);
}

// Unlocking button clicked
static void unlockButton_task(lv_obj_t *obj, lv_event_t event)
{
    Serial.print(lv_btn_get_state(unlockButton));
    if (event == LV_EVENT_CLICKED)
        screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);
}

static void temp_settings_btn(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
        lv_scr_load(samplingSettingsScr);
}

static void sampling_hourIncrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_increment(measurePeriodHour);
    }
}

static void sampling_secondIncrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(measurePeriodsecond) == 59)
        {
            if (lv_spinbox_get_value(measurePeriodMinute) == 59)
            {
                if (lv_spinbox_get_value(measurePeriodHour) != 24)
                {
                    lv_spinbox_set_value(measurePeriodMinute, 0);
                    lv_spinbox_set_value(measurePeriodsecond, 0);
                    lv_spinbox_increment(measurePeriodHour);
                }
            }
            else
            {
                lv_spinbox_set_value(measurePeriodsecond, 0);
                lv_spinbox_increment(measurePeriodMinute);
            }
        }
        else
        {
            lv_spinbox_increment(measurePeriodsecond);
        }
    }
}

static void sampling_secondDecrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(measurePeriodsecond) == 0 && lv_spinbox_get_value(measurePeriodMinute) != 0)
        {
            if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond) - 1) >= (lv_spinbox_get_value(turnFanOnTime) + (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
            {
                lv_spinbox_decrement(measurePeriodMinute);
                lv_spinbox_set_value(measurePeriodsecond, 59);
            }
        }
        else
        {
            if (lv_spinbox_get_value(measurePeriodsecond) == 0 && lv_spinbox_get_value(measurePeriodMinute) == 0)
            {
                if (lv_spinbox_get_value(measurePeriodHour) != 0)
                {
                    if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond) - 1) >= (lv_spinbox_get_value(turnFanOnTime) + (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
                    {
                        lv_spinbox_decrement(measurePeriodHour);
                        lv_spinbox_set_value(measurePeriodMinute, 59);
                        lv_spinbox_set_value(measurePeriodsecond, 59);
                    }
                }
            }
            else
            {
                if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond) - 1) >= (lv_spinbox_get_value(turnFanOnTime) + (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
                    lv_spinbox_decrement(measurePeriodsecond);
            }
        }
    }
}

static void sampling_hourDecrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (((lv_spinbox_get_value(measurePeriodHour) - 1) * 3600) + (lv_spinbox_get_value(measurePeriodMinute) * 60) + lv_spinbox_get_value(measurePeriodsecond) >= 1)
        {
            if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond) - 3600) >= (lv_spinbox_get_value(turnFanOnTime) + (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
                lv_spinbox_decrement(measurePeriodHour);
        }
    }
}

static void sampling_minuteIncrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(measurePeriodMinute) == 59)
        {
            if (lv_spinbox_get_value(measurePeriodHour) != 24)
            {
                lv_spinbox_set_value(measurePeriodMinute, 0);
                lv_spinbox_increment(measurePeriodHour);
            }
        }
        else
        {
            lv_spinbox_increment(measurePeriodMinute);
        }
    }
}

static void sampling_minuteDecrement(lv_obj_t *btn, lv_event_t e)
{
    if (e == LV_EVENT_SHORT_CLICKED || e == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (lv_spinbox_get_value(measurePeriodMinute) == 0)
        {
            if (lv_spinbox_get_value(measurePeriodHour) != 0)
                if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond) - 60) >= (lv_spinbox_get_value(turnFanOnTime) + (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
                {
                    lv_spinbox_set_value(measurePeriodMinute, 59);
                    lv_spinbox_decrement(measurePeriodHour);
                }
        }
        else
        {
            if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond) - 60) >= (lv_spinbox_get_value(turnFanOnTime) + (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
                lv_spinbox_decrement(measurePeriodMinute);
        }
    }
}

static void measureNumberIncrement_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond)) >= (lv_spinbox_get_value(turnFanOnTime) + ((lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod))))
        {
            set_spinbox_digit_format(measureNumber, Constants::MIN_RANGE, Constants::MAX_RANGE, 1);
            lv_spinbox_increment(measureNumber);
        }
    }
}

static void measureNumberDecrement_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        set_spinbox_digit_format(measureNumber, Constants::MIN_RANGE, Constants::MAX_RANGE, -1);
        lv_spinbox_decrement(measureNumber);
    }
}

static void turnFanOnTimeIncrement_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond)) >= (lv_spinbox_get_value(turnFanOnTime) + 1 + (lv_spinbox_get_value(measureNumber) - 1) * lv_spinbox_get_value(measureAvPeriod)))
        {
            set_spinbox_digit_format(turnFanOnTime, Constants::MIN_RANGE, Constants::MAX_RANGE, 1);
            lv_spinbox_increment(turnFanOnTime);
        }
    }
}

static void turnFanOnTimeDecrement_func(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        set_spinbox_digit_format(turnFanOnTime, Constants::MIN_RANGE, Constants::MAX_RANGE, -1);
        lv_spinbox_decrement(turnFanOnTime);
    }
}

static void av_periodIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if ((lv_spinbox_get_value(measurePeriodHour) * 3600 + lv_spinbox_get_value(measurePeriodMinute) * 60 + lv_spinbox_get_value(measurePeriodsecond)) >= (lv_spinbox_get_value(turnFanOnTime) + (lv_spinbox_get_value(measureNumber) - 1) * (lv_spinbox_get_value(measureAvPeriod) + 1)))
        {
            set_spinbox_digit_format(measureAvPeriod, Constants::MIN_RANGE, Constants::MAX_RANGE, 1);
            lv_spinbox_increment(measureAvPeriod);
        }
    }
}

static void av_periodDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        set_spinbox_digit_format(measureAvPeriod, Constants::MIN_RANGE, Constants::MAX_RANGE, -1);
        lv_spinbox_decrement(measureAvPeriod);
    }
}

static void sampling_settings_save_btn(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        int get_value = lv_spinbox_get_value(measurePeriodHour) * 60 * 60000 + lv_spinbox_get_value(measurePeriodMinute) * 60000 + lv_spinbox_get_value(measurePeriodsecond) * 1000;
        config.timeBetweenSavingSamples = get_value;
        config.numberOfSamples = lv_spinbox_get_value(measureNumber);
        config.measurePeriod = lv_spinbox_get_value(measureAvPeriod) * 1000;
        config.turnFanTime = lv_spinbox_get_value(turnFanOnTime) * 1000;
        getSample = lv_task_create(getSampleFunc, (config.timeBetweenSavingSamples - (config.numberOfSamples - 1) * config.measurePeriod), LV_TASK_PRIO_HIGH, NULL);
        turnFanOn = lv_task_create(turnFanOnFunc, config.timeBetweenSavingSamples - config.turnFanTime, LV_TASK_PRIO_HIGHEST, NULL);
        networkManager.saveConfig(config, StringConstants::CONFIG_FILE_PATH);
        networkManager.printConfig(StringConstants::CONFIG_FILE_PATH);
        screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);
    }
}

static void sampling_settings_back_btn(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        lv_spinbox_set_value(measurePeriodHour, ((config.timeBetweenSavingSamples / 60000) / 60));
        lv_spinbox_set_value(measurePeriodMinute, ((config.timeBetweenSavingSamples / 60000) % 60));
        screenManager.switchToScreen(BaseScreen::ScreenType::SETTINGS);
    }
}
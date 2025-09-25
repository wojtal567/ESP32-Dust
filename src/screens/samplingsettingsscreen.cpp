#include "screens/samplingsettingsscreen.h"

#include <managers/networkmanager.h>
#include <managers/stylemanager.h>
#include <managers/taskmanager.h>
#include <utils/constants.h>
#include <utils/stringConstants.h>

// Static instance pointer for LVGL callbacks
SamplingSettingsScreen *SamplingSettingsScreen::s_activeInstance = nullptr;

SamplingSettingsScreen::SamplingSettingsScreen(const Types::ConfigData &config,
                                               TaskManager *taskManager,
                                               NetworkManager *networkManager,
                                               ScreenManager &screenManager)
    : BaseScreen(ScreenType::SAMPLING_SETTINGS)
    , m_config(config)
    , m_taskManager(taskManager)
    , m_networkManager(networkManager)
    , m_screenManager(screenManager)
{
    s_activeInstance = this;
}

SamplingSettingsScreen::~SamplingSettingsScreen() {}

void SamplingSettingsScreen::initialize()
{
    m_backButton = createButton(m_screenContainer, nullptr, 30, 15, 14, 10, backButtonCallback);

    StyleManager::applyTransparentButton(m_backButton);

    m_backButtonLabel = lv_label_create(m_backButton, NULL);
    lv_label_set_text(m_backButtonLabel, LV_SYMBOL_LEFT);

    m_saveButton = createButton(m_screenContainer, nullptr, 75, 25, 231, 10, saveButtonCallback);

    StyleManager::applyWhiteButton(m_saveButton);
    m_saveButtonLabel = lv_label_create(m_saveButton, NULL);
    lv_label_set_text(m_saveButtonLabel, "Save");

    m_measurePeriodLabel = createLabel(m_screenContainer,
                                       NULL,
                                       5,
                                       52,
                                       "Measurements\nsaving time\n[HH:MM:SS]",
                                       LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(m_measurePeriodLabel,
                                     LV_OBJ_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     &lv_font_montserrat_14);

    m_measurePeriodHourSpinbox = lv_spinbox_create(m_screenContainer, NULL);
    lv_textarea_set_cursor_hidden(m_measurePeriodHourSpinbox, true);
    lv_textarea_set_text_align(m_measurePeriodHourSpinbox, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(m_measurePeriodHourSpinbox, 0, 24);
    lv_spinbox_set_digit_format(m_measurePeriodHourSpinbox, 2, 0);
    lv_obj_set_size(m_measurePeriodHourSpinbox, 40, 34);
    lv_obj_set_pos(m_measurePeriodHourSpinbox, 165, 61);

    m_measurePeriodHourIncrementButton
        = createButton(m_screenContainer, NULL, 20, 20, 175, 39, hourIncrementCallback);
    lv_theme_apply(m_measurePeriodHourIncrementButton, LV_THEME_SPINBOX_BTN);
    StyleManager::applyBorderlessContainer(m_measurePeriodHourIncrementButton);
    lv_obj_set_style_local_value_str(m_measurePeriodHourIncrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_PLUS);

    m_measurePeriodHourDecrementButton = createButton(m_screenContainer,
                                                      m_measurePeriodHourIncrementButton,
                                                      20,
                                                      20,
                                                      175,
                                                      97,
                                                      hourDecrementCallback);
    lv_obj_set_style_local_value_str(m_measurePeriodHourDecrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_MINUS);

    m_measurePeriodHourMinuteColonLabel
        = createLabel(m_screenContainer, nullptr, 210, 70, ":", LV_COLOR_WHITE);

    m_measurePeriodMinuteSpinbox = lv_spinbox_create(m_screenContainer, NULL);
    lv_textarea_set_cursor_hidden(m_measurePeriodMinuteSpinbox, true);
    lv_textarea_set_text_align(m_measurePeriodMinuteSpinbox, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(m_measurePeriodMinuteSpinbox, 0, 59);
    lv_spinbox_set_digit_format(m_measurePeriodMinuteSpinbox, 2, 0);
    lv_obj_set_size(m_measurePeriodMinuteSpinbox, 40, 34);
    lv_obj_set_pos(m_measurePeriodMinuteSpinbox, 219, 61);

    m_measurePeriodMinuteIncrementButton = createButton(m_screenContainer,
                                                        m_measurePeriodHourIncrementButton,
                                                        20,
                                                        20,
                                                        229,
                                                        39,
                                                        minuteIncrementCallback);
    lv_obj_set_style_local_value_str(m_measurePeriodMinuteIncrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_PLUS);

    m_measurePeriodMinuteDecrementButton = createButton(m_screenContainer,
                                                        m_measurePeriodHourIncrementButton,
                                                        20,
                                                        20,
                                                        229,
                                                        97,
                                                        minuteDecrementCallback);
    lv_obj_set_style_local_value_str(m_measurePeriodMinuteDecrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_MINUS);

    m_measurePeriodMinuteSecondColonLabel
        = createLabel(m_screenContainer, nullptr, 264, 70, ":", LV_COLOR_WHITE);

    m_measurePeriodSecondSpinbox = lv_spinbox_create(m_screenContainer, NULL);
    lv_textarea_set_cursor_hidden(m_measurePeriodSecondSpinbox, true);
    lv_textarea_set_text_align(m_measurePeriodSecondSpinbox, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(m_measurePeriodSecondSpinbox, 0, 59);
    lv_spinbox_set_digit_format(m_measurePeriodSecondSpinbox, 2, 0);
    lv_obj_set_size(m_measurePeriodSecondSpinbox, 40, 34);
    lv_obj_set_pos(m_measurePeriodSecondSpinbox, 274, 61);

    m_measurePeriodSecondIncrementButton = createButton(m_screenContainer,
                                                        m_measurePeriodHourIncrementButton,
                                                        20,
                                                        20,
                                                        284,
                                                        39,
                                                        secondIncrementCallback);
    lv_obj_set_style_local_value_str(m_measurePeriodSecondIncrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_PLUS);

    m_measurePeriodSecondDecrementButton = createButton(m_screenContainer,
                                                        m_measurePeriodHourIncrementButton,
                                                        20,
                                                        20,
                                                        284,
                                                        97,
                                                        secondDecrementCallback);
    lv_obj_set_style_local_value_str(m_measurePeriodSecondDecrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_MINUS);

    m_samplesNumberLabel
        = createLabel(m_screenContainer, NULL, 5, 127, "Number of samples", LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(m_samplesNumberLabel,
                                     LV_OBJ_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     &lv_font_montserrat_14);

    m_samplesNumberSpinbox = lv_spinbox_create(m_screenContainer, NULL);
    lv_textarea_set_cursor_hidden(m_samplesNumberSpinbox, true);
    lv_textarea_set_text_align(m_samplesNumberSpinbox, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(m_samplesNumberSpinbox, 1, 20);
    lv_spinbox_set_digit_format(m_samplesNumberSpinbox, 2, 0);
    lv_obj_set_size(m_samplesNumberSpinbox, 50, 34);
    lv_obj_set_pos(m_samplesNumberSpinbox, 206, 119);
    setSpinboxDigitFormat(m_samplesNumberSpinbox, Constants::MIN_RANGE, Constants::MAX_RANGE, 0);

    m_samplesNumberIncrementButton = createButton(m_screenContainer,
                                                  m_measurePeriodHourIncrementButton,
                                                  40,
                                                  34,
                                                  257,
                                                  119,
                                                  sampleNumberIncrementCallback);
    lv_obj_set_style_local_value_str(m_samplesNumberIncrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_PLUS);

    m_samplesNumberDecrementButton = createButton(m_screenContainer,
                                                  m_measurePeriodHourIncrementButton,
                                                  40,
                                                  34,
                                                  165,
                                                  119,
                                                  sampleNumberDecrementCallback);
    lv_obj_set_style_local_value_str(m_samplesNumberDecrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_MINUS);

    m_measureAvPeriodLabel = createLabel(m_screenContainer,
                                         NULL,
                                         5,
                                         165,
                                         "Time between\nmeasurements [s]",
                                         LV_COLOR_WHITE);

    m_measureAvPeriodSpinbox = lv_spinbox_create(m_screenContainer, NULL);
    lv_textarea_set_cursor_hidden(m_measureAvPeriodSpinbox, true);
    lv_textarea_set_text_align(m_measureAvPeriodSpinbox, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(m_measureAvPeriodSpinbox, 5, 999);
    lv_spinbox_set_digit_format(m_measureAvPeriodSpinbox, 3, 0);
    lv_obj_set_size(m_measureAvPeriodSpinbox, 50, 34);
    lv_obj_set_pos(m_measureAvPeriodSpinbox, 206, 160);
    m_measureAvPeriodIncrementButton = createButton(m_screenContainer,
                                                    m_measurePeriodHourIncrementButton,
                                                    40,
                                                    34,
                                                    257,
                                                    160,
                                                    measureAvPeriodIncrementCallback);
    lv_obj_set_style_local_value_str(m_measureAvPeriodIncrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_PLUS);

    m_measureAvPeriodDecrementButton = createButton(m_screenContainer,
                                                    m_measurePeriodHourIncrementButton,
                                                    40,
                                                    34,
                                                    165,
                                                    160,
                                                    measureAvPeriodDecrementCallback);
    lv_obj_set_style_local_value_str(m_measureAvPeriodDecrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_MINUS);

    m_turnFanOnTimeLabel = createLabel(m_screenContainer,
                                       m_measurePeriodLabel,
                                       5,
                                       205,
                                       "Fan running time \nbefore measure [s]:",
                                       LV_COLOR_WHITE);

    m_turnFanOnTimeSpinbox = lv_spinbox_create(m_screenContainer, NULL);
    lv_textarea_set_cursor_hidden(m_turnFanOnTimeSpinbox, true);
    lv_textarea_set_text_align(m_turnFanOnTimeSpinbox, LV_LABEL_ALIGN_CENTER);
    lv_spinbox_set_range(m_turnFanOnTimeSpinbox, 1, 999);
    lv_spinbox_set_digit_format(m_turnFanOnTimeSpinbox, 3, 0);
    lv_obj_set_size(m_turnFanOnTimeSpinbox, 50, 34);
    lv_obj_set_pos(m_turnFanOnTimeSpinbox, 206, 200);

    m_turnFanOnTimeIncrementButton = createButton(m_screenContainer,
                                                  m_measurePeriodHourIncrementButton,
                                                  40,
                                                  34,
                                                  257,
                                                  200,
                                                  turnFanOnTimeIncrementCallback);
    lv_obj_set_style_local_value_str(m_turnFanOnTimeIncrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_PLUS);

    m_turnFanOnTimeDecrementButton = createButton(m_screenContainer,
                                                  m_measurePeriodHourIncrementButton,
                                                  40,
                                                  34,
                                                  165,
                                                  200,
                                                  turnFanOnTimeDecrementCallback);
    lv_obj_set_style_local_value_str(m_turnFanOnTimeDecrementButton,
                                     LV_BTN_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     LV_SYMBOL_MINUS);
}

void SamplingSettingsScreen::updateData()
{
    lv_spinbox_set_value(m_measurePeriodHourSpinbox,
                         ((m_config.timeBetweenSavingSamples / 60000) / 60));
    lv_spinbox_set_value(m_measurePeriodMinuteSpinbox,
                         ((m_config.timeBetweenSavingSamples / 60000) % 60));
    lv_spinbox_set_value(m_measurePeriodSecondSpinbox,
                         (m_config.timeBetweenSavingSamples / 1000) % 60);

    lv_spinbox_set_value(m_samplesNumberSpinbox, m_config.numberOfSamples);

    lv_spinbox_set_value(m_measureAvPeriodSpinbox, (m_config.measurePeriod / 1000));

    setSpinboxDigitFormat(m_measureAvPeriodSpinbox, Constants::MIN_RANGE, Constants::MAX_RANGE, -1);

    lv_spinbox_set_value(m_turnFanOnTimeSpinbox, (m_config.turnFanTime / 1000));
    setSpinboxDigitFormat(m_turnFanOnTimeSpinbox, Constants::MIN_RANGE, Constants::MAX_RANGE, 0);
}

void SamplingSettingsScreen::setSpinboxDigitFormat(lv_obj_t *spinbox, int min, int max, int offset)
{
    int spinboxValue = lv_spinbox_get_value(spinbox) + offset;
    if (spinboxValue > 9) {
        if (spinboxValue > 99) {
            lv_spinbox_set_digit_format(spinbox, 3, 0);
        } else {
            lv_spinbox_set_digit_format(spinbox, 2, 0);
        }
    } else {
        lv_spinbox_set_digit_format(spinbox, 1, 0);
    }
    lv_spinbox_set_range(spinbox, min, max);
}

void SamplingSettingsScreen::handleSaveButton(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        int get_value = lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 60 * 60000
                        + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60000
                        + lv_spinbox_get_value(m_measurePeriodSecondSpinbox) * 1000;
        m_config.timeBetweenSavingSamples = get_value;
        m_config.numberOfSamples = lv_spinbox_get_value(m_samplesNumberSpinbox);
        m_config.measurePeriod = lv_spinbox_get_value(m_measureAvPeriodSpinbox) * 1000;
        m_config.turnFanTime = lv_spinbox_get_value(m_turnFanOnTimeSpinbox) * 1000;

        if (m_taskManager) {
            m_taskManager->recreateSampleTasksFromConfig();
        }

        m_networkManager->saveConfig(m_config, StringConstants::CONFIG_FILE_PATH);
        m_networkManager->printConfig(StringConstants::CONFIG_FILE_PATH);
        m_screenManager.switchToScreen(BaseScreen::ScreenType::MAIN);
    }
}

void SamplingSettingsScreen::handleHourIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_increment(m_measurePeriodHourSpinbox);
    }
}

void SamplingSettingsScreen::handleHourDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (((lv_spinbox_get_value(m_measurePeriodHourSpinbox) - 1) * 3600)
                + (lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60)
                + lv_spinbox_get_value(m_measurePeriodSecondSpinbox)
            >= 1) {
            if ((lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 3600
                 + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60
                 + lv_spinbox_get_value(m_measurePeriodSecondSpinbox) - 3600)
                >= (lv_spinbox_get_value(m_turnFanOnTimeSpinbox)
                    + (lv_spinbox_get_value(m_samplesNumberSpinbox) - 1)
                          * lv_spinbox_get_value(m_measureAvPeriodSpinbox)))
                lv_spinbox_decrement(m_measurePeriodHourSpinbox);
        }
    }
}

void SamplingSettingsScreen::handleMinuteIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) == 59) {
            if (lv_spinbox_get_value(m_measurePeriodHourSpinbox) != 24) {
                lv_spinbox_set_value(m_measurePeriodMinuteSpinbox, 0);
                lv_spinbox_increment(m_measurePeriodHourSpinbox);
            }
        } else {
            lv_spinbox_increment(m_measurePeriodMinuteSpinbox);
        }
    }
}

void SamplingSettingsScreen::handleMinuteDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) == 0) {
            if (lv_spinbox_get_value(m_measurePeriodHourSpinbox) != 0)
                if ((lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 3600
                     + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60
                     + lv_spinbox_get_value(m_measurePeriodSecondSpinbox) - 60)
                    >= (lv_spinbox_get_value(m_turnFanOnTimeSpinbox)
                        + (lv_spinbox_get_value(m_samplesNumberSpinbox) - 1)
                              * lv_spinbox_get_value(m_measureAvPeriodSpinbox))) {
                    lv_spinbox_set_value(m_measurePeriodMinuteSpinbox, 59);
                    lv_spinbox_decrement(m_measurePeriodHourSpinbox);
                }
        } else {
            if ((lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 3600
                 + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60
                 + lv_spinbox_get_value(m_measurePeriodSecondSpinbox) - 60)
                >= (lv_spinbox_get_value(m_turnFanOnTimeSpinbox)
                    + (lv_spinbox_get_value(m_samplesNumberSpinbox) - 1)
                          * lv_spinbox_get_value(m_measureAvPeriodSpinbox)))
                lv_spinbox_decrement(m_measurePeriodMinuteSpinbox);
        }
    }
}

void SamplingSettingsScreen::handleSecondIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(m_measurePeriodSecondSpinbox) == 59) {
            if (lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) == 59) {
                if (lv_spinbox_get_value(m_measurePeriodHourSpinbox) != 24) {
                    lv_spinbox_set_value(m_measurePeriodMinuteSpinbox, 0);
                    lv_spinbox_set_value(m_measurePeriodSecondSpinbox, 0);
                    lv_spinbox_increment(m_measurePeriodHourSpinbox);
                }
            } else {
                lv_spinbox_set_value(m_measurePeriodSecondSpinbox, 0);
                lv_spinbox_increment(m_measurePeriodMinuteSpinbox);
            }
        } else {
            lv_spinbox_increment(m_measurePeriodSecondSpinbox);
        }
    }
}

void SamplingSettingsScreen::handleSecondDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if (lv_spinbox_get_value(m_measurePeriodSecondSpinbox) == 0
            && lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) != 0) {
            if ((lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 3600
                 + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60
                 + lv_spinbox_get_value(m_measurePeriodSecondSpinbox) - 1)
                >= (lv_spinbox_get_value(m_turnFanOnTimeSpinbox)
                    + (lv_spinbox_get_value(m_samplesNumberSpinbox) - 1)
                          * lv_spinbox_get_value(m_measureAvPeriodSpinbox))) {
                lv_spinbox_decrement(m_measurePeriodMinuteSpinbox);
                lv_spinbox_set_value(m_measurePeriodSecondSpinbox, 59);
            }
        } else {
            if (lv_spinbox_get_value(m_measurePeriodSecondSpinbox) == 0
                && lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) == 0) {
                if (lv_spinbox_get_value(m_measurePeriodHourSpinbox) != 0) {
                    if ((lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 3600
                         + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60
                         + lv_spinbox_get_value(m_measurePeriodSecondSpinbox) - 1)
                        >= (lv_spinbox_get_value(m_turnFanOnTimeSpinbox)
                            + (lv_spinbox_get_value(m_samplesNumberSpinbox) - 1)
                                  * lv_spinbox_get_value(m_measureAvPeriodSpinbox))) {
                        lv_spinbox_decrement(m_measurePeriodHourSpinbox);
                        lv_spinbox_set_value(m_measurePeriodMinuteSpinbox, 59);
                        lv_spinbox_set_value(m_measurePeriodSecondSpinbox, 59);
                    }
                }
            } else {
                if ((lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 3600
                     + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60
                     + lv_spinbox_get_value(m_measurePeriodSecondSpinbox) - 1)
                    >= (lv_spinbox_get_value(m_turnFanOnTimeSpinbox)
                        + (lv_spinbox_get_value(m_samplesNumberSpinbox) - 1)
                              * lv_spinbox_get_value(m_measureAvPeriodSpinbox)))
                    lv_spinbox_decrement(m_measurePeriodSecondSpinbox);
            }
        }
    }
}

void SamplingSettingsScreen::handleSampleNumberIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if ((lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 3600
             + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60
             + lv_spinbox_get_value(m_measurePeriodSecondSpinbox))
            >= (lv_spinbox_get_value(m_turnFanOnTimeSpinbox)
                + ((lv_spinbox_get_value(m_samplesNumberSpinbox) - 1)
                   * lv_spinbox_get_value(m_measureAvPeriodSpinbox)))) {
            setSpinboxDigitFormat(m_samplesNumberSpinbox,
                                  Constants::MIN_RANGE,
                                  Constants::MAX_RANGE,
                                  1);
            lv_spinbox_increment(m_samplesNumberSpinbox);
        }
    }
}

void SamplingSettingsScreen::handleSampleNumberDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        setSpinboxDigitFormat(m_samplesNumberSpinbox, Constants::MIN_RANGE, Constants::MAX_RANGE, 0);
        lv_spinbox_decrement(m_samplesNumberSpinbox);
    }
}

void SamplingSettingsScreen::handleMeasureAvPeriodIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if ((lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 3600
             + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60
             + lv_spinbox_get_value(m_measurePeriodSecondSpinbox))
            >= (lv_spinbox_get_value(m_turnFanOnTimeSpinbox)
                + (lv_spinbox_get_value(m_samplesNumberSpinbox) - 1)
                      * (lv_spinbox_get_value(m_measureAvPeriodSpinbox) + 1))) {
            setSpinboxDigitFormat(m_measureAvPeriodSpinbox,
                                  Constants::MIN_RANGE,
                                  Constants::MAX_RANGE,
                                  1);
            lv_spinbox_increment(m_measureAvPeriodSpinbox);
        }
    }
}

void SamplingSettingsScreen::handleMeasureAvPeriodDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        lv_spinbox_decrement(m_measureAvPeriodSpinbox);
    }
}

void SamplingSettingsScreen::handleTurnFanOnTimeIncrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {
        if ((lv_spinbox_get_value(m_measurePeriodHourSpinbox) * 3600
             + lv_spinbox_get_value(m_measurePeriodMinuteSpinbox) * 60
             + lv_spinbox_get_value(m_measurePeriodSecondSpinbox))
            >= (lv_spinbox_get_value(m_turnFanOnTimeSpinbox) + 1
                + (lv_spinbox_get_value(m_samplesNumberSpinbox) - 1)
                      * lv_spinbox_get_value(m_measureAvPeriodSpinbox))) {
            setSpinboxDigitFormat(m_turnFanOnTimeSpinbox,
                                  Constants::MIN_RANGE,
                                  Constants::MAX_RANGE,
                                  1);
            lv_spinbox_increment(m_turnFanOnTimeSpinbox);
        }
    }
}

void SamplingSettingsScreen::handleTurnFanOnTimeDecrement(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT) {

        setSpinboxDigitFormat(m_turnFanOnTimeSpinbox,
                              Constants::MIN_RANGE,
                              Constants::MAX_RANGE,
                              -1);
        lv_spinbox_decrement(m_turnFanOnTimeSpinbox);
    }
}

// Static wrappers for LVGL callbacks
void SamplingSettingsScreen::saveButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleSaveButton(btn, event);
    }
}

void SamplingSettingsScreen::hourIncrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleHourIncrement(btn, event);
    }
}

void SamplingSettingsScreen::hourDecrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleHourDecrement(btn, event);
    }
}

void SamplingSettingsScreen::minuteIncrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleMinuteIncrement(btn, event);
    }
}

void SamplingSettingsScreen::minuteDecrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleMinuteDecrement(btn, event);
    }
}

void SamplingSettingsScreen::secondIncrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleSecondIncrement(btn, event);
    }
}

void SamplingSettingsScreen::secondDecrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleSecondDecrement(btn, event);
    }
}

void SamplingSettingsScreen::sampleNumberIncrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleSampleNumberIncrement(btn, event);
    }
}

void SamplingSettingsScreen::sampleNumberDecrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleSampleNumberDecrement(btn, event);
    }
}

void SamplingSettingsScreen::measureAvPeriodIncrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleMeasureAvPeriodIncrement(btn, event);
    }
}

void SamplingSettingsScreen::measureAvPeriodDecrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleMeasureAvPeriodDecrement(btn, event);
    }
}

void SamplingSettingsScreen::turnFanOnTimeIncrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleTurnFanOnTimeIncrement(btn, event);
    }
}

void SamplingSettingsScreen::turnFanOnTimeDecrementCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleTurnFanOnTimeDecrement(btn, event);
    }
}

void SamplingSettingsScreen::backButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (s_activeInstance) {
        s_activeInstance->handleBackButtonEvent(btn, event);
    }
}

void SamplingSettingsScreen::handleBackButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        m_screenManager.switchToScreen(BaseScreen::ScreenType::SETTINGS);
    }
}
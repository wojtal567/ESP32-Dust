#include "screens/mainscreen.h"

#include "managers/stylemanager.h"
#include "utils/constants.h"
#include "utils/stringConstants.h"
#include "utils/timeUtils.h"
#include "screens/screenmanager.h"

MainScreen::MainScreen(ScreenManager *screenManager)
    : BaseScreen<MainScreen>(ScreenType::MAIN), m_screenManager(screenManager)
{
}

MainScreen::~MainScreen()
{
}

void MainScreen::initialize()
{
    // settings button
    m_setButton = createButton(m_screenContainer, NULL, 16, 18, 32, 7, settingsButtonCallback);
    m_setButtonLabel = lv_label_create(m_setButton, NULL);
    lv_label_set_text(m_setButtonLabel, LV_SYMBOL_SETTINGS);
    StyleManager::applyTransparentButton(m_setButton);

    // lock button
    m_lockButton = createButton(m_screenContainer,
                                m_setButton,
                                14,
                                18,
                                95,
                                7,
                                lockButtonCallback);
    m_lockButtonLabel = lv_label_create(m_lockButton, NULL);
    lv_obj_set_style_local_text_font(m_lockButton,
                                     LV_OBJ_PART_MAIN,
                                     LV_STATE_DEFAULT,
                                     StyleManager::getMonte16LockFont());
    lv_label_set_text(m_lockButtonLabel, StringConstants::LOCK_SYMBOL);

    // wifi status
    m_wifiStatus = createLabel(m_screenContainer, NULL, 52, 7, LV_SYMBOL_WIFI, LV_COLOR_WHITE);
    m_wifiStatusWarning = createLabel(m_wifiStatus, NULL, 6, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);
    StyleManager::applyTransparentContainer(m_wifiStatusWarning);
    StyleManager::applyFont12(m_wifiStatusWarning);

    // sd card status
    m_sdStatus
        = createLabel(m_screenContainer, m_wifiStatus, 77, 7, LV_SYMBOL_SD_CARD, LV_COLOR_WHITE);
    m_sdStatusWarning
        = createLabel(m_sdStatus, m_wifiStatusWarning, 2, 6, LV_SYMBOL_CLOSE, LV_COLOR_RED);

    // date and time label
    m_dateTimeLabel = createLabel(m_screenContainer, NULL, 150, 7, "No Datetime set");

    // sample containers
    m_pm25Container = createContainer(m_screenContainer, NULL, 180, 90, 10, 30);
    lv_obj_set_click(m_pm25Container, false);
    StyleManager::applyStandardContainer(m_pm25Container);

    m_pm10Container = createContainer(m_screenContainer, NULL, 91, 62, 10, 118);
    lv_obj_set_click(m_pm10Container, false);
    StyleManager::applyStandardContainer(m_pm10Container);
    StyleManager::applyFont12(m_pm10Container);

    m_pm100Container = createContainer(m_screenContainer, m_pm10Container, 91, 62, 99, 118);
    lv_obj_set_click(m_pm100Container, false);

    m_temperatureContainer = createContainer(m_screenContainer, m_pm25Container, 122, 46, 188, 30);
    lv_obj_set_click(m_temperatureContainer, false);

    m_humidityContainer = createContainer(m_screenContainer, m_pm25Container, 122, 46, 188, 74);
    lv_obj_set_click(m_humidityContainer, false);

    // AQI
    m_aqiContainer = createContainer(m_screenContainer, m_pm10Container, 122, 62, 188, 118);
    lv_obj_set_click(m_aqiContainer, false);

    m_aqiColorBar = createContainer(m_aqiContainer, m_aqiContainer, 92, 24, 15, 25);
    lv_obj_set_style_local_bg_opa(m_aqiColorBar, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_70);
    lv_obj_set_click(m_aqiColorBar, false);

    // temp and humidity labels
    m_temperatureLabel = createLabel(m_temperatureContainer, NULL, 5, 3, "Temp");
    m_temperatureValueLabel = createLabel(m_temperatureContainer, NULL, 16, 22, "         -");
    StyleManager::applyFont20(m_temperatureValueLabel);

    m_humidityLabel = createLabel(m_humidityContainer, NULL, 5, 3, "RH");
    m_humidityValueLabel = createLabel(m_humidityContainer, NULL, 16, 22, "         -");
    StyleManager::applyFont20(m_humidityValueLabel);

    // pm labels and data
    m_pm10Label = createLabel(m_pm10Container, NULL, 5, 5, "PM 1.0 ug/m");
    m_pm10UpperIndex = createLabel(m_pm10Container, NULL, 80, 3, "3");

    m_pm25Label = createLabel(m_pm25Container, NULL, 5, 5, "PM 2.5                ug/m");
    m_pm25UpperIndex = createLabel(m_pm25Container, NULL, 167, 3, "3");
    StyleManager::applyFont12(m_pm25UpperIndex);

    m_pm100Label = createLabel(m_pm100Container, NULL, 5, 5, "PM 10 ug/m");
    m_pm100UpperIndex = createLabel(m_pm100Container, NULL, 77, 3, "3");
    StyleManager::applyFont12(m_pm100UpperIndex);

    m_pm10Data = lv_label_create(m_pm10Container, NULL);
    StyleManager::applyFont22White(m_pm10Data);
    lv_obj_set_auto_realign(m_pm10Data, true);
    lv_obj_align(m_pm10Data, NULL, LV_ALIGN_CENTER, 0, 5);
    lv_label_set_text(m_pm10Data, "-");

    m_pm25Data = lv_label_create(m_pm25Container, NULL);
    StyleManager::applyFont22White(m_pm25Data);
    lv_obj_set_auto_realign(m_pm25Data, true);
    lv_obj_align(m_pm25Data, NULL, LV_ALIGN_CENTER, 0, 10);
    lv_label_set_text(m_pm25Data, "-");

    m_pm100Data = lv_label_create(m_pm100Container, NULL);
    StyleManager::applyFont22White(m_pm100Data);
    lv_obj_set_auto_realign(m_pm100Data, true);
    lv_obj_align(m_pm100Data, NULL, LV_ALIGN_CENTER, 0, 5);
    lv_label_set_text(m_pm100Data, "-");

    // AQI labels
    m_aqiLabel = createLabel(m_aqiContainer, NULL, 5, 5, "Air Quality  PM 2.5");
    m_aqiColorBarLabel = lv_label_create(m_aqiColorBar, NULL);
    lv_obj_set_auto_realign(m_aqiColorBarLabel, true);
    lv_obj_align(m_aqiColorBarLabel, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(m_aqiColorBarLabel, "-");
    StyleManager::applyWhiteFont(m_aqiColorBarLabel);

    m_dbLed = lv_led_create(m_screenContainer, NULL);
    lv_obj_set_size(m_dbLed, 13, 13);
    lv_obj_set_pos(m_dbLed, 13, 10);
    lv_led_set_bright(m_dbLed, 200);
    lv_obj_set_style_local_bg_color(m_dbLed, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_YELLOW);
    lv_obj_set_style_local_shadow_color(m_dbLed,
                                        LV_OBJ_PART_MAIN,
                                        LV_STATE_DEFAULT,
                                        LV_COLOR_YELLOW);
    lv_obj_set_style_local_border_opa(m_dbLed, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);

    for (int i = 0; i < PARTICLE_SIZE_COUNT; i++) {
        m_dividingLines[i] = lv_line_create(m_screenContainer, NULL);
        lv_line_set_points(m_dividingLines[i], Constants::DIVIDING_LINES_POINTS[i], 2);
        StyleManager::applyStandardLine(m_dividingLines[i]);

        m_particleSizeLabel[i] = lv_label_create(m_screenContainer, NULL);
        lv_label_set_text(m_particleSizeLabel[i], StringConstants::PARTICLES_SIZE[i]);
        StyleManager::applyFont12White(m_particleSizeLabel[i]);
        lv_obj_set_pos(m_particleSizeLabel[i], Constants::LABEL_PARTICLE_SIZE_POS_X[i], 190);
    }

    for (int j = 0; j < PARTICLE_SIZE_COUNT - 1; j++) {

        m_particlesNumberContainer[j] = lv_cont_create(m_screenContainer, NULL);
        StyleManager::applyStandardContainer(m_particlesNumberContainer[j]);
        StyleManager::applyBorderlessContainer(m_particlesNumberContainer[j]);
        lv_obj_set_click(m_particlesNumberContainer[j], false);
        lv_obj_set_size(m_particlesNumberContainer[j], 47, 14);
        m_particlesNumberLabel[j] = lv_label_create(m_particlesNumberContainer[j], NULL);
        lv_obj_set_pos(m_particlesNumberContainer[j],
                       Constants::CONT_PARTICLE_NUMBER_POS_X[j],
                       215); // 20
        lv_label_set_align(m_particlesNumberLabel[j], LV_LABEL_ALIGN_CENTER);
        lv_obj_set_auto_realign(m_particlesNumberLabel[j], true);
        lv_label_set_text(m_particlesNumberLabel[j], "-");
        StyleManager::applyFont12White(m_particlesNumberLabel[j]);
    }

    m_mainLine = lv_line_create(m_screenContainer, NULL);
    lv_line_set_points(m_mainLine, Constants::MAIN_LINE_POINTS, 2);
    lv_line_set_auto_size(m_mainLine, true);
    StyleManager::applyStandardLine(m_mainLine);
}

void MainScreen::updateWiFiStatus(bool connected)
{
    lv_obj_set_hidden(m_wifiStatusWarning, connected);
}

void MainScreen::updateSDStatus(bool connected)
{
    lv_obj_set_hidden(m_sdStatusWarning, connected);
}

void MainScreen::updateDateTimeLabel(const char *dateTime)
{
    if (dateTime != nullptr && dateTime[0] != '\0') {
        lv_label_set_text(m_dateTimeLabel, dateTime);
    } else {
        lv_label_set_text(m_dateTimeLabel, m_defaultTimeOnDisplay ? "" : "No Datetime set");
        m_defaultTimeOnDisplay = !m_defaultTimeOnDisplay;
    }
}

void MainScreen::updateAqi(float pm25Aqi)
{
    float aqi = Utils::calculatePM25AQI(pm25Aqi);

    // determine the air quality category based on AQI value
    int categoryIndex = (aqi <= 50)    ? 0
                        : (aqi <= 100) ? 1
                        : (aqi <= 150) ? 2
                        : (aqi <= 200) ? 3
                        : (aqi <= 300) ? 4
                                       : 5;

    // map category index to the appropriate quality text
    static const char *qualityTexts[] = {StringConstants::AIR_QUALITY_EXCELLENT,
                                         StringConstants::AIR_QUALITY_GOOD,
                                         StringConstants::AIR_QUALITY_MODERATE,
                                         StringConstants::AIR_QUALITY_UNHEALTHY,
                                         StringConstants::AIR_QUALITY_BAD,
                                         StringConstants::AIR_QUALITY_HAZARDOUS};

    lv_label_set_text(m_aqiColorBarLabel, qualityTexts[categoryIndex]);
    lv_obj_set_style_local_bg_color(m_aqiColorBar,
                                    LV_CONT_PART_MAIN,
                                    LV_STATE_DEFAULT,
                                    AIR_QUALITY_COLORS[categoryIndex]);
}

void MainScreen::updateSensorData(float temperature,
                                  float humidity,
                                  const std::map<std::string, float> &averagedData)
{
    char buffer[32];

    auto updateLabel = [&buffer](lv_obj_t *label,
                                 const std::map<std::string, float> &data,
                                 const std::string &key) {
        auto it = data.find(key);
        if (it != data.end()) {
            itoa(static_cast<int>(it->second), buffer, 10);
            lv_label_set_text(label, buffer);
        } else {
            lv_label_set_text(label, "ERR");
        }
    };

    updateLabel(m_pm10Data, averagedData, "pm10_standard");
    updateLabel(m_pm25Data, averagedData, "pm25_standard");
    updateLabel(m_pm100Data, averagedData, "pm100_standard");

    updateLabel(m_particlesNumberLabel[0], averagedData, "particles_03um");
    updateLabel(m_particlesNumberLabel[1], averagedData, "particles_05um");
    updateLabel(m_particlesNumberLabel[2], averagedData, "particles_10um");
    updateLabel(m_particlesNumberLabel[3], averagedData, "particles_25um");
    updateLabel(m_particlesNumberLabel[4], averagedData, "particles_50um");
    updateLabel(m_particlesNumberLabel[5], averagedData, "particles_100um");

    char tempBuffer[32];
    snprintf(tempBuffer, sizeof(tempBuffer), "%.2f°C", temperature);
    lv_label_set_text(m_temperatureValueLabel, tempBuffer);

    char humiBuffer[32];
    snprintf(humiBuffer, sizeof(humiBuffer), "%.2f%%", humidity);
    lv_label_set_text(m_humidityValueLabel, humiBuffer);

    auto pm25It = averagedData.find("pm25_standard");
    if (pm25It != averagedData.end()) {
        updateAqi(pm25It->second);
    }
}

void MainScreen::updateLedStatus(bool isLastSampleSaved)
{
    lv_obj_set_style_local_bg_color(m_dbLed,
                                    LV_LED_PART_MAIN,
                                    LV_STATE_DEFAULT,
                                    isLastSampleSaved ? LV_COLOR_GREEN : LV_COLOR_RED);
    lv_obj_set_style_local_shadow_color(m_dbLed,
                                        LV_LED_PART_MAIN,
                                        LV_STATE_DEFAULT,
                                        isLastSampleSaved ? LV_COLOR_GREEN : LV_COLOR_RED);
}

void MainScreen::settingsButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto* instance = getActiveInstance()) {
        instance->handleSettingsButtonEvent(btn, event);
    }
}

void MainScreen::lockButtonCallback(lv_obj_t *btn, lv_event_t event)
{
    if (auto* instance = getActiveInstance()) {
        instance->handleLockButtonEvent(btn, event);
    }
}

void MainScreen::handleSettingsButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        m_screenManager->switchToScreen(ScreenType::SETTINGS);
    }
}

void MainScreen::handleLockButtonEvent(lv_obj_t *btn, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        m_screenManager->switchToScreen(ScreenType::LOCK);
    }
}
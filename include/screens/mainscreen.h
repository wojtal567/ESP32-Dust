#pragma once

#include <map>
#include "screens/basescreen.h"

constexpr int PARTICLE_SIZE_COUNT = 7;
constexpr lv_color_t AIR_QUALITY_COLORS[6] = {LV_COLOR_GREEN,
                                              LV_COLOR_GREEN,
                                              LV_COLOR_YELLOW,
                                              LV_COLOR_ORANGE,
                                              LV_COLOR_RED,
                                              LV_COLOR_RED};

class ScreenManager;

class MainScreen : public BaseScreen<MainScreen>
{
public:
    MainScreen(ScreenManager *screenManager);
    ~MainScreen();

    void initialize() override;
    void updateWiFiStatus(bool connected);
    void updateSDStatus(bool connected);
    void updateDateTimeLabel(const char *dateTime);
    void updateAqi(float pm25Aqi);
    void updateSensorData(float temperature,
                          float humidity,
                          const std::map<std::string, float> &averagedData);
    void updateLedStatus(bool isLastSampleSaved);

private:
    lv_obj_t *m_setButton{nullptr};
    lv_obj_t *m_setButtonLabel{nullptr};

    lv_obj_t *m_lockButton{nullptr};
    lv_obj_t *m_lockButtonLabel{nullptr};

    lv_obj_t *m_wifiStatus{nullptr};
    lv_obj_t *m_wifiStatusWarning{nullptr};

    lv_obj_t *m_sdStatus{nullptr};
    lv_obj_t *m_sdStatusWarning{nullptr};

    lv_obj_t *m_dateTimeLabel{nullptr};

    lv_obj_t *m_pm25Container{nullptr};
    lv_obj_t *m_pm10Container{nullptr};
    lv_obj_t *m_pm100Container{nullptr};
    lv_obj_t *m_aqiContainer{nullptr};

    lv_obj_t *m_temperatureContainer{nullptr};
    lv_obj_t *m_humidityContainer{nullptr};

    lv_obj_t *m_aqiColorBar{nullptr};

    lv_obj_t *m_temperatureLabel{nullptr};
    lv_obj_t *m_temperatureValueLabel{nullptr};
    lv_obj_t *m_humidityLabel{nullptr};
    lv_obj_t *m_humidityValueLabel{nullptr};

    lv_obj_t *m_pm10Label{nullptr};
    lv_obj_t *m_pm10UpperIndex{nullptr};
    lv_obj_t *m_pm25Label{nullptr};
    lv_obj_t *m_pm25UpperIndex{nullptr};
    lv_obj_t *m_pm100Label{nullptr};
    lv_obj_t *m_pm100UpperIndex{nullptr};
    lv_obj_t *m_pm10Data{nullptr};
    lv_obj_t *m_pm25Data{nullptr};
    lv_obj_t *m_pm100Data{nullptr};

    lv_obj_t *m_aqiLabel{nullptr};
    lv_obj_t *m_aqiColorBarLabel{nullptr};
    lv_obj_t *m_dbLed{nullptr};

    lv_obj_t *m_mainLine{nullptr};
    lv_obj_t *m_dividingLines[PARTICLE_SIZE_COUNT] = {nullptr};
    lv_obj_t *m_particleSizeLabel[PARTICLE_SIZE_COUNT] = {nullptr};

    lv_obj_t *m_particlesNumberContainer[PARTICLE_SIZE_COUNT - 1] = {nullptr};
    lv_obj_t *m_particlesNumberLabel[PARTICLE_SIZE_COUNT - 1] = {nullptr};

    bool m_defaultTimeOnDisplay{false};
    ScreenManager *m_screenManager;

    // Static callbacks
    static void settingsButtonCallback(lv_obj_t *btn, lv_event_t event);
    static void lockButtonCallback(lv_obj_t *btn, lv_event_t event);
    void handleSettingsButtonEvent(lv_obj_t *btn, lv_event_t event);
    void handleLockButtonEvent(lv_obj_t *btn, lv_event_t event);
};

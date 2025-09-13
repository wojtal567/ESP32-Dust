#pragma once

#include "screens/basescreen.h"

class SettingsScreen : public BaseScreen
{
public:
    SettingsScreen();
    ~SettingsScreen() override;

    void initialize() override;
private:
    lv_obj_t *m_backButton{nullptr};
    lv_obj_t *m_backButtonLabel{nullptr};
    lv_obj_t *m_screenTitleLabel{nullptr};
    lv_obj_t *m_wifiSettingsButton{nullptr};
    lv_obj_t *m_wifiSettingsButtonIcon{nullptr};
    lv_obj_t *m_wifiSettingsButtonLabel{nullptr};
    lv_obj_t *m_timeSettingsButton{nullptr};
    lv_obj_t *m_timeSettingsButtonIcon{nullptr};
    lv_obj_t *m_timeSettingsButtonLabel{nullptr};
    lv_obj_t *m_infoButton{nullptr};
    lv_obj_t *m_infoButtonIcon{nullptr};
    lv_obj_t *m_infoButtonLabel{nullptr};
    lv_obj_t *m_samplingSettingsButton{nullptr};
    lv_obj_t *m_samplingSettingsButtonIcon{nullptr};
    lv_obj_t *m_samplingSettingsButtonLabel{nullptr};
};
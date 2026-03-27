#pragma once

#include "screens/basescreen.h"

class LoadingScreen : public BaseScreen<LoadingScreen>
{
public:
    LoadingScreen()
        : BaseScreen(ScreenType::LOADING)
    {}

    ~LoadingScreen() override = default;

    void initialize() override;
};

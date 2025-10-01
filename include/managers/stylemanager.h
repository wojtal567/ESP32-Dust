#pragma once

#include <lvgl.h>

class StyleManager
{
public:
    static void initialize();

    static bool isInitialized();

    static void applyTransparentButton(lv_obj_t *button);
    static void applyWhiteButton(lv_obj_t *button);

    static void applyTransparentContainer(lv_obj_t *container);
    static void applyStandardContainer(lv_obj_t *container);
    static void applyBorderlessContainer(lv_obj_t *container);

    static void applyWhiteFont(lv_obj_t *label);
    static void applyFont12(lv_obj_t *label);
    static void applyFont16(lv_obj_t *label);
    static void applyFont20(lv_obj_t *label);
    static void applyFont22(lv_obj_t *label);
    static void applyHugeFont(lv_obj_t *label);
    static void applyLockFont(lv_obj_t *label);

    static void applyFont12White(lv_obj_t *label);
    static void applyFont16White(lv_obj_t *label);
    static void applyFont20White(lv_obj_t *label);
    static void applyFont22White(lv_obj_t *label);

    static void applyStandardLine(lv_obj_t *line);

    // Font access methods
    static const lv_font_t *getHugeSymbolsFont();
    static const lv_font_t *getMonte16LockFont();

    static void cleanup();

private:
    static bool m_initialized;

    static lv_style_t m_transparentBg;
    static lv_style_t m_borderless;
    static lv_style_t m_whiteFont;
    static lv_style_t m_font12;
    static lv_style_t m_font16;
    static lv_style_t m_font20;
    static lv_style_t m_font22;
    static lv_style_t m_hugeFont;
    static lv_style_t m_lockFont;
    static lv_style_t m_container;
    static lv_style_t m_whiteButton;
    static lv_style_t m_line;

    static void initializeStyles();
};

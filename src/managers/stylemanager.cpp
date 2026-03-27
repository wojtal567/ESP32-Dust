#include "managers/stylemanager.h"

// Forward declarations of fonts
extern lv_font_t hugeSymbolsFont48;
extern lv_font_t monte16lock;

// Static member initialization
bool StyleManager::m_initialized = false;

// Style objects
lv_style_t StyleManager::m_transparentBg;
lv_style_t StyleManager::m_borderless;
lv_style_t StyleManager::m_whiteFont;
lv_style_t StyleManager::m_font12;
lv_style_t StyleManager::m_font16;
lv_style_t StyleManager::m_font20;
lv_style_t StyleManager::m_font22;
lv_style_t StyleManager::m_hugeFont;
lv_style_t StyleManager::m_lockFont;
lv_style_t StyleManager::m_container;
lv_style_t StyleManager::m_whiteButton;
lv_style_t StyleManager::m_line;

void StyleManager::initialize()
{
    if (m_initialized) {
        return;
    }

    initializeStyles();
    m_initialized = true;
}

bool StyleManager::isInitialized()
{
    return m_initialized;
}

void StyleManager::initializeStyles()
{
    // Initialize transparent background style
    lv_style_init(&m_transparentBg);
    lv_style_set_bg_opa(&m_transparentBg, LV_STATE_DEFAULT, LV_OPA_0);

    // Initialize borderless style
    lv_style_init(&m_borderless);
    lv_style_set_border_width(&m_borderless, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&m_borderless, LV_STATE_DEFAULT, 0);

    // Initialize white font style
    lv_style_init(&m_whiteFont);
    lv_style_set_text_color(&m_whiteFont, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    // Initialize font styles
    lv_style_init(&m_font12);
    lv_style_set_text_font(&m_font12, LV_STATE_DEFAULT, &lv_font_montserrat_12);

    lv_style_init(&m_font16);
    lv_style_set_text_font(&m_font16, LV_STATE_DEFAULT, &lv_font_montserrat_16);

    lv_style_init(&m_font20);
    lv_style_set_text_font(&m_font20, LV_STATE_DEFAULT, &lv_font_montserrat_20);

    lv_style_init(&m_font22);
    lv_style_set_text_font(&m_font22, LV_STATE_DEFAULT, &lv_font_montserrat_22);

    lv_style_init(&m_hugeFont);
    lv_style_set_text_font(&m_hugeFont, LV_STATE_DEFAULT, &hugeSymbolsFont48);

    lv_style_init(&m_lockFont);
    lv_style_set_text_font(&m_lockFont, LV_STATE_DEFAULT, &monte16lock);

    // Initialize container style
    lv_style_init(&m_container);
    lv_style_set_bg_opa(&m_container, LV_STATE_DEFAULT, LV_OPA_0);
    lv_style_set_border_color(&m_container, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&m_container, LV_STATE_DEFAULT, 0);

    // Initialize white button style
    lv_style_init(&m_whiteButton);
    lv_style_set_bg_color(&m_whiteButton, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_radius(&m_whiteButton, LV_STATE_DEFAULT, 10);
    lv_style_set_border_width(&m_whiteButton, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&m_whiteButton, LV_STATE_DEFAULT, 0);
    lv_style_set_text_color(&m_whiteButton, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    // Initialize line style
    lv_style_init(&m_line);
    lv_style_set_line_width(&m_line, LV_STATE_DEFAULT, 2);
    lv_style_set_line_color(&m_line, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_line_rounded(&m_line, LV_STATE_DEFAULT, false);
}

void StyleManager::applyTransparentButton(lv_obj_t *button)
{
    if (!m_initialized || !button)
        return;

    lv_obj_add_style(button, LV_OBJ_PART_MAIN, &m_transparentBg);
    lv_obj_add_style(button, LV_OBJ_PART_MAIN, &m_borderless);
    lv_obj_add_style(button, LV_OBJ_PART_MAIN, &m_whiteFont);
}

void StyleManager::applyWhiteButton(lv_obj_t *button)
{
    if (!m_initialized || !button)
        return;

    lv_obj_add_style(button, LV_OBJ_PART_MAIN, &m_whiteButton);
    lv_obj_add_style(button, LV_OBJ_PART_MAIN, &m_borderless);
}

void StyleManager::applyTransparentContainer(lv_obj_t *container)
{
    if (!m_initialized || !container)
        return;

    lv_obj_add_style(container, LV_OBJ_PART_MAIN, &m_transparentBg);
}

void StyleManager::applyStandardContainer(lv_obj_t *container)
{
    if (!m_initialized || !container)
        return;

    lv_obj_add_style(container, LV_OBJ_PART_MAIN, &m_transparentBg);
    lv_obj_add_style(container, LV_OBJ_PART_MAIN, &m_container);
}

void StyleManager::applyBorderlessContainer(lv_obj_t *container)
{
    if (!m_initialized || !container)
        return;

    lv_obj_add_style(container, LV_OBJ_PART_MAIN, &m_borderless);
}

void StyleManager::applyWhiteFont(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_whiteFont);
}

void StyleManager::applyFont12(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_font12);
}

void StyleManager::applyFont16(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_font16);
}

void StyleManager::applyFont20(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_font20);
}

void StyleManager::applyFont22(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_font22);
}

void StyleManager::applyHugeFont(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_hugeFont);
}

void StyleManager::applyFont12White(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_font12);
    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_whiteFont);
}

void StyleManager::applyFont16White(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_font16);
    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_whiteFont);
}

void StyleManager::applyFont20White(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_font20);
    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_whiteFont);
}

void StyleManager::applyFont22White(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_font22);
    lv_obj_add_style(label, LV_LABEL_PART_MAIN, &m_whiteFont);
}

void StyleManager::applyLockFont(lv_obj_t *label)
{
    if (!m_initialized || !label)
        return;

    lv_obj_add_style(label, LV_OBJ_PART_MAIN, &m_lockFont);
}

void StyleManager::applyStandardLine(lv_obj_t *line)
{
    if (!m_initialized || !line)
        return;

    lv_obj_add_style(line, LV_LINE_PART_MAIN, &m_line);
}

const lv_font_t *StyleManager::getHugeSymbolsFont()
{
    return &hugeSymbolsFont48;
}

const lv_font_t *StyleManager::getMonte16LockFont()
{
    return &monte16lock;
}

void StyleManager::cleanup()
{
    if (!m_initialized) {
        return;
    }

    // Reset styles if needed (LVGL handles most cleanup automatically)
    m_initialized = false;
}

#include "lvgl.h"

/*******************************************************************************
 * Size: 48 px
 * Bpp: 1
 * Opts: 
 ******************************************************************************/

#ifndef HUGESYMBOLSFONT48
#define HUGESYMBOLSFONT48 1
#endif

#if HUGESYMBOLSFONT48

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t gylph_bitmap[] = {
    /* U+F017 "" */
    0x0, 0x0, 0x3f, 0xf8, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xfe, 0x0, 0x0, 0x0, 0x1f, 0xff, 0xff,
    0x0, 0x0, 0x0, 0xff, 0xff, 0xff, 0x80, 0x0,
    0x3, 0xff, 0xff, 0xff, 0x80, 0x0, 0x1f, 0xff,
    0xff, 0xff, 0xc0, 0x0, 0x7f, 0xff, 0xff, 0xff,
    0xc0, 0x1, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xe0,
    0x3f, 0xff, 0xc0, 0x3f, 0xff, 0xc0, 0x7f, 0xff,
    0x80, 0xff, 0xff, 0x80, 0xff, 0xff, 0x81, 0xff,
    0xff, 0x1, 0xff, 0xff, 0x7, 0xff, 0xfe, 0x3,
    0xff, 0xff, 0xf, 0xff, 0xfc, 0x7, 0xff, 0xfe,
    0x3f, 0xff, 0xf8, 0xf, 0xff, 0xfe, 0x7f, 0xff,
    0xf0, 0x1f, 0xff, 0xfc, 0xff, 0xff, 0xe0, 0x3f,
    0xff, 0xfb, 0xff, 0xff, 0xc0, 0x7f, 0xff, 0xff,
    0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x1, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3, 0xff,
    0xff, 0xff, 0xff, 0xfc, 0x7, 0xff, 0xff, 0xff,
    0xff, 0xf8, 0x7, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x7, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x7, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x3, 0xff, 0xff, 0xff,
    0xff, 0xc0, 0x3, 0xff, 0xf7, 0xff, 0xff, 0xe0,
    0x7, 0xff, 0xcf, 0xff, 0xff, 0xe0, 0xf, 0xff,
    0x9f, 0xff, 0xff, 0xf0, 0x3f, 0xff, 0x3f, 0xff,
    0xff, 0xf0, 0xff, 0xfe, 0x3f, 0xff, 0xff, 0xf9,
    0xff, 0xf8, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0xff, 0xff, 0xff, 0xff,
    0xfe, 0x0, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0,
    0xff, 0xff, 0xff, 0xff, 0xe0, 0x0, 0xff, 0xff,
    0xff, 0xff, 0x80, 0x0, 0xff, 0xff, 0xff, 0xfe,
    0x0, 0x0, 0xff, 0xff, 0xff, 0xf8, 0x0, 0x0,
    0xff, 0xff, 0xff, 0xe0, 0x0, 0x0, 0x7f, 0xff,
    0xff, 0x0, 0x0, 0x0, 0x3f, 0xff, 0xf8, 0x0,
    0x0, 0x0, 0x1f, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0x0, 0x40, 0x0, 0x0, 0x0,

    /* U+F05A "" */
    0x0, 0x0, 0x3f, 0xf0, 0x0, 0x0, 0x0, 0x7,
    0xff, 0xf8, 0x0, 0x0, 0x0, 0xff, 0xff, 0xfc,
    0x0, 0x0, 0x7, 0xff, 0xff, 0xf8, 0x0, 0x0,
    0x7f, 0xff, 0xff, 0xf8, 0x0, 0x3, 0xff, 0xff,
    0xff, 0xf0, 0x0, 0x1f, 0xff, 0xff, 0xff, 0xe0,
    0x0, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x7, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x3f, 0xff, 0xff, 0xff,
    0xff, 0x0, 0xff, 0xff, 0x87, 0xff, 0xfc, 0x7,
    0xff, 0xfc, 0xf, 0xff, 0xf8, 0x3f, 0xff, 0xf0,
    0x1f, 0xff, 0xf0, 0xff, 0xff, 0x80, 0x7f, 0xff,
    0xc3, 0xff, 0xfe, 0x1, 0xff, 0xff, 0x1f, 0xff,
    0xfc, 0x7, 0xff, 0xfe, 0x7f, 0xff, 0xf0, 0x3f,
    0xff, 0xf9, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xef,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff,
    0xff, 0xff, 0xff, 0x0, 0xff, 0xff, 0xff, 0xff,
    0xfc, 0x3, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x3f, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x3, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xf, 0xff,
    0xff, 0x7f, 0xff, 0xf0, 0x3f, 0xff, 0xf9, 0xff,
    0xff, 0xc0, 0xff, 0xff, 0xe7, 0xff, 0xff, 0x3,
    0xff, 0xff, 0x8f, 0xff, 0xf0, 0x3, 0xff, 0xfc,
    0x3f, 0xff, 0xc0, 0xf, 0xff, 0xf0, 0xff, 0xff,
    0x0, 0x3f, 0xff, 0xc1, 0xff, 0xfc, 0x0, 0xff,
    0xfe, 0x3, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xf,
    0xff, 0xff, 0xff, 0xff, 0xc0, 0x1f, 0xff, 0xff,
    0xff, 0xfe, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xf0,
    0x0, 0x7f, 0xff, 0xff, 0xff, 0x80, 0x0, 0xff,
    0xff, 0xff, 0xfc, 0x0, 0x1, 0xff, 0xff, 0xff,
    0xe0, 0x0, 0x1, 0xff, 0xff, 0xfe, 0x0, 0x0,
    0x3, 0xff, 0xff, 0xf0, 0x0, 0x0, 0x1, 0xff,
    0xfe, 0x0, 0x0, 0x0, 0x0, 0xff, 0xc0, 0x0,
    0x0,

    /* U+F085 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x38, 0x70, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x7, 0xc7, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x3f, 0xff, 0x0, 0x0,
    0xf, 0xf8, 0x0, 0x1, 0xff, 0xf8, 0x0, 0x0,
    0xff, 0xe0, 0x0, 0x7, 0xff, 0x80, 0x0, 0x7,
    0xff, 0x0, 0x0, 0x7f, 0xfe, 0x0, 0x0, 0x3f,
    0xf8, 0x0, 0x7, 0xf7, 0xf0, 0x0, 0x1, 0xff,
    0xc0, 0x0, 0xfe, 0xf, 0xf0, 0x0, 0x1f, 0xff,
    0x0, 0xf, 0xe0, 0x7f, 0x81, 0x83, 0xff, 0xfe,
    0xc, 0x7f, 0x1, 0xfc, 0x1f, 0x7f, 0xff, 0xfd,
    0xf3, 0xf8, 0xf, 0xe1, 0xff, 0xff, 0xff, 0xff,
    0xcf, 0xe0, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff,
    0x1f, 0x7, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xf8,
    0x7f, 0xfe, 0xf, 0xff, 0xff, 0xff, 0xff, 0xe1,
    0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x1f,
    0xff, 0x87, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff,
    0xfc, 0x3f, 0xff, 0xf0, 0x7f, 0xff, 0xe7, 0xc7,
    0xc0, 0xff, 0xfe, 0x1, 0xff, 0xfe, 0xe, 0x1c,
    0x1, 0xff, 0xe0, 0x7, 0xff, 0xc0, 0x0, 0x0,
    0x7, 0xfe, 0x0, 0x1f, 0xfc, 0x0, 0x0, 0x0,
    0x3f, 0xf0, 0x0, 0x7f, 0xe0, 0x0, 0x0, 0x1,
    0xff, 0x80, 0x3, 0xff, 0x0, 0x0, 0x0, 0xf,
    0xf8, 0x0, 0x1f, 0xf8, 0x0, 0x0, 0x0, 0x7f,
    0xc0, 0x0, 0xff, 0xc0, 0x0, 0x0, 0x3, 0xff,
    0x0, 0x7, 0xfe, 0x0, 0x0, 0x0, 0x1f, 0xf8,
    0x0, 0x7f, 0xf0, 0x0, 0x0, 0x3, 0xff, 0xe0,
    0x3, 0xff, 0xe0, 0x0, 0x0, 0x3f, 0xff, 0x0,
    0x3f, 0xff, 0x83, 0x87, 0x3, 0xff, 0xfe, 0x3,
    0xff, 0xfe, 0x7c, 0x7c, 0x1f, 0xff, 0xfc, 0x7f,
    0xff, 0xf3, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff,
    0xff, 0x1f, 0xff, 0x3, 0xff, 0xff, 0xff, 0xff,
    0xf8, 0xff, 0xf8, 0xf, 0xff, 0xff, 0xff, 0xff,
    0x87, 0xff, 0xe0, 0x7f, 0xff, 0xff, 0xff, 0xfc,
    0x7f, 0xff, 0x1, 0xff, 0xff, 0xff, 0xff, 0xcf,
    0xe0, 0xff, 0x7, 0xcf, 0xff, 0xfe, 0x7c, 0xfe,
    0x7, 0xf8, 0x18, 0x3f, 0xff, 0xe0, 0xc7, 0xf0,
    0x1f, 0xc0, 0x0, 0x3f, 0xf8, 0x0, 0x3f, 0x80,
    0xfe, 0x0, 0x1, 0xff, 0xc0, 0x0, 0xfe, 0x7,
    0xf0, 0x0, 0xf, 0xfe, 0x0, 0x1, 0xf0, 0x7c,
    0x0, 0x0, 0x7f, 0xf0, 0x0, 0x7, 0xff, 0xe0,
    0x0, 0x3, 0xff, 0x80, 0x0, 0x1f, 0xfe, 0x0,
    0x0, 0xf, 0xf8, 0x0, 0x1, 0xff, 0xf8, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xf, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x7c, 0x7c, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xe1, 0xc0,

    /* U+F1EB "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x1f, 0xff, 0x80, 0x0, 0x0, 0x0,
    0x0, 0x3f, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0x1f, 0xff, 0xff, 0xff, 0x80, 0x0, 0x0, 0x7,
    0xff, 0xff, 0xff, 0xfe, 0x0, 0x0, 0x1, 0xff,
    0xff, 0xff, 0xff, 0xf8, 0x0, 0x0, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xe0, 0x0, 0x1f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x7, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0x0, 0xff, 0xff, 0x80, 0x0,
    0x1f, 0xff, 0xf0, 0x1f, 0xff, 0xc0, 0x0, 0x0,
    0x3f, 0xff, 0x83, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0x7f, 0xfc, 0xff, 0xf8, 0x0, 0x0, 0x0, 0x1,
    0xff, 0xef, 0xff, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xff, 0xff, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x3f,
    0xf7, 0xf8, 0x0, 0x0, 0x0, 0x0, 0x1, 0xfe,
    0x3e, 0x0, 0x0, 0xff, 0xf0, 0x0, 0x7, 0xc1,
    0xc0, 0x0, 0xff, 0xff, 0xf0, 0x0, 0x38, 0x0,
    0x0, 0x3f, 0xff, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x3,
    0xff, 0xff, 0xff, 0xfc, 0x0, 0x0, 0x0, 0xff,
    0xff, 0xff, 0xff, 0xf0, 0x0, 0x0, 0x1f, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x0, 0x3, 0xff, 0xf8,
    0x1, 0xff, 0xf8, 0x0, 0x0, 0x1f, 0xfc, 0x0,
    0x3, 0xff, 0x80, 0x0, 0x0, 0xff, 0x0, 0x0,
    0xf, 0xf0, 0x0, 0x0, 0x7, 0xc0, 0x0, 0x0,
    0x3e, 0x0, 0x0, 0x0, 0x38, 0x0, 0x0, 0x1,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0xf8, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x3f, 0xc0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7, 0xfe, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xff, 0xe0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xf, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf,
    0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7f,
    0xe0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xfc,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x0,
    0x0, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 768, .box_w = 47, .box_h = 47, .ofs_x = 1, .ofs_y = -6},
    {.bitmap_index = 277, .adv_w = 768, .box_w = 46, .box_h = 46, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 542, .adv_w = 960, .box_w = 61, .box_h = 48, .ofs_x = 0, .ofs_y = -5},
    {.bitmap_index = 908, .adv_w = 960, .box_w = 60, .box_h = 43, .ofs_x = 0, .ofs_y = -3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x43, 0x6e, 0x1d4
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61463, .range_length = 469, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 4, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

/*Store all the custom data of the font*/
static lv_font_fmt_txt_dsc_t font_dsc = {
    .glyph_bitmap = gylph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
lv_font_t hugeSymbolsFont48 = {
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 49,          /*The maximum line height required by the font*/
    .base_line = 6,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0)
    .underline_position = -17,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if HUGESYMBOLSFONT48*/


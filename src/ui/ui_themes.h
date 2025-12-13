#ifndef UI_THEMES_H
#define UI_THEMES_H

#include <lvgl.h>

// Day mode color palette
#define DAY_BG_COLOR        lv_color_hex(0xFFFFFF)
#define DAY_PRIMARY_COLOR   lv_color_hex(0x2196F3)
#define DAY_SECONDARY_COLOR lv_color_hex(0x03A9F4)
#define DAY_TEXT_COLOR      lv_color_hex(0x000000)
#define DAY_ACCENT_COLOR    lv_color_hex(0xFF9800)

// Night mode color palette
#define NIGHT_BG_COLOR        lv_color_hex(0x000000)
#define NIGHT_PRIMARY_COLOR   lv_color_hex(0x8B0000)
#define NIGHT_SECONDARY_COLOR lv_color_hex(0xFF6347)
#define NIGHT_TEXT_COLOR      lv_color_hex(0xFF8C00)
#define NIGHT_ACCENT_COLOR    lv_color_hex(0xFFAA00)

void applyDayTheme();
void applyNightTheme();

#endif

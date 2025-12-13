#include "ui_themes.h"

static lv_style_t day_bg_style;
static lv_style_t night_bg_style;
static bool styles_initialized = false;

void initStyles() {
    if (styles_initialized) return;
    
    // Day mode styles
    lv_style_init(&day_bg_style);
    lv_style_set_bg_color(&day_bg_style, DAY_BG_COLOR);
    lv_style_set_text_color(&day_bg_style, DAY_TEXT_COLOR);
    
    // Night mode styles
    lv_style_init(&night_bg_style);
    lv_style_set_bg_color(&night_bg_style, NIGHT_BG_COLOR);
    lv_style_set_text_color(&night_bg_style, NIGHT_TEXT_COLOR);
    
    styles_initialized = true;
}

void applyDayTheme() {
    initStyles();
    
    // Get default display and set screen background
    lv_obj_t* scr = lv_scr_act();
    if (scr) {
        lv_obj_set_style_bg_color(scr, DAY_BG_COLOR, 0);
        lv_obj_set_style_text_color(scr, DAY_TEXT_COLOR, 0);
        
        // Update all children recursively
        uint32_t child_cnt = lv_obj_get_child_cnt(scr);
        for (uint32_t i = 0; i < child_cnt; i++) {
            lv_obj_t* child = lv_obj_get_child(scr, i);
            if (child) {
                // Apply day theme colors to buttons
                if (lv_obj_check_type(child, &lv_btn_class)) {
                    lv_obj_set_style_bg_color(child, DAY_PRIMARY_COLOR, 0);
                    lv_obj_set_style_text_color(child, DAY_BG_COLOR, 0);
                }
                // Apply to labels
                else if (lv_obj_check_type(child, &lv_label_class)) {
                    lv_obj_set_style_text_color(child, DAY_TEXT_COLOR, 0);
                }
            }
        }
    }
    
    Serial.println("Day theme applied");
}

void applyNightTheme() {
    initStyles();
    
    // Get default display and set screen background
    lv_obj_t* scr = lv_scr_act();
    if (scr) {
        lv_obj_set_style_bg_color(scr, NIGHT_BG_COLOR, 0);
        lv_obj_set_style_text_color(scr, NIGHT_TEXT_COLOR, 0);
        
        // Update all children recursively
        uint32_t child_cnt = lv_obj_get_child_cnt(scr);
        for (uint32_t i = 0; i < child_cnt; i++) {
            lv_obj_t* child = lv_obj_get_child(scr, i);
            if (child) {
                // Apply night theme colors to buttons
                if (lv_obj_check_type(child, &lv_btn_class)) {
                    lv_obj_set_style_bg_color(child, NIGHT_PRIMARY_COLOR, 0);
                    lv_obj_set_style_text_color(child, NIGHT_TEXT_COLOR, 0);
                }
                // Apply to labels
                else if (lv_obj_check_type(child, &lv_label_class)) {
                    lv_obj_set_style_text_color(child, NIGHT_TEXT_COLOR, 0);
                }
            }
        }
    }
    
    Serial.println("Night theme applied");
}

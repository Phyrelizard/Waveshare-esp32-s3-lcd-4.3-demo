#include "ui_message.h"

lv_obj_t* message_overlay = NULL;
static lv_obj_t* message_container = NULL;
static lv_obj_t* message_title_label = NULL;
static lv_obj_t* message_text_label = NULL;
static lv_obj_t* message_dismiss_btn = NULL;

extern void lvgl_port_lock(int timeout_ms);
extern void lvgl_port_unlock(void);

static void dismiss_btn_event_cb(lv_event_t* e) {
    hideMessage();
}

void initMessageUI() {
    lvgl_port_lock(-1);
    
    // Create overlay (initially hidden)
    message_overlay = lv_obj_create(lv_layer_top());
    lv_obj_set_size(message_overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(message_overlay, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(message_overlay, LV_OPA_50, 0);
    lv_obj_set_style_border_width(message_overlay, 0, 0);
    lv_obj_add_flag(message_overlay, LV_OBJ_FLAG_HIDDEN);
    
    // Create message container
    message_container = lv_obj_create(message_overlay);
    lv_obj_set_size(message_container, 600, 300);
    lv_obj_center(message_container);
    lv_obj_set_style_bg_color(message_container, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_radius(message_container, 10, 0);
    
    // Create title label
    message_title_label = lv_label_create(message_container);
    lv_obj_set_width(message_title_label, LV_PCT(90));
    lv_label_set_long_mode(message_title_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(message_title_label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(message_title_label, lv_color_hex(0x000000), 0);
    lv_obj_align(message_title_label, LV_ALIGN_TOP_MID, 0, 20);
    lv_label_set_text(message_title_label, "");
    
    // Create text label
    message_text_label = lv_label_create(message_container);
    lv_obj_set_width(message_text_label, LV_PCT(90));
    lv_label_set_long_mode(message_text_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_font(message_text_label, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(message_text_label, lv_color_hex(0x333333), 0);
    lv_obj_align(message_text_label, LV_ALIGN_TOP_MID, 0, 70);
    lv_label_set_text(message_text_label, "");
    
    // Create dismiss button
    message_dismiss_btn = lv_btn_create(message_container);
    lv_obj_set_size(message_dismiss_btn, 120, 50);
    lv_obj_align(message_dismiss_btn, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_t* btn_label = lv_label_create(message_dismiss_btn);
    lv_label_set_text(btn_label, "Dismiss");
    lv_obj_center(btn_label);
    lv_obj_add_event_cb(message_dismiss_btn, dismiss_btn_event_cb, LV_EVENT_CLICKED, NULL);
    
    lvgl_port_unlock();
}

void showMessage(const String& text, const String& title, MessagePriority priority) {
    if (message_overlay == NULL) {
        initMessageUI();
    }
    
    lvgl_port_lock(-1);
    
    // Set title
    if (title.length() > 0) {
        lv_label_set_text(message_title_label, title.c_str());
    } else {
        switch (priority) {
            case PRIORITY_URGENT:
                lv_label_set_text(message_title_label, "URGENT");
                lv_obj_set_style_text_color(message_title_label, lv_color_hex(0xFF0000), 0);
                break;
            case PRIORITY_IMPORTANT:
                lv_label_set_text(message_title_label, "Important");
                lv_obj_set_style_text_color(message_title_label, lv_color_hex(0xFF9800), 0);
                break;
            default:
                lv_label_set_text(message_title_label, "Message");
                lv_obj_set_style_text_color(message_title_label, lv_color_hex(0x2196F3), 0);
                break;
        }
    }
    
    // Set text
    lv_label_set_text(message_text_label, text.c_str());
    
    // Adjust container color based on priority
    switch (priority) {
        case PRIORITY_URGENT:
            lv_obj_set_style_bg_color(message_container, lv_color_hex(0xFFEBEE), 0);
            lv_obj_set_style_bg_opa(message_overlay, LV_OPA_80, 0);
            break;
        case PRIORITY_IMPORTANT:
            lv_obj_set_style_bg_color(message_container, lv_color_hex(0xFFF3E0), 0);
            lv_obj_set_style_bg_opa(message_overlay, LV_OPA_70, 0);
            break;
        default:
            lv_obj_set_style_bg_color(message_container, lv_color_hex(0xFFFFFF), 0);
            lv_obj_set_style_bg_opa(message_overlay, LV_OPA_50, 0);
            break;
    }
    
    // Show overlay
    lv_obj_clear_flag(message_overlay, LV_OBJ_FLAG_HIDDEN);
    
    lvgl_port_unlock();
}

void hideMessage() {
    if (message_overlay == NULL) return;
    
    lvgl_port_lock(-1);
    lv_obj_add_flag(message_overlay, LV_OBJ_FLAG_HIDDEN);
    lvgl_port_unlock();
}

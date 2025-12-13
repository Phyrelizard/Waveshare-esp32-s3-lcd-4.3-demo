#include "ui_photo_frame.h"
#include <LittleFS.h>

lv_obj_t* photo_frame_screen = NULL;
lv_obj_t* photo_frame_image = NULL;
lv_obj_t* photo_frame_counter_label = NULL;
lv_obj_t* photo_frame_clock_label = NULL;

static lv_obj_t* controls_container = NULL;
static lv_obj_t* btn_prev = NULL;
static lv_obj_t* btn_play_pause = NULL;
static lv_obj_t* btn_next = NULL;
static lv_obj_t* btn_exit = NULL;

extern void lvgl_port_lock(int timeout_ms);
extern void lvgl_port_unlock(void);

// Event handlers
static void btn_prev_event_cb(lv_event_t* e) {
    // Will be connected to PhotoFrame::previous()
    Serial.println("Previous button clicked");
}

static void btn_next_event_cb(lv_event_t* e) {
    // Will be connected to PhotoFrame::next()
    Serial.println("Next button clicked");
}

static void btn_play_pause_event_cb(lv_event_t* e) {
    // Will be connected to PhotoFrame::pause/resume()
    Serial.println("Play/Pause button clicked");
}

static void btn_exit_event_cb(lv_event_t* e) {
    // Will be connected to PhotoFrame::stop()
    Serial.println("Exit button clicked");
}

void initPhotoFrameUI() {
    lvgl_port_lock(-1);
    
    // Create photo frame screen
    photo_frame_screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(photo_frame_screen, lv_color_hex(0x000000), 0);
    
    // Create image container
    photo_frame_image = lv_img_create(photo_frame_screen);
    lv_obj_set_size(photo_frame_image, 800, 480);
    lv_obj_center(photo_frame_image);
    
    // Create counter label (top-right)
    photo_frame_counter_label = lv_label_create(photo_frame_screen);
    lv_obj_set_style_text_color(photo_frame_counter_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_color(photo_frame_counter_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(photo_frame_counter_label, LV_OPA_70, 0);
    lv_obj_set_style_pad_all(photo_frame_counter_label, 10, 0);
    lv_label_set_text(photo_frame_counter_label, "0/0");
    lv_obj_align(photo_frame_counter_label, LV_ALIGN_TOP_RIGHT, -10, 10);
    
    // Create clock label (top-left)
    photo_frame_clock_label = lv_label_create(photo_frame_screen);
    lv_obj_set_style_text_color(photo_frame_clock_label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_color(photo_frame_clock_label, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(photo_frame_clock_label, LV_OPA_70, 0);
    lv_obj_set_style_pad_all(photo_frame_clock_label, 10, 0);
    lv_label_set_text(photo_frame_clock_label, "00:00");
    lv_obj_align(photo_frame_clock_label, LV_ALIGN_TOP_LEFT, 10, 10);
    
    // Create controls container (bottom)
    controls_container = lv_obj_create(photo_frame_screen);
    lv_obj_set_size(controls_container, 400, 60);
    lv_obj_set_style_bg_color(controls_container, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(controls_container, LV_OPA_70, 0);
    lv_obj_set_style_border_width(controls_container, 0, 0);
    lv_obj_align(controls_container, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_flex_flow(controls_container, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(controls_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    
    // Create control buttons
    btn_prev = lv_btn_create(controls_container);
    lv_obj_set_size(btn_prev, 60, 40);
    lv_obj_t* label_prev = lv_label_create(btn_prev);
    lv_label_set_text(label_prev, LV_SYMBOL_LEFT);
    lv_obj_center(label_prev);
    lv_obj_add_event_cb(btn_prev, btn_prev_event_cb, LV_EVENT_CLICKED, NULL);
    
    btn_play_pause = lv_btn_create(controls_container);
    lv_obj_set_size(btn_play_pause, 60, 40);
    lv_obj_t* label_play = lv_label_create(btn_play_pause);
    lv_label_set_text(label_play, LV_SYMBOL_PAUSE);
    lv_obj_center(label_play);
    lv_obj_add_event_cb(btn_play_pause, btn_play_pause_event_cb, LV_EVENT_CLICKED, NULL);
    
    btn_next = lv_btn_create(controls_container);
    lv_obj_set_size(btn_next, 60, 40);
    lv_obj_t* label_next = lv_label_create(btn_next);
    lv_label_set_text(label_next, LV_SYMBOL_RIGHT);
    lv_obj_center(label_next);
    lv_obj_add_event_cb(btn_next, btn_next_event_cb, LV_EVENT_CLICKED, NULL);
    
    btn_exit = lv_btn_create(controls_container);
    lv_obj_set_size(btn_exit, 60, 40);
    lv_obj_t* label_exit = lv_label_create(btn_exit);
    lv_label_set_text(label_exit, LV_SYMBOL_CLOSE);
    lv_obj_center(label_exit);
    lv_obj_add_event_cb(btn_exit, btn_exit_event_cb, LV_EVENT_CLICKED, NULL);
    
    lvgl_port_unlock();
}

void showPhotoFrameScreen() {
    if (photo_frame_screen == NULL) {
        initPhotoFrameUI();
    }
    
    lvgl_port_lock(-1);
    lv_scr_load(photo_frame_screen);
    lvgl_port_unlock();
}

void hidePhotoFrameScreen() {
    // Return to main screen - will need reference to ui_FirstScreen
    lvgl_port_lock(-1);
    // lv_scr_load(ui_FirstScreen);  // This will be added in main integration
    lvgl_port_unlock();
}

void showPhotoFrameImage(const String& filename) {
    if (photo_frame_image == NULL) return;
    
    lvgl_port_lock(-1);
    
    // Note: LVGL file system integration needed
    // For now, just show a placeholder
    lv_obj_set_style_bg_color(photo_frame_image, lv_color_hex(0x404040), 0);
    
    // TODO: Load actual image from LittleFS
    // String path = "/images/" + filename;
    // lv_img_set_src(photo_frame_image, path.c_str());
    
    lvgl_port_unlock();
}

void updatePhotoFrameUI(int current, int total) {
    if (photo_frame_counter_label == NULL) return;
    
    lvgl_port_lock(-1);
    
    String counter = String(current) + "/" + String(total);
    lv_label_set_text(photo_frame_counter_label, counter.c_str());
    
    // Update clock if enabled
    struct tm timeinfo;
    if (getLocalTime(&timeinfo) && photo_frame_clock_label) {
        char timeStr[10];
        strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
        lv_label_set_text(photo_frame_clock_label, timeStr);
    }
    
    lvgl_port_unlock();
}

void showPhotoFrameControls(bool show) {
    if (controls_container == NULL) return;
    
    lvgl_port_lock(-1);
    if (show) {
        lv_obj_clear_flag(controls_container, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(controls_container, LV_OBJ_FLAG_HIDDEN);
    }
    lvgl_port_unlock();
}

#ifndef UI_PHOTO_FRAME_H
#define UI_PHOTO_FRAME_H

#include <Arduino.h>
#include <lvgl.h>

void initPhotoFrameUI();
void showPhotoFrameScreen();
void hidePhotoFrameScreen();
void showPhotoFrameImage(const String& filename);
void updatePhotoFrameUI(int current, int total);
void showPhotoFrameControls(bool show);

extern lv_obj_t* photo_frame_screen;
extern lv_obj_t* photo_frame_image;
extern lv_obj_t* photo_frame_counter_label;
extern lv_obj_t* photo_frame_clock_label;

#endif

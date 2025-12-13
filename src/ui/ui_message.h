#ifndef UI_MESSAGE_H
#define UI_MESSAGE_H

#include <Arduino.h>
#include <lvgl.h>
#include "message_handler.h"

void initMessageUI();
void showMessage(const String& text, const String& title, MessagePriority priority);
void hideMessage();

extern lv_obj_t* message_overlay;

#endif

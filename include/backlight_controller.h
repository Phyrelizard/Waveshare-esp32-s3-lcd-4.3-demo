#ifndef BACKLIGHT_CONTROLLER_H
#define BACKLIGHT_CONTROLLER_H

#include <Arduino.h>
#include <Preferences.h>
#include <ESP_IOExpander_Library.h>

class BacklightController {
public:
    BacklightController(ESP_IOExpander* expander, int pin);
    void begin();
    void loop(int currentHour);
    
    void setSchedule(int onHour, int offHour);
    void getSchedule(int& onHour, int& offHour);
    
    void turnOn();
    void turnOff();
    void toggle();
    bool isOn();
    
    void onTouch();  // Call when touch is detected
    
private:
    ESP_IOExpander* _expander;
    int _pin;
    bool _isOn;
    int _onHour;
    int _offHour;
    unsigned long _lastTouchTime;
    bool _touchWakeEnabled;
    Preferences _prefs;
    
    void loadSettings();
    void saveSettings();
    void applyState();
};

#endif // BACKLIGHT_CONTROLLER_H

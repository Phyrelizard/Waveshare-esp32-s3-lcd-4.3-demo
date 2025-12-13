#ifndef NIGHT_MODE_H
#define NIGHT_MODE_H

#include <Arduino.h>
#include <lvgl.h>

class NightMode {
public:
    void begin();
    void update();  // Call periodically to check schedule
    
    bool isNightMode() { return nightModeActive; }
    void setNightMode(bool enabled);
    void setAutoMode(bool enabled) { autoModeEnabled = enabled; }
    bool isAutoMode() { return autoModeEnabled; }
    
    void setSchedule(int startHour, int endHour);
    void getSchedule(int& startHour, int& endHour);
    
    void setNightBrightness(int brightness);
    int getNightBrightness() { return nightBrightness; }
    
    void applyTheme();
    
private:
    bool nightModeActive = false;
    bool autoModeEnabled = true;
    int nightStartHour = 20;  // 8:00 PM
    int nightEndHour = 7;     // 7:00 AM
    int nightBrightness = 30; // 30%
    
    bool shouldBeNightMode();
    void transitionToNightMode();
    void transitionToDayMode();
};

extern NightMode nightMode;

#endif

#include "night_mode.h"
#include "ui/ui_themes.h"
#include <time.h>

NightMode nightMode;

extern ESP_Panel *panel;  // Defined in main.cpp

void NightMode::begin() {
    // Initialize with current state
    update();
}

void NightMode::update() {
    if (!autoModeEnabled) {
        return;
    }
    
    bool shouldBeNight = shouldBeNightMode();
    if (shouldBeNight != nightModeActive) {
        setNightMode(shouldBeNight);
    }
}

bool NightMode::shouldBeNightMode() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        return false;  // Default to day mode if time not available
    }
    
    int currentHour = timeinfo.tm_hour;
    
    // Handle schedule that crosses midnight
    if (nightStartHour > nightEndHour) {
        // e.g., 20:00 - 7:00
        return (currentHour >= nightStartHour || currentHour < nightEndHour);
    } else {
        // e.g., 0:00 - 6:00
        return (currentHour >= nightStartHour && currentHour < nightEndHour);
    }
}

void NightMode::setNightMode(bool enabled) {
    if (nightModeActive == enabled) {
        return;
    }
    
    nightModeActive = enabled;
    
    if (enabled) {
        transitionToNightMode();
    } else {
        transitionToDayMode();
    }
}

void NightMode::transitionToNightMode() {
    Serial.println("Transitioning to night mode");
    applyNightTheme();
    
    // Dim backlight if panel is available
    if (panel && panel->getLcd()) {
        panel->getLcd()->setBrightness(nightBrightness);
    }
}

void NightMode::transitionToDayMode() {
    Serial.println("Transitioning to day mode");
    applyDayTheme();
    
    // Restore full backlight
    if (panel && panel->getLcd()) {
        panel->getLcd()->setBrightness(100);
    }
}

void NightMode::setSchedule(int startHour, int endHour) {
    if (startHour >= 0 && startHour < 24) {
        nightStartHour = startHour;
    }
    if (endHour >= 0 && endHour < 24) {
        nightEndHour = endHour;
    }
}

void NightMode::getSchedule(int& startHour, int& endHour) {
    startHour = nightStartHour;
    endHour = nightEndHour;
}

void NightMode::setNightBrightness(int brightness) {
    if (brightness >= 0 && brightness <= 100) {
        nightBrightness = brightness;
        if (nightModeActive && panel && panel->getLcd()) {
            panel->getLcd()->setBrightness(brightness);
        }
    }
}

void NightMode::applyTheme() {
    if (nightModeActive) {
        applyNightTheme();
    } else {
        applyDayTheme();
    }
}

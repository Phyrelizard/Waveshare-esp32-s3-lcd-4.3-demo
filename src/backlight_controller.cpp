#include "backlight_controller.h"
#include "config.h"

BacklightController::BacklightController(ESP_IOExpander* expander, int pin) 
    : _expander(expander), _pin(pin), _isOn(true), 
      _onHour(DEFAULT_BACKLIGHT_ON_HOUR), _offHour(DEFAULT_BACKLIGHT_OFF_HOUR),
      _lastTouchTime(0), _touchWakeEnabled(true) {
}

void BacklightController::begin() {
    Serial.println("Initializing backlight controller...");
    
    // Load saved settings
    _prefs.begin(PREFS_NAMESPACE, false);
    loadSettings();
    
    // Initially turn on backlight
    turnOn();
    
    Serial.print("Backlight schedule: ON at ");
    Serial.print(_onHour);
    Serial.print(":00, OFF at ");
    Serial.print(_offHour);
    Serial.println(":00");
}

void BacklightController::loadSettings() {
    _onHour = _prefs.getInt("bl_on_hour", DEFAULT_BACKLIGHT_ON_HOUR);
    _offHour = _prefs.getInt("bl_off_hour", DEFAULT_BACKLIGHT_OFF_HOUR);
    
    // Validate values
    if (_onHour < 0 || _onHour > 23) _onHour = DEFAULT_BACKLIGHT_ON_HOUR;
    if (_offHour < 0 || _offHour > 23) _offHour = DEFAULT_BACKLIGHT_OFF_HOUR;
}

void BacklightController::saveSettings() {
    _prefs.putInt("bl_on_hour", _onHour);
    _prefs.putInt("bl_off_hour", _offHour);
}

void BacklightController::loop(int currentHour) {
    if (currentHour < 0 || currentHour > 23) return;
    
    // Check if we should automatically turn on/off based on schedule
    bool shouldBeOn = true;
    
    if (_offHour > _onHour) {
        // Normal case: e.g., ON at 7, OFF at 22
        shouldBeOn = (currentHour >= _onHour && currentHour < _offHour);
    } else {
        // Wrap around case: e.g., ON at 22, OFF at 7
        shouldBeOn = (currentHour >= _onHour || currentHour < _offHour);
    }
    
    // Apply scheduled state if no recent touch activity
    unsigned long now = millis();
    bool recentTouch = (now - _lastTouchTime) < TOUCH_WAKE_TIMEOUT;
    
    if (!recentTouch) {
        if (shouldBeOn && !_isOn) {
            Serial.println("Auto turning on backlight (scheduled time)");
            turnOn();
        } else if (!shouldBeOn && _isOn) {
            Serial.println("Auto turning off backlight (scheduled time)");
            turnOff();
        }
    }
}

void BacklightController::setSchedule(int onHour, int offHour) {
    if (onHour >= 0 && onHour <= 23 && offHour >= 0 && offHour <= 23) {
        _onHour = onHour;
        _offHour = offHour;
        saveSettings();
        
        Serial.print("Backlight schedule updated: ON at ");
        Serial.print(_onHour);
        Serial.print(":00, OFF at ");
        Serial.print(_offHour);
        Serial.println(":00");
    }
}

void BacklightController::getSchedule(int& onHour, int& offHour) {
    onHour = _onHour;
    offHour = _offHour;
}

void BacklightController::turnOn() {
    _isOn = true;
    applyState();
    Serial.println("Backlight ON");
}

void BacklightController::turnOff() {
    _isOn = false;
    applyState();
    Serial.println("Backlight OFF");
}

void BacklightController::toggle() {
    if (_isOn) {
        turnOff();
    } else {
        turnOn();
    }
}

bool BacklightController::isOn() {
    return _isOn;
}

void BacklightController::onTouch() {
    _lastTouchTime = millis();
    
    // If backlight is off and touch wake is enabled, turn it on
    if (!_isOn && _touchWakeEnabled) {
        Serial.println("Touch detected - waking display");
        turnOn();
    }
}

void BacklightController::applyState() {
    if (_expander) {
        _expander->digitalWrite(_pin, _isOn ? HIGH : LOW);
    }
}

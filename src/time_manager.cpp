#include "time_manager.h"
#include "config.h"

TimeManager::TimeManager() : _timeValid(false), _lastUpdate(0) {
    memset(&_timeinfo, 0, sizeof(struct tm));
}

void TimeManager::begin(const char* ntpServer, long gmtOffset, int daylightOffset) {
    Serial.println("Initializing time sync...");
    
    // Configure time with NTP
    configTime(gmtOffset, daylightOffset, ntpServer);
    
    // Wait for time to be set
    int attempts = 0;
    while (!getLocalTime() && attempts < 10) {
        Serial.print(".");
        delay(500);
        attempts++;
    }
    
    if (_timeValid) {
        Serial.println("\nTime synchronized!");
        Serial.println(getFormattedTime(false));
        Serial.println(getFormattedDate());
    } else {
        Serial.println("\nFailed to obtain time!");
    }
    
    _lastUpdate = millis();
}

bool TimeManager::getLocalTime() {
    time_t now;
    time(&now);
    localtime_r(&now, &_timeinfo);
    
    // Check if time is valid (year should be > 2020)
    _timeValid = (_timeinfo.tm_year + 1900) > 2020;
    return _timeValid;
}

void TimeManager::update() {
    // Update time info every second
    getLocalTime();
    
    // Resync with NTP server periodically
    unsigned long now = millis();
    if (now - _lastUpdate > NTP_UPDATE_INTERVAL) {
        syncTime();
        _lastUpdate = now;
    }
}

void TimeManager::syncTime() {
    Serial.println("Resyncing time with NTP server...");
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
}

String TimeManager::getFormattedTime(bool format24h) {
    if (!_timeValid) return "--:--:--";
    
    char buffer[20];
    if (format24h) {
        strftime(buffer, sizeof(buffer), "%H:%M:%S", &_timeinfo);
    } else {
        strftime(buffer, sizeof(buffer), "%I:%M:%S %p", &_timeinfo);
    }
    return String(buffer);
}

String TimeManager::getFormattedDate() {
    if (!_timeValid) return "--/--/----";
    
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%B %d, %Y", &_timeinfo);
    return String(buffer);
}

String TimeManager::getDayOfWeek() {
    if (!_timeValid) return "---";
    
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%A", &_timeinfo);
    return String(buffer);
}

int TimeManager::getHour() {
    return _timeValid ? _timeinfo.tm_hour : -1;
}

int TimeManager::getMinute() {
    return _timeValid ? _timeinfo.tm_min : -1;
}

int TimeManager::getSecond() {
    return _timeValid ? _timeinfo.tm_sec : -1;
}

int TimeManager::getDay() {
    return _timeValid ? _timeinfo.tm_mday : -1;
}

int TimeManager::getMonth() {
    return _timeValid ? (_timeinfo.tm_mon + 1) : -1;
}

int TimeManager::getYear() {
    return _timeValid ? (_timeinfo.tm_year + 1900) : -1;
}

bool TimeManager::isTimeValid() {
    return _timeValid;
}

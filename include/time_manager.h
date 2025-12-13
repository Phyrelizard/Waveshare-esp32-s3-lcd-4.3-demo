#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include <time.h>

class TimeManager {
public:
    TimeManager();
    void begin(const char* ntpServer, long gmtOffset, int daylightOffset);
    void update();
    
    // Getters
    String getFormattedTime(bool format24h = false);
    String getFormattedDate();
    String getDayOfWeek();
    int getHour();
    int getMinute();
    int getSecond();
    int getDay();
    int getMonth();
    int getYear();
    bool isTimeValid();
    
private:
    bool _timeValid;
    struct tm _timeinfo;
    unsigned long _lastUpdate;
    
    void syncTime();
    bool getLocalTime();
};

#endif // TIME_MANAGER_H

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include <lvgl.h>
#include "time_manager.h"
#include "weather_api.h"
#include "wifi_manager.h"

class UIManager {
public:
    UIManager();
    void begin(lv_obj_t* parent);
    void update(TimeManager& timeManager, WeatherAPI& weatherApi, WiFiManager& wifiManager);
    
private:
    // UI objects
    lv_obj_t* _mainScreen;
    lv_obj_t* _timeLabel;
    lv_obj_t* _dateLabel;
    lv_obj_t* _tempLabel;
    lv_obj_t* _weatherLabel;
    lv_obj_t* _statusBar;
    lv_obj_t* _wifiLabel;
    
    // Forecast objects
    lv_obj_t* _forecastLabels[7];
    
    void createMainUI(lv_obj_t* parent);
    void updateMainUI(TimeManager& timeManager, WeatherAPI& weatherApi, WiFiManager& wifiManager);
};

#endif // UI_MANAGER_H

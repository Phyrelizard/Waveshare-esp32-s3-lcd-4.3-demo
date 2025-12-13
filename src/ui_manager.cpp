#include "ui_manager.h"

UIManager::UIManager() {
    _mainScreen = NULL;
    _timeLabel = NULL;
    _dateLabel = NULL;
    _tempLabel = NULL;
    _weatherLabel = NULL;
    _statusBar = NULL;
    _wifiLabel = NULL;
    
    for (int i = 0; i < 7; i++) {
        _forecastLabels[i] = NULL;
    }
}

void UIManager::begin(lv_obj_t* parent) {
    createMainUI(parent);
}

void UIManager::createMainUI(lv_obj_t* parent) {
    // Create main screen
    _mainScreen = lv_obj_create(parent);
    lv_obj_set_size(_mainScreen, ESP_PANEL_LCD_H_RES, ESP_PANEL_LCD_V_RES);
    lv_obj_clear_flag(_mainScreen, LV_OBJ_FLAG_SCROLLABLE);
    
    // Status bar at top
    _statusBar = lv_obj_create(_mainScreen);
    lv_obj_set_size(_statusBar, ESP_PANEL_LCD_H_RES, 30);
    lv_obj_set_pos(_statusBar, 0, 0);
    lv_obj_set_style_bg_color(_statusBar, lv_color_hex(0x333333), 0);
    lv_obj_clear_flag(_statusBar, LV_OBJ_FLAG_SCROLLABLE);
    
    // WiFi status in status bar
    _wifiLabel = lv_label_create(_statusBar);
    lv_label_set_text(_wifiLabel, LV_SYMBOL_WIFI " --");
    lv_obj_set_style_text_color(_wifiLabel, lv_color_white(), 0);
    lv_obj_align(_wifiLabel, LV_ALIGN_RIGHT_MID, -10, 0);
    
    // Time display (large, center-top)
    _timeLabel = lv_label_create(_mainScreen);
    lv_label_set_text(_timeLabel, "--:--:--");
    lv_obj_set_style_text_font(_timeLabel, &lv_font_montserrat_48, 0);
    lv_obj_align(_timeLabel, LV_ALIGN_TOP_MID, 0, 50);
    
    // Date display
    _dateLabel = lv_label_create(_mainScreen);
    lv_label_set_text(_dateLabel, "---");
    lv_obj_set_style_text_font(_dateLabel, &lv_font_montserrat_24, 0);
    lv_obj_align(_dateLabel, LV_ALIGN_TOP_MID, 0, 110);
    
    // Temperature display (large, center)
    _tempLabel = lv_label_create(_mainScreen);
    lv_label_set_text(_tempLabel, "--°");
    lv_obj_set_style_text_font(_tempLabel, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(_tempLabel, lv_color_hex(0xFF9800), 0);
    lv_obj_align(_tempLabel, LV_ALIGN_CENTER, 0, 0);
    
    // Weather condition
    _weatherLabel = lv_label_create(_mainScreen);
    lv_label_set_text(_weatherLabel, "---");
    lv_obj_set_style_text_font(_weatherLabel, &lv_font_montserrat_20, 0);
    lv_obj_align(_weatherLabel, LV_ALIGN_CENTER, 0, 60);
    
    // Create 7-day forecast at bottom
    int forecastY = ESP_PANEL_LCD_V_RES - 80;
    int forecastSpacing = ESP_PANEL_LCD_H_RES / 7;
    
    for (int i = 0; i < 7; i++) {
        _forecastLabels[i] = lv_label_create(_mainScreen);
        lv_label_set_text(_forecastLabels[i], "---\n--°");
        lv_obj_set_style_text_font(_forecastLabels[i], &lv_font_montserrat_12, 0);
        lv_obj_set_style_text_align(_forecastLabels[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_pos(_forecastLabels[i], i * forecastSpacing + 10, forecastY);
    }
}

void UIManager::update(TimeManager& timeManager, WeatherAPI& weatherApi, WiFiManager& wifiManager) {
    updateMainUI(timeManager, weatherApi, wifiManager);
}

void UIManager::updateMainUI(TimeManager& timeManager, WeatherAPI& weatherApi, WiFiManager& wifiManager) {
    // Update time
    if (_timeLabel && timeManager.isTimeValid()) {
        String timeStr = timeManager.getFormattedTime(false);  // 12-hour format
        lv_label_set_text(_timeLabel, timeStr.c_str());
    }
    
    // Update date
    if (_dateLabel && timeManager.isTimeValid()) {
        String dateStr = timeManager.getDayOfWeek() + ", " + timeManager.getFormattedDate();
        lv_label_set_text(_dateLabel, dateStr.c_str());
    }
    
    // Update WiFi status
    if (_wifiLabel) {
        if (wifiManager.isConnected()) {
            int rssi = wifiManager.getSignalStrength();
            String wifiStr = LV_SYMBOL_WIFI " " + String(rssi) + "dBm";
            lv_label_set_text(_wifiLabel, wifiStr.c_str());
            lv_obj_set_style_text_color(_wifiLabel, lv_color_hex(0x00FF00), 0);
        } else {
            lv_label_set_text(_wifiLabel, LV_SYMBOL_WIFI " X");
            lv_obj_set_style_text_color(_wifiLabel, lv_color_hex(0xFF0000), 0);
        }
    }
    
    // Update weather
    CurrentWeather weather = weatherApi.getCurrentWeather();
    if (_tempLabel && weather.valid) {
        String tempStr = String((int)weather.temperature) + "°";
        lv_label_set_text(_tempLabel, tempStr.c_str());
    }
    
    if (_weatherLabel && weather.valid) {
        String weatherStr = weather.description;
        weatherStr += "\nH: " + String((int)weather.tempHigh) + "° L: " + String((int)weather.tempLow) + "°";
        lv_label_set_text(_weatherLabel, weatherStr.c_str());
    }
    
    // Update forecast
    for (int i = 0; i < 7; i++) {
        if (_forecastLabels[i]) {
            DailyForecast forecast = weatherApi.getForecast(i);
            if (forecast.valid) {
                String forecastStr = forecast.dayOfWeek.substring(5, 10) + "\n";
                forecastStr += String((int)forecast.tempMax) + "°\n";
                forecastStr += String((int)forecast.tempMin) + "°";
                lv_label_set_text(_forecastLabels[i], forecastStr.c_str());
            }
        }
    }
}

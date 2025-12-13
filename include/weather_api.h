#ifndef WEATHER_API_H
#define WEATHER_API_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

struct CurrentWeather {
    float temperature;
    float feelsLike;
    float tempHigh;
    float tempLow;
    int humidity;
    float windSpeed;
    int weatherCode;  // WMO Weather code
    String description;
    bool valid;
};

struct DailyForecast {
    String date;
    String dayOfWeek;
    float tempMax;
    float tempMin;
    int weatherCode;
    int precipitationProb;
    bool valid;
};

class WeatherAPI {
public:
    WeatherAPI();
    void begin(const char* latitude, const char* longitude, bool useFahrenheit = true);
    bool update();
    
    CurrentWeather getCurrentWeather();
    DailyForecast getForecast(int day);  // day: 0-6 for next 7 days
    String getWeatherIcon(int weatherCode);
    String getWeatherDescription(int weatherCode);
    
private:
    String _latitude;
    String _longitude;
    bool _useFahrenheit;
    CurrentWeather _currentWeather;
    DailyForecast _forecasts[7];
    unsigned long _lastUpdate;
    
    bool fetchWeatherData();
    void parseCurrentWeather(JsonDocument& doc);
    void parseForecast(JsonDocument& doc);
};

#endif // WEATHER_API_H

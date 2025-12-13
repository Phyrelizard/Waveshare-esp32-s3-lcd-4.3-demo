#include "weather_api.h"
#include "config.h"

WeatherAPI::WeatherAPI() : _useFahrenheit(true), _lastUpdate(0) {
    _currentWeather.valid = false;
    for (int i = 0; i < 7; i++) {
        _forecasts[i].valid = false;
    }
}

void WeatherAPI::begin(const char* latitude, const char* longitude, bool useFahrenheit) {
    _latitude = String(latitude);
    _longitude = String(longitude);
    _useFahrenheit = useFahrenheit;
    
    Serial.println("Weather API initialized");
    Serial.print("Location: ");
    Serial.print(_latitude);
    Serial.print(", ");
    Serial.println(_longitude);
}

bool WeatherAPI::update() {
    unsigned long now = millis();
    
    // Update if never updated or interval elapsed
    if (_lastUpdate == 0 || (now - _lastUpdate > WEATHER_UPDATE_INTERVAL)) {
        Serial.println("Fetching weather data...");
        bool success = fetchWeatherData();
        if (success) {
            _lastUpdate = now;
            Serial.println("Weather data updated successfully!");
        } else {
            Serial.println("Failed to fetch weather data!");
        }
        return success;
    }
    
    return _currentWeather.valid;
}

bool WeatherAPI::fetchWeatherData() {
    HTTPClient http;
    
    // Build URL with parameters
    String url = String(WEATHER_API_URL);
    url += "?latitude=" + _latitude;
    url += "&longitude=" + _longitude;
    url += "&current=temperature_2m,relative_humidity_2m,weather_code,wind_speed_10m";
    url += "&daily=weather_code,temperature_2m_max,temperature_2m_min,precipitation_probability_max";
    url += "&temperature_unit=" + String(_useFahrenheit ? "fahrenheit" : "celsius");
    url += "&wind_speed_unit=mph";
    url += "&timezone=auto";
    url += "&forecast_days=7";
    
    Serial.print("Request URL: ");
    Serial.println(url);
    
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode != HTTP_CODE_OK) {
        Serial.print("HTTP error code: ");
        Serial.println(httpCode);
        http.end();
        return false;
    }
    
    String payload = http.getString();
    http.end();
    
    // Parse JSON
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.print("JSON parse error: ");
        Serial.println(error.c_str());
        return false;
    }
    
    parseCurrentWeather(doc);
    parseForecast(doc);
    
    return _currentWeather.valid;
}

void WeatherAPI::parseCurrentWeather(JsonDocument& doc) {
    if (!doc.containsKey("current")) {
        Serial.println("No current weather data in response");
        return;
    }
    
    JsonObject current = doc["current"];
    
    _currentWeather.temperature = current["temperature_2m"] | 0.0f;
    _currentWeather.humidity = current["relative_humidity_2m"] | 0;
    _currentWeather.windSpeed = current["wind_speed_10m"] | 0.0f;
    _currentWeather.weatherCode = current["weather_code"] | 0;
    _currentWeather.description = getWeatherDescription(_currentWeather.weatherCode);
    
    // For feels like and high/low, we'll approximate from daily data
    if (doc.containsKey("daily")) {
        JsonObject daily = doc["daily"];
        if (daily.containsKey("temperature_2m_max") && daily["temperature_2m_max"].size() > 0) {
            _currentWeather.tempHigh = daily["temperature_2m_max"][0] | 0.0f;
            _currentWeather.tempLow = daily["temperature_2m_min"][0] | 0.0f;
        }
    }
    
    // Simple feels-like approximation (can be improved)
    _currentWeather.feelsLike = _currentWeather.temperature;
    
    _currentWeather.valid = true;
    
    Serial.print("Current: ");
    Serial.print(_currentWeather.temperature);
    Serial.print("°, Weather code: ");
    Serial.println(_currentWeather.weatherCode);
}

void WeatherAPI::parseForecast(JsonDocument& doc) {
    if (!doc.containsKey("daily")) {
        Serial.println("No forecast data in response");
        return;
    }
    
    JsonObject daily = doc["daily"];
    JsonArray dates = daily["time"];
    JsonArray tempMax = daily["temperature_2m_max"];
    JsonArray tempMin = daily["temperature_2m_min"];
    JsonArray weatherCodes = daily["weather_code"];
    JsonArray precipProb = daily["precipitation_probability_max"];
    
    int count = min((int)dates.size(), 7);
    
    // Day of week names
    const char* dayNames[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    
    for (int i = 0; i < count; i++) {
        _forecasts[i].date = dates[i].as<String>();
        _forecasts[i].tempMax = tempMax[i] | 0.0f;
        _forecasts[i].tempMin = tempMin[i] | 0.0f;
        _forecasts[i].weatherCode = weatherCodes[i] | 0;
        _forecasts[i].precipitationProb = precipProb[i] | 0;
        _forecasts[i].valid = true;
        
        // Parse date and calculate day of week
        // Date format: YYYY-MM-DD
        String dateStr = _forecasts[i].date;
        if (dateStr.length() >= 10) {
            int year = dateStr.substring(0, 4).toInt();
            int month = dateStr.substring(5, 7).toInt();
            int day = dateStr.substring(8, 10).toInt();
            
            // Zeller's congruence algorithm to calculate day of week
            if (month < 3) {
                month += 12;
                year--;
            }
            int q = day;
            int m = month;
            int k = year % 100;
            int j = year / 100;
            int h = (q + ((13 * (m + 1)) / 5) + k + (k / 4) + (j / 4) - (2 * j)) % 7;
            
            // Convert result (0=Saturday, 1=Sunday, ..., 6=Friday) to our format (0=Sunday)
            int dayIndex = (h + 6) % 7;
            _forecasts[i].dayOfWeek = String(dayNames[dayIndex]);
        } else {
            // Fallback to showing MM-DD if date parsing fails
            _forecasts[i].dayOfWeek = dateStr.substring(5, 10);
        }
    }
    
    Serial.print("Parsed ");
    Serial.print(count);
    Serial.println(" days of forecast");
}

CurrentWeather WeatherAPI::getCurrentWeather() {
    return _currentWeather;
}

DailyForecast WeatherAPI::getForecast(int day) {
    if (day >= 0 && day < 7) {
        return _forecasts[day];
    }
    DailyForecast empty;
    empty.valid = false;
    return empty;
}

String WeatherAPI::getWeatherIcon(int weatherCode) {
    // WMO Weather interpretation codes
    // https://open-meteo.com/en/docs
    if (weatherCode == 0) return LV_SYMBOL_OK;  // Clear sky
    if (weatherCode <= 3) return LV_SYMBOL_IMAGE;  // Partly cloudy
    if (weatherCode <= 48) return LV_SYMBOL_IMAGE;  // Fog
    if (weatherCode <= 67) return LV_SYMBOL_IMAGE;  // Rain
    if (weatherCode <= 77) return LV_SYMBOL_IMAGE;  // Snow
    if (weatherCode <= 82) return LV_SYMBOL_IMAGE;  // Rain showers
    if (weatherCode <= 86) return LV_SYMBOL_IMAGE;  // Snow showers
    return LV_SYMBOL_WARNING;  // Thunderstorm
}

String WeatherAPI::getWeatherDescription(int weatherCode) {
    // WMO Weather interpretation codes
    if (weatherCode == 0) return "Clear sky";
    if (weatherCode == 1) return "Mainly clear";
    if (weatherCode == 2) return "Partly cloudy";
    if (weatherCode == 3) return "Overcast";
    if (weatherCode == 45 || weatherCode == 48) return "Foggy";
    if (weatherCode >= 51 && weatherCode <= 55) return "Drizzle";
    if (weatherCode >= 56 && weatherCode <= 57) return "Freezing drizzle";
    if (weatherCode >= 61 && weatherCode <= 65) return "Rain";
    if (weatherCode >= 66 && weatherCode <= 67) return "Freezing rain";
    if (weatherCode >= 71 && weatherCode <= 75) return "Snow";
    if (weatherCode == 77) return "Snow grains";
    if (weatherCode >= 80 && weatherCode <= 82) return "Rain showers";
    if (weatherCode >= 85 && weatherCode <= 86) return "Snow showers";
    if (weatherCode >= 95 && weatherCode <= 99) return "Thunderstorm";
    return "Unknown";
}

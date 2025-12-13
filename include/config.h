#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
#define WIFI_RECONNECT_INTERVAL 30000  // 30 seconds

// NTP Configuration
#define NTP_SERVER "pool.ntp.org"
#define NTP_UPDATE_INTERVAL 3600000  // 1 hour in milliseconds
#define DEFAULT_TIMEZONE "PST8PDT,M3.2.0,M11.1.0"  // Pacific Time with DST
#define GMT_OFFSET_SEC -28800  // PST is UTC-8 hours
#define DAYLIGHT_OFFSET_SEC 3600  // 1 hour DST

// Weather Configuration (Open-Meteo API)
#define WEATHER_UPDATE_INTERVAL 1800000  // 30 minutes in milliseconds
#define DEFAULT_LATITUDE "37.7749"  // San Francisco
#define DEFAULT_LONGITUDE "-122.4194"
#define WEATHER_API_URL "https://api.open-meteo.com/v1/forecast"

// Backlight Configuration
#define DEFAULT_BACKLIGHT_ON_HOUR 7    // 7:00 AM
#define DEFAULT_BACKLIGHT_OFF_HOUR 22  // 10:00 PM
#define BACKLIGHT_FADE_DURATION 500    // milliseconds
#define TOUCH_WAKE_TIMEOUT 30000       // Auto-dim after 30 seconds of inactivity

// Display Configuration
#define TEMP_UNIT_FAHRENHEIT true  // false for Celsius
#define TIME_FORMAT_24H false      // true for 24-hour format, false for 12-hour

// Preferences namespace
#define PREFS_NAMESPACE "smartdisplay"

#endif // CONFIG_H

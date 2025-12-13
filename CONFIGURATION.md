# Smart Display Configuration Guide

This guide explains how to configure your Waveshare ESP32-S3-Touch-LCD-4.3 Smart Display.

## Quick Start

### 1. WiFi Configuration

Edit `include/config.h` and update your WiFi credentials:

```cpp
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"
```

### 2. Location Configuration

For weather data, set your location coordinates in `include/config.h`:

```cpp
#define DEFAULT_LATITUDE "37.7749"   // Your latitude
#define DEFAULT_LONGITUDE "-122.4194" // Your longitude
```

You can find your coordinates at: https://www.latlong.net/

### 3. Timezone Configuration

The default timezone is set to Pacific Standard Time (PST). To change it, modify:

```cpp
#define DEFAULT_TIMEZONE "PST8PDT,M3.2.0,M11.1.0"  // POSIX timezone string
#define GMT_OFFSET_SEC -28800  // Offset from UTC in seconds
#define DAYLIGHT_OFFSET_SEC 3600  // DST offset in seconds
```

#### Common Timezone Examples:

**US Timezones:**
- **Pacific (PST/PDT):** `"PST8PDT,M3.2.0,M11.1.0"`, GMT offset: `-28800`
- **Mountain (MST/MDT):** `"MST7MDT,M3.2.0,M11.1.0"`, GMT offset: `-25200`
- **Central (CST/CDT):** `"CST6CDT,M3.2.0,M11.1.0"`, GMT offset: `-21600`
- **Eastern (EST/EDT):** `"EST5EDT,M3.2.0,M11.1.0"`, GMT offset: `-18000`

**Other Timezones:**
- **UK (GMT/BST):** `"GMT0BST,M3.5.0/1,M10.5.0"`, GMT offset: `0`
- **Central Europe (CET/CEST):** `"CET-1CEST,M3.5.0,M10.5.0/3"`, GMT offset: `3600`
- **Japan (JST):** `"JST-9"`, GMT offset: `32400`
- **Australia Eastern (AEST/AEDT):** `"AEST-10AEDT,M10.1.0,M4.1.0/3"`, GMT offset: `36000`

For more timezones, see: https://github.com/nayarsystems/posix_tz_db

### 4. Backlight Schedule

Configure when the display turns on/off automatically:

```cpp
#define DEFAULT_BACKLIGHT_ON_HOUR 7    // 7:00 AM
#define DEFAULT_BACKLIGHT_OFF_HOUR 22  // 10:00 PM
```

### 5. Temperature Units

Choose between Fahrenheit or Celsius:

```cpp
#define TEMP_UNIT_FAHRENHEIT true  // false for Celsius
```

### 6. Time Format

Choose between 12-hour or 24-hour format:

```cpp
#define TIME_FORMAT_24H false  // true for 24-hour, false for 12-hour
```

## Features

### WiFi Connection
- Automatic connection on startup
- Auto-reconnect on connection loss (every 30 seconds)
- WiFi status indicator on display
- Signal strength display (RSSI in dBm)

### Time Synchronization
- NTP time sync with configurable server
- Automatic timezone handling with DST support
- Live clock display with date
- Time format customization (12/24 hour)

### Weather Forecast
- Current weather conditions:
  - Temperature (current, high/low)
  - Weather description (clear, cloudy, rain, etc.)
  - Humidity and wind speed
- 7-day forecast:
  - Daily high/low temperatures
  - Weather codes translated to descriptions
  - Precipitation probability
- Updates every 30 minutes
- Uses Open-Meteo API (no API key required)

### Backlight Control
- **Scheduled control:**
  - Automatic on/off based on time
  - Configurable hours
  - Settings persist across reboots
- **Touch-to-wake:**
  - Touch screen when off to wake display
  - Auto-dim after 30 seconds of inactivity
- **Manual control:**
  - Touch screen to keep awake

### User Interface
- **Main Screen:**
  - Large clock display
  - Current date and day of week
  - Current temperature (large display)
  - Weather condition description
  - High/low temperature for today
  - WiFi status indicator
- **7-Day Forecast:**
  - Bottom section shows 7-day forecast
  - Each day shows date, high/low temps
  - Compact display for easy viewing

## Troubleshooting

### WiFi Won't Connect
1. Verify SSID and password are correct
2. Check if 2.4GHz network (ESP32 doesn't support 5GHz)
3. Ensure network is not hidden
4. Check serial monitor for connection status

### Time Not Syncing
1. Verify WiFi is connected
2. Check NTP server is accessible
3. Verify timezone settings are correct
4. Look for "Time synchronized!" in serial monitor

### Weather Not Updating
1. Verify WiFi is connected
2. Check latitude/longitude are correct (use decimal format)
3. Verify API URL is accessible
4. Check serial monitor for HTTP errors
5. Open-Meteo API is free and requires no key

### Backlight Not Working
1. Check backlight schedule settings
2. Verify current time is correct
3. Try manual toggle by touching screen
4. Check if time is within scheduled ON hours

### Display Shows "---" or "--"
This indicates data not yet loaded:
- Wait for WiFi connection
- Wait for time sync
- Wait for first weather update (can take up to 30 seconds)

## Advanced Configuration

### Changing Weather Update Interval

In `include/config.h`:
```cpp
#define WEATHER_UPDATE_INTERVAL 1800000  // 30 minutes (in milliseconds)
```

### Changing NTP Update Interval

In `include/config.h`:
```cpp
#define NTP_UPDATE_INTERVAL 3600000  // 1 hour (in milliseconds)
```

### Changing Touch Wake Timeout

In `include/config.h`:
```cpp
#define TOUCH_WAKE_TIMEOUT 30000  // 30 seconds (in milliseconds)
```

### Using Different NTP Server

In `include/config.h`:
```cpp
#define NTP_SERVER "time.nist.gov"  // Or other NTP server
```

## Display Pinout Reference

- **LCD Backlight:** Pin 2 (via IO Expander)
- **Touch Reset:** Pin 1 (via IO Expander)
- **LCD Reset:** Pin 3 (via IO Expander)
- **I2C SDA:** GPIO 8
- **I2C SCL:** GPIO 9

## Weather API Information

This project uses the **Open-Meteo API**, which is:
- ✅ Free and open-source
- ✅ No API key required
- ✅ No registration needed
- ✅ Professional weather data
- ✅ 7-day forecasts included
- ✅ Worldwide coverage

API Documentation: https://open-meteo.com/en/docs

### Weather Codes Reference

The display shows weather conditions based on WMO weather codes:
- **0:** Clear sky
- **1-3:** Partly cloudy to overcast
- **45-48:** Fog
- **51-67:** Rain (various intensities)
- **71-77:** Snow
- **80-82:** Rain showers
- **85-86:** Snow showers
- **95-99:** Thunderstorm

## Serial Monitor Output

Connect via serial at 115200 baud to see:
- WiFi connection status
- Time synchronization status
- Weather update status
- Backlight schedule events
- Touch events
- Error messages and debugging info

## Persistence

Settings are stored in ESP32 preferences (NVS):
- Backlight ON hour
- Backlight OFF hour

These settings survive power cycles and firmware updates.

## Building and Uploading

1. Open in PlatformIO
2. Configure settings in `include/config.h`
3. Build: `pio run`
4. Upload: `pio run --target upload`
5. Monitor: `pio device monitor`

## Support

For issues, see the serial monitor output first. Most problems are configuration-related.

Common fixes:
- Verify WiFi credentials
- Check coordinates format (decimal degrees)
- Ensure timezone string is correct
- Verify NTP server is accessible from your network

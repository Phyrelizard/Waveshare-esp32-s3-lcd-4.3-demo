# Waveshare ESP32-S3-Touch-LCD-4.3 Smart Display

A fully-featured smart display for the Waveshare ESP32-S3-Touch-LCD-4.3, featuring WiFi connectivity, real-time clock, weather forecasting, and intelligent backlight management.

![Smart Display](https://img.shields.io/badge/ESP32--S3-Smart%20Display-blue)
![LVGL](https://img.shields.io/badge/LVGL-v8.3.8-green)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange)

## 🚀 Quick Start

**New to this project? Start here:** [QUICKSTART.md](QUICKSTART.md) - Get up and running in 5 minutes!

**Need detailed configuration help?** See [CONFIGURATION.md](CONFIGURATION.md) for timezone examples, troubleshooting, and advanced settings.

## ✨ Features

### 🌐 WiFi Connectivity
- Automatic connection on startup with configurable credentials
- WiFi status indicator showing signal strength (RSSI)
- Automatic reconnection on connection loss
- Real-time connection monitoring

### ⏰ Network Time Synchronization (NTP)
- Automatic time sync via NTP protocol
- **Default timezone: Pacific Standard Time (PST) with automatic DST**
- User-configurable timezone support for worldwide use
- Live clock display with current time (12/24 hour format)
- Date display with day of week
- Automatic daylight saving time handling

### 🌤️ Weather Application
- **Current weather conditions:**
  - Current temperature with large, readable display
  - Today's high and low temperatures
  - Weather description (Clear, Cloudy, Rain, Snow, etc.)
  - Humidity and wind speed
- **7-day weather forecast:**
  - Daily high/low temperatures for the week ahead
  - Weather condition codes for each day
  - Precipitation probability
  - Compact forecast display at bottom of screen
- Powered by **Open-Meteo API** (no API key required!)
- Updates automatically every 30 minutes
- Location configurable via latitude/longitude

### 💡 Programmable Backlight Control
- **Scheduled automatic control:**
  - Configurable "time on" hour (default: 7:00 AM)
  - Configurable "time off" hour (default: 10:00 PM)
  - Settings persist across reboots
- **Touch-to-wake feature:**
  - Touch screen to wake display when backlight is off
  - Auto-dim after 30 seconds of inactivity
  - Works even when display is sleeping

### 🎨 Professional User Interface
- Clean, modern LVGL-based UI design
- Large, readable fonts for time and temperature
- Status bar showing WiFi signal strength
- Real-time updates without blocking
- Smooth animations and transitions
- Professional color scheme with good contrast

## 📋 Requirements

### Hardware
- Waveshare ESP32-S3-Touch-LCD-4.3 development board
- WiFi network (2.4GHz)
- USB cable for programming and power

### Software
- [PlatformIO](https://platformio.org/) IDE or CLI
- Libraries (automatically installed via platformio.ini):
  - LVGL 8.3.8
  - ArduinoJson 7.0.0
  - NTPClient 3.2.1
  - ESP32_Display_Panel (included in lib/)
  - ESP32_IO_Expander (included in lib/)

## 📖 Documentation

- **[QUICKSTART.md](QUICKSTART.md)** - 5-minute setup guide for beginners 🚀
- **[CONFIGURATION.md](CONFIGURATION.md)** - Detailed configuration guide with timezone examples ⚙️
- **[IMPLEMENTATION.md](IMPLEMENTATION.md)** - Complete technical documentation 📚
- **config.h.example** - Example configuration with extensive comments

## 🔧 Basic Setup

### 1. Clone and Open Project
```bash
git clone https://github.com/Phyrelizard/Waveshare-esp32-s3-lcd-4.3-demo.git
cd Waveshare-esp32-s3-lcd-4.3-demo
```

### 2. Configure Your Settings

Edit `include/config.h` with your information:

```cpp
// WiFi credentials
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"

// Your location (for weather)
#define DEFAULT_LATITUDE "37.7749"    // San Francisco example
#define DEFAULT_LONGITUDE "-122.4194"
```

**📍 Find your coordinates:** https://www.latlong.net/

See [QUICKSTART.md](QUICKSTART.md) for detailed step-by-step instructions!

### 3. Build and Upload

```bash
# Using PlatformIO CLI
pio run --target upload

# Or use PlatformIO IDE
# Open project, click "Upload" button
```

### 4. Upload to Board

1. Connect your board via USB
2. Press and hold the **Boot** button
3. Press the **Reset** button
4. Release the **Boot** button
5. Click Upload in PlatformIO

### 5. Monitor Serial Output (Optional)

```bash
pio device monitor
# Baud rate: 115200
```

For complete step-by-step instructions with screenshots, see **[QUICKSTART.md](QUICKSTART.md)**

## 📖 Configuration

See **[CONFIGURATION.md](CONFIGURATION.md)** for detailed setup instructions including:
- WiFi configuration
- Timezone settings for all regions
- Weather location setup
- Backlight schedule customization
- Temperature units (°F/°C)
- Time format (12/24 hour)
- Troubleshooting guide

## 🏗️ Project Structure

```
.
├── src/
│   ├── main.cpp                 # Main application logic
│   ├── wifi_manager.cpp         # WiFi connection handling
│   ├── time_manager.cpp         # NTP time synchronization
│   ├── weather_api.cpp          # Weather data fetching
│   ├── backlight_controller.cpp # Backlight scheduling
│   └── ui_manager.cpp           # UI display updates
├── include/
│   ├── config.h                 # Main configuration file ⚙️
│   ├── wifi_manager.h
│   ├── time_manager.h
│   ├── weather_api.h
│   ├── backlight_controller.h
│   └── ui_manager.h
├── lib/
│   ├── ESP32_Display_Panel/     # Display driver
│   ├── ESP32_IO_Expander/       # IO expander for backlight
│   └── ui/                      # LVGL UI components (legacy)
├── platformio.ini               # PlatformIO configuration
├── CONFIGURATION.md             # Detailed setup guide
└── README.md                    # This file
```

## 🌍 Timezone Support

The display supports all major timezones with automatic DST:

- **US:** Pacific, Mountain, Central, Eastern
- **Europe:** GMT, CET, EET, WET
- **Asia:** JST, CST, IST, SGT
- **Australia:** AEST, ACST, AWST
- **And many more!**

See [CONFIGURATION.md](CONFIGURATION.md) for timezone configuration examples.

## 🌐 Weather API

This project uses the **Open-Meteo API**:
- ✅ **Free** and open-source
- ✅ **No API key required**
- ✅ **No registration** needed
- ✅ Professional weather data
- ✅ 7-day forecasts
- ✅ Worldwide coverage

Learn more: https://open-meteo.com/

## 🛠️ Technical Details

### Libraries & Dependencies
```ini
lib_deps = 
    lvgl/lvgl@8.3.8
    bblanchon/ArduinoJson@^7.0.0
    arduino-libraries/NTPClient@^3.2.1
```

### Display Specifications
- **Resolution:** 800x480 pixels
- **Display:** 4.3" TFT LCD with RGB interface
- **Touch:** Capacitive touch via GT911
- **Backlight:** Controllable via TCA9554 IO Expander (Pin 2)

### Memory Usage
- **PSRAM:** Enabled for larger LVGL buffers
- **Flash:** 8MB
- **RAM:** Optimized for smooth UI performance

## 🐛 Troubleshooting

### Common Issues

**WiFi won't connect:**
- Verify SSID and password in `config.h`
- Ensure 2.4GHz network (ESP32 doesn't support 5GHz)
- Check serial monitor for error messages

**Time shows "---":**
- WiFi must be connected first
- Wait 5-10 seconds for NTP sync
- Check timezone settings

**Weather shows "---":**
- WiFi must be connected
- Verify latitude/longitude are correct
- Wait up to 30 seconds for first update
- Check serial monitor for HTTP errors

**Backlight not working:**
- Check schedule settings match current time
- Try touching screen to wake
- Verify time is synchronized

See [CONFIGURATION.md](CONFIGURATION.md) for more troubleshooting tips.

## 📊 Serial Monitor Output

Connect at 115200 baud to see detailed logging:
- WiFi connection status and signal strength
- NTP time synchronization events
- Weather API requests and responses
- Backlight schedule events
- Touch interactions
- Error messages and debugging info

## 🔄 Updates & Maintenance

### Weather Data
- Updates automatically every 30 minutes
- Manual update on WiFi reconnection
- Configurable update interval

### Time Synchronization
- Initial sync on WiFi connection
- Automatic resync every hour
- Timezone and DST handled automatically

### Settings Persistence
- Backlight schedule saved to NVS (non-volatile storage)
- Survives power cycles and firmware updates
- Factory defaults available

## 🎯 Future Enhancements

Potential features for future versions:
- Settings UI screen for on-device configuration
- Multiple screen layouts (swipe to switch)
- Additional weather data (pressure, UV index)
- WiFi Manager captive portal for easy setup
- Multiple location support
- Weather alerts and notifications
- Customizable color themes

## 📄 License

This project builds upon the original Waveshare ESP32-S3-Touch-LCD-4.3 demo.

Original demo components retain their original licenses. New smart display features are provided as-is for educational and personal use.

## 🙏 Acknowledgments

- **Waveshare** for the ESP32-S3-Touch-LCD-4.3 hardware and base libraries
- **LVGL** for the excellent embedded graphics library
- **Open-Meteo** for free weather data API
- **ESP-Arduino-Libs** for display panel libraries
- Original demo project contributors

## 📞 Support

For issues and questions:
1. Check [CONFIGURATION.md](CONFIGURATION.md) first
2. Review serial monitor output
3. Verify all settings in `config.h`
4. Check GitHub Issues

## 🌟 Original Demo

This project is based on the original Waveshare demo. The original simple two-screen button demo UI files are preserved in `lib/ui/` but are no longer used. The display now shows a functional smart display interface.

---

**Made with ☕ for the Waveshare ESP32-S3-Touch-LCD-4.3 community**

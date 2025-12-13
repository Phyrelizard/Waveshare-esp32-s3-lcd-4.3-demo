# Quick Start Guide - Smart Display

Get your Waveshare ESP32-S3-Touch-LCD-4.3 Smart Display up and running in 5 minutes!

## Prerequisites

✅ Waveshare ESP32-S3-Touch-LCD-4.3 board  
✅ USB cable  
✅ WiFi network (2.4GHz)  
✅ PlatformIO IDE or CLI  

## Step 1: Get Your WiFi Credentials

You'll need:
- Your WiFi network name (SSID)
- Your WiFi password

## Step 2: Get Your Location Coordinates

Visit https://www.latlong.net/ and find your coordinates:
- Enter your city or address
- Copy the latitude and longitude (e.g., 37.7749, -122.4194)

## Step 3: Configure the Display

Edit `include/config.h`:

```cpp
// 1. WiFi Settings
#define WIFI_SSID "YourActualWiFiName"      // ← Replace this
#define WIFI_PASSWORD "YourActualPassword"  // ← Replace this

// 2. Your Location (for weather)
#define DEFAULT_LATITUDE "37.7749"          // ← Your latitude
#define DEFAULT_LONGITUDE "-122.4194"       // ← Your longitude

// 3. Backlight Schedule (optional - default is fine)
#define DEFAULT_BACKLIGHT_ON_HOUR 7         // Turn on at 7:00 AM
#define DEFAULT_BACKLIGHT_OFF_HOUR 22       // Turn off at 10:00 PM
```

**That's it! Those are the only settings you MUST change.**

## Step 4: Upload to Board

### Using PlatformIO IDE:
1. Open the project in PlatformIO
2. Connect your board via USB
3. Press and hold the **Boot** button on the board
4. Press the **Reset** button (while still holding Boot)
5. Release the **Boot** button
6. Click the **Upload** button (→) in PlatformIO
7. Wait for upload to complete

### Using PlatformIO CLI:
```bash
# Build and upload
pio run --target upload
```

## Step 5: Enjoy!

After upload completes:
1. The display will show the smart display UI
2. It will connect to WiFi (watch for WiFi icon in status bar)
3. Time will sync from the internet
4. Weather will update within 30 seconds
5. Touch the screen to interact

## What You'll See

### Status Bar (Top)
- WiFi icon with signal strength (e.g., "-45 dBm")
- Green = connected, Red = disconnected

### Main Display
- **Large Clock** - Current time
- **Date** - Day of week and full date
- **Temperature** - Large orange number (current temp)
- **Weather** - Condition description and high/low temps

### Bottom
- **7-Day Forecast** - Shows upcoming week with temps

## Monitoring Serial Output (Optional)

Connect serial monitor at 115200 baud to see:
- WiFi connection status
- Time synchronization
- Weather updates
- Touch events
- Backlight changes

```bash
# PlatformIO CLI
pio device monitor
```

## Troubleshooting

### Display shows "---"
**Cause:** Waiting for data  
**Solution:** Wait 10-30 seconds for WiFi connection and first update

### WiFi won't connect
**Cause:** Wrong credentials or 5GHz network  
**Solutions:**
- Double-check WIFI_SSID and WIFI_PASSWORD spelling
- Ensure you're using a 2.4GHz network (ESP32 doesn't support 5GHz)
- Check serial monitor for error messages

### Wrong temperature or weather
**Cause:** Incorrect coordinates  
**Solution:** Verify latitude and longitude are correct at latlong.net

### Time is wrong
**Cause:** Wrong timezone  
**Solution:** See CONFIGURATION.md for your timezone settings

### Backlight doesn't turn off
**Cause:** Outside scheduled hours or recent touch  
**Solution:** 
- Wait until the scheduled OFF hour
- Backlight stays on for 30 seconds after touch

## Next Steps

### Want to customize more?

See **CONFIGURATION.md** for:
- Different timezones
- Celsius instead of Fahrenheit
- 24-hour time format
- Different backlight schedule
- Advanced settings

### Want technical details?

See **IMPLEMENTATION.md** for:
- Architecture overview
- Module descriptions
- Technical specifications
- Feature details

## Need Help?

1. **Check serial monitor** - Most issues show error messages
2. **Read CONFIGURATION.md** - Detailed troubleshooting guide
3. **Verify settings in config.h** - Most issues are configuration
4. **Check GitHub Issues** - See if others had same problem

## Optional Configurations

### Use Celsius Instead of Fahrenheit

In `include/config.h`:
```cpp
#define TEMP_UNIT_FAHRENHEIT false  // Change to false for Celsius
```

### Use 24-Hour Time Format

In `include/config.h`:
```cpp
#define TIME_FORMAT_24H true  // Change to true for 24-hour format
```

### Change Timezone (Example: Eastern Time)

In `include/config.h`:
```cpp
#define DEFAULT_TIMEZONE "EST5EDT,M3.2.0,M11.1.0"
#define GMT_OFFSET_SEC -18000  // EST is UTC-5 hours
```

See **CONFIGURATION.md** for more timezone examples!

## Features

✅ **WiFi Connectivity** - Auto-connect and reconnect  
✅ **Live Clock** - Always accurate via NTP  
✅ **Current Weather** - Temperature, conditions, humidity, wind  
✅ **7-Day Forecast** - Week ahead at a glance  
✅ **Smart Backlight** - Auto on/off based on time  
✅ **Touch to Wake** - Touch screen when off to wake  
✅ **No API Keys** - Uses free Open-Meteo weather API  

## Default Behavior

- **WiFi:** Connects on startup, reconnects if lost
- **Time:** Syncs every hour, handles DST automatically
- **Weather:** Updates every 30 minutes
- **Backlight:** 
  - ON at 7:00 AM
  - OFF at 10:00 PM
  - Touch to wake when off
  - Stays on 30 seconds after touch

---

**🎉 Congratulations! Your smart display is ready to use!**

For more information, see:
- **README.md** - Full feature list and overview
- **CONFIGURATION.md** - Detailed configuration options
- **IMPLEMENTATION.md** - Technical documentation

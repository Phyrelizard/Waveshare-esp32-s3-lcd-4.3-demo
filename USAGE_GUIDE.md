# ESP32 Smart Display - User Guide

## Table of Contents
1. [Getting Started](#getting-started)
2. [Night Mode](#night-mode)
3. [Photo Frame Mode](#photo-frame-mode)
4. [Remote Control](#remote-control)
5. [Tips and Tricks](#tips-and-tricks)
6. [Troubleshooting](#troubleshooting)

---

## Getting Started

### Initial Setup

1. **Flash the firmware:**
   - Connect the Waveshare ESP32-S3-LCD-4.3 to your computer via USB
   - Hold Boot button, press Reset, release Boot
   - Upload the firmware using PlatformIO: `pio run --target upload`

2. **Configure WiFi (optional):**
   - Edit `src/main.cpp` before uploading:
   ```cpp
   #define WIFI_SSID "YourWiFiNetwork"
   #define WIFI_PASSWORD "YourPassword"
   #define ENABLE_WIFI true
   ```
   - Re-upload firmware
   - Note the IP address from serial monitor

3. **Verify operation:**
   - Display should show the default SquareLine UI
   - Touch should be responsive
   - Check serial monitor for initialization messages

---

## Night Mode

### What is Night Mode?

Night mode automatically switches the display to a dark theme with reduced blue light and dimmer backlight during nighttime hours. This is easier on the eyes in dark environments and can help with sleep patterns.

### Features

- **Automatic activation** based on time schedule
- **Dark color scheme** with black backgrounds and warm amber/red tones
- **Reduced backlight** (default 30% brightness)
- **Manual override** to force day or night mode
- **Smooth transitions** between modes

### Default Schedule

- **Night mode starts:** 8:00 PM (20:00)
- **Night mode ends:** 7:00 AM (07:00)

### Configuring Night Mode

#### Via Code (before uploading)

Edit `src/main.cpp` in the `setup()` function:

```cpp
// Set custom schedule (start hour, end hour in 24-hour format)
nightMode.setSchedule(22, 6);  // 10 PM to 6 AM

// Set night brightness (0-100%)
nightMode.setNightBrightness(20);  // 20% brightness

// Enable/disable automatic mode
nightMode.setAutoMode(true);  // true = automatic, false = manual only
```

#### Manual Control

To manually toggle night mode (requires code modification to add button/touch handler):

```cpp
// Force night mode on
nightMode.setNightMode(true);

// Force night mode off
nightMode.setNightMode(false);

// Check current mode
if (nightMode.isNightMode()) {
    // Currently in night mode
}
```

### Color Schemes

**Day Mode:**
- Background: White (#FFFFFF)
- Primary: Blue (#2196F3)
- Text: Black (#000000)
- Accent: Orange (#FF9800)

**Night Mode:**
- Background: Black (#000000)
- Primary: Dark Red (#8B0000)
- Text: Dark Orange (#FF8C00)
- Accent: Amber (#FFAA00)

---

## Photo Frame Mode

### Overview

Photo frame mode turns your display into a digital photo frame with automatic slideshow capabilities.

### Getting Images on the Display

#### Method 1: Web Upload (Recommended)
1. Connect to the same WiFi as the display
2. Open web browser to `http://esp32-display.local`
3. Click "Choose File" under Upload Image
4. Select your image (JPEG, PNG, or BMP)
5. Click "Upload Image"

#### Method 2: API Upload
```bash
curl -X POST -F "image=@vacation.jpg" http://esp32-display.local/api/upload-image
```

### Image Requirements

- **Formats:** JPEG, PNG, BMP
- **Maximum size:** 512 KB per image
- **Recommended resolution:** 800×480 (native display resolution)
- **Storage capacity:** 50+ images (depending on file sizes)

### Starting the Slideshow

Currently requires code modification to add a button handler:

```cpp
photoFrame.start();  // Start slideshow
```

### Slideshow Controls

Once started, the slideshow will:
- Display each image for 30 seconds (configurable)
- Show image counter (e.g., "5/20")
- Display clock overlay (if enabled)
- Automatically advance to next image

### Touch Controls (implemented in UI)

- **Tap:** Show/hide control bar
- **Swipe left:** Next image
- **Swipe right:** Previous image
- **Long press:** Pause/resume
- **Exit button:** Return to main screen

### Configuration

Edit before uploading:

```cpp
// Set display duration per image (seconds)
photoFrame.setDuration(45);  // 45 seconds per image

// Enable/disable shuffle
photoFrame.setShuffle(true);  // Random order

// Show clock on photos
photoFrame.setClockOverlay(true);  // Show time/date
```

### Managing Images

**View all images:**
```bash
curl http://esp32-display.local/api/images
```

**Delete an image:**
```bash
curl -X DELETE http://esp32-display.local/api/images/vacation.jpg
```

Or use the web interface at `http://esp32-display.local`

---

## Remote Control

### Web Interface

The easiest way to control your display remotely.

#### Accessing the Web Interface

1. Ensure WiFi is enabled and connected
2. Open browser on any device on the same network
3. Navigate to:
   - `http://esp32-display.local` (preferred)
   - Or `http://<ip-address>` (use IP from serial monitor)

#### Web Interface Features

**Upload Images:**
1. Click "Choose File"
2. Select image
3. Check "Display immediately" to show right away
4. Click "Upload Image"

**Send Messages:**
1. Enter message text
2. Optional: Add a title
3. Select priority:
   - Normal: Small notification
   - Important: Yellow dialog box
   - Urgent: Red full-screen alert
4. Select duration or "Until dismissed"
5. Click "Send Message"

**View Images:**
- See list of all uploaded images
- View file sizes and upload dates
- Delete images directly

**System Status:**
- Storage space usage
- Number of images
- Slideshow status
- WiFi signal strength

### REST API

For automation and integration with other systems. See [API_DOCUMENTATION.md](API_DOCUMENTATION.md) for full details.

**Quick Examples:**

Upload image:
```bash
curl -X POST -F "image=@photo.jpg" http://esp32-display.local/api/upload-image
```

Send notification:
```bash
curl -X POST \
  -d "message=Doorbell!" \
  -d "priority=2" \
  http://esp32-display.local/api/send-message
```

### Message System

#### Priority Levels

1. **Normal (0):**
   - Appears as toast notification
   - Auto-dismisses after duration
   - Doesn't interrupt slideshow

2. **Important (1):**
   - Modal dialog with orange accent
   - Requires user interaction or timeout
   - Pauses slideshow temporarily

3. **Urgent (2):**
   - Full-screen red alert
   - Interrupts any current display
   - Requires acknowledgment

#### Message Queue

- Messages are queued if one is already displaying
- Urgent messages jump to front of queue
- Messages auto-dismiss after configured duration
- Permanent messages (duration: -1) stay until dismissed

### Integration Examples

#### Home Automation

**Home Assistant automation:**
```yaml
automation:
  - alias: "Doorbell notification"
    trigger:
      platform: state
      entity_id: binary_sensor.doorbell
      to: 'on'
    action:
      - service: rest_command.esp32_message
        data:
          message: "Someone at the door!"
          priority: 2
          duration: 30
```

**REST command configuration:**
```yaml
rest_command:
  esp32_message:
    url: "http://esp32-display.local/api/send-message"
    method: POST
    payload: "message={{ message }}&priority={{ priority }}&duration={{ duration }}"
```

#### Weather Updates

Python script to send weather updates:
```python
import requests
import schedule
import time

def send_weather():
    # Get weather from API
    weather = get_weather_data()  # Your weather API
    
    message = f"Temperature: {weather['temp']}°C\n"
    message += f"Condition: {weather['condition']}"
    
    requests.post('http://esp32-display.local/api/send-message', data={
        'message': message,
        'title': 'Weather Update',
        'priority': 0,
        'duration': 60
    })

# Send weather every 3 hours
schedule.every(3).hours.do(send_weather)

while True:
    schedule.run_pending()
    time.sleep(60)
```

---

## Tips and Tricks

### Optimizing Images

1. **Resize images to 800×480** before uploading for best performance
2. **Use JPEG with medium compression** for good quality and small size
3. **Batch resize** using ImageMagick:
   ```bash
   mogrify -resize 800x480 -quality 85 *.jpg
   ```

### Battery Backup

For uninterrupted operation:
- Use a UPS or battery pack
- ESP32-S3 consumes ~500mA with display
- A 10,000mAh battery provides ~15-20 hours

### Extending Storage

- Default LittleFS partition is ~1.5MB
- Can store 50+ images at average 30KB each
- Regularly clean up old images
- Consider using lower JPEG quality for more images

### Power Saving

Night mode automatically reduces backlight:
- Day: 100% brightness
- Night: 30% brightness (configurable)
- Further reduction via `panel->getLcd()->setBrightness(10)`

### Remote Monitoring

Check status periodically:
```bash
# Cron job to check space every hour
0 * * * * curl http://esp32-display.local/api/status | \
  jq -r 'if (.free_space < 100000) then "Low space warning!" else empty end' | \
  mail -s "Display Alert" admin@example.com
```

---

## Troubleshooting

### Display Issues

**No display:**
- Check power connection
- Verify USB cable provides sufficient current (2A recommended)
- Check serial monitor for error messages

**Touch not working:**
- Ensure IO expander initialized (check serial log)
- Try power cycle
- Check touch calibration in SquareLine Studio settings

**Yellow artifacts during animations:**
- Known issue with this display panel
- Reduce animation speeds in LVGL settings
- Use static transitions instead

### Network Issues

**Can't access web interface:**
- Verify WiFi is enabled in code (`ENABLE_WIFI true`)
- Check WiFi credentials are correct
- Ensure device is on same network as computer
- Try IP address instead of `esp32-display.local`
- Check router hasn't blocked device

**mDNS not working:**
- Some networks block mDNS
- Use IP address directly
- Windows may require Bonjour service installed

### Image Upload Issues

**Upload fails:**
- Check file size (must be under 512KB)
- Verify file type (JPEG, PNG, BMP only)
- Ensure sufficient storage space
- Check network connection

**Image doesn't display:**
- Verify upload was successful (check API response)
- Start slideshow mode
- Check serial monitor for decoding errors
- Try different image format

### Memory Issues

**Out of memory errors:**
- Reduce number of stored images
- Use smaller image files
- Lower LVGL buffer size if needed
- Reboot device to clear memory

**Slow performance:**
- Too many images in slideshow
- Image files too large
- Reduce transition effects
- Increase slideshow duration

### System Not Responding

**Display frozen:**
1. Check serial monitor for crash logs
2. Power cycle the device
3. Re-upload firmware if issue persists

**Web server not responding:**
1. Check WiFi connection
2. Verify IP address hasn't changed
3. Restart device
4. Check serial monitor for errors

---

## Getting Help

If you encounter issues not covered here:

1. **Check serial monitor** - Most errors are logged
2. **Review the code** - Comments explain functionality
3. **Test with examples** - Use curl commands to isolate issues
4. **Check memory usage** - Use ESP.getFreeHeap()
5. **Open an issue on GitHub** - Include serial logs and details

---

## Advanced Configuration

### Custom Themes

Edit `src/ui/ui_themes.h` to customize colors:

```cpp
// Day mode colors
#define DAY_BG_COLOR        lv_color_hex(0xF0F0F0)  // Light gray
#define DAY_PRIMARY_COLOR   lv_color_hex(0x00BCD4)  // Cyan
// ... etc

// Night mode colors
#define NIGHT_BG_COLOR      lv_color_hex(0x121212)  // Very dark gray
#define NIGHT_TEXT_COLOR    lv_color_hex(0xFFB74D)  // Warm orange
// ... etc
```

### Adding Features

The modular design makes it easy to add features:

1. **New screens:** Create in SquareLine Studio, export to `lib/ui/`
2. **New APIs:** Add endpoints in `src/web_server.cpp`
3. **Custom handlers:** Modify main loop in `src/main.cpp`

---

## Safety and Warnings

- Do not expose device to moisture
- Ensure adequate ventilation
- Use appropriate power supply (5V 2A minimum)
- Do not modify hardware without proper knowledge
- Keep away from heat sources
- Regular backups recommended (filesystem can be corrupted on power loss)

---

## Maintenance

### Regular Tasks

**Weekly:**
- Check available storage
- Clear old images if needed
- Verify WiFi connectivity

**Monthly:**
- Review message history
- Update WiFi password if changed
- Check for firmware updates

**As Needed:**
- Clean display screen gently with microfiber cloth
- Check for loose connections
- Update slideshow images

---

## Support

For additional help:
- Read the full [README.md](README.md)
- Check [API_DOCUMENTATION.md](API_DOCUMENTATION.md)
- Review source code comments
- Open issue on GitHub
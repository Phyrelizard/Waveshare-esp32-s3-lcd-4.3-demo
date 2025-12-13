# Waveshare ESP32-S3-Touch-LCD-4.3 with Advanced Features

This project provides a feature-rich smart display implementation for the Waveshare ESP32-S3-LCD-4.3 Touch panel using SquareLine Studio and PlatformIO.

## Features

### 🌙 Night Mode with Automatic Color Scheme
- **Automatic activation** based on configurable time schedule
- **Two color schemes:**
  - **Day Mode:** Bright colors, high contrast, white/light backgrounds
  - **Night Mode:** Dark theme, red/amber tones, reduced blue light, black backgrounds
- **Smart backlight control:** Automatically dims display in night mode
- **Manual override:** Toggle night mode on/off regardless of schedule
- **Configurable schedule:** Default 8:00 PM - 7:00 AM, user-adjustable

### 🖼️ Photo Frame Mode with Image Slideshow
- **Image slideshow** that cycles through stored images
- **Image storage:** Uses LittleFS for persistent storage (JPEG, PNG, BMP supported)
- **Slideshow controls:**
  - Configurable display duration (default: 30 seconds)
  - Shuffle or sequential playback
  - Touch controls: tap to show/hide controls, swipe for next/previous
- **Clock overlay:** Optional time/date display on photos
- **Memory efficient:** Supports up to 50+ images depending on size (512KB max per image)

### 📡 Remote Image & Message Sender (Network Feature)
- **HTTP web server** for remote control from any PC/phone on the network
- **Web interface:** Upload images, send messages, view status
- **REST API** for programmatic access:
  - `POST /api/upload-image` - Upload images
  - `POST /api/send-message` - Send text messages with priority levels
  - `GET /api/images` - List stored images
  - `DELETE /api/images/{filename}` - Delete images
  - `GET /api/status` - System status
- **Message display:**
  - Three priority levels: Normal, Important, Urgent
  - Configurable duration or persistent display
  - Visual overlays with priority-based styling
- **Security:**
  - File type validation (magic byte checking)
  - File size limits (512KB default)
  - Optional password authentication
- **mDNS support:** Access via `http://esp32-display.local`

## Get Started

### Hardware Setup
1. Connect your Waveshare ESP32-S3-LCD-4.3 board to USB
2. Press and hold the Boot button, then press Reset, then release Boot button
3. Board is ready for programming

### Software Setup
1. Install [PlatformIO](https://platformio.org/) or use VS Code with PlatformIO extension
2. Clone this repository
3. Open the project in PlatformIO
4. Configure WiFi (optional, for remote control):
   ```cpp
   // In src/main.cpp
   #define WIFI_SSID "YourWiFiName"
   #define WIFI_PASSWORD "YourWiFiPassword"
   #define ENABLE_WIFI true
   ```
5. Build and upload:
   ```bash
   pio run --target upload
   ```

## Configuration

### Night Mode Settings
Edit in `src/main.cpp` or via future settings UI:
```cpp
nightMode.setSchedule(20, 7);         // Start at 8PM, end at 7AM
nightMode.setNightBrightness(30);     // 30% brightness in night mode
nightMode.setAutoMode(true);          // Enable automatic switching
```

### Photo Frame Settings
```cpp
photoFrame.setDuration(30);           // 30 seconds per image
photoFrame.setShuffle(false);         // Sequential playback
photoFrame.setClockOverlay(true);     // Show clock on photos
```

### Web Server Settings
```cpp
webServer.setPassword("your_password"); // Set authentication password
webServer.setMaxUploadSize(512 * 1024); // 512KB max per image
```

## Usage

### Using the Web Interface
1. Connect to the same WiFi network as the ESP32
2. Open browser and navigate to:
   - `http://esp32-display.local` (if mDNS works)
   - Or use the IP address shown in serial monitor
3. Upload images, send messages, and control the display remotely

### REST API Examples

**Upload an image:**
```bash
curl -X POST -F "image=@photo.jpg" http://esp32-display.local/api/upload-image
```

**Send a message:**
```bash
curl -X POST http://esp32-display.local/api/send-message \
  -d "message=Hello World" \
  -d "priority=2" \
  -d "duration=10"
```

**List images:**
```bash
curl http://esp32-display.local/api/images
```

**Get status:**
```bash
curl http://esp32-display.local/api/status
```

### Python Example Client
```python
import requests

# Upload image
with open('photo.jpg', 'rb') as f:
    files = {'image': f}
    response = requests.post('http://esp32-display.local/api/upload-image', files=files)
    print(response.json())

# Send message
data = {
    'message': 'Temperature Alert: 25°C',
    'title': 'Weather Update',
    'priority': 1,  # Important
    'duration': 30
}
response = requests.post('http://esp32-display.local/api/send-message', data=data)
print(response.json())
```

## API Documentation

### POST /api/upload-image
Upload an image to the display.

**Parameters:**
- `image` (file, required): Image file (JPEG, PNG, or BMP)
- `show_now` (boolean, optional): Immediately display the image
- `temporary` (boolean, optional): Auto-delete after showing

**Response:**
```json
{
  "status": "success",
  "filename": "image.jpg"
}
```

### POST /api/send-message
Send a text message to display.

**Parameters:**
- `message` (string, required): Message text
- `title` (string, optional): Message title
- `priority` (integer, 0-2): 0=Normal, 1=Important, 2=Urgent
- `duration` (integer): Display duration in seconds (-1 for permanent)

**Response:**
```json
{
  "status": "success",
  "message_id": 123
}
```

### GET /api/images
List all stored images.

**Response:**
```json
[
  {
    "filename": "photo1.jpg",
    "size": 45678,
    "uploaded": 1702484400
  }
]
```

### DELETE /api/images/{filename}
Delete a specific image.

**Response:**
```json
{
  "status": "success"
}
```

### GET /api/status
Get system status information.

**Response:**
```json
{
  "free_space": 1048576,
  "total_space": 1572864,
  "image_count": 5,
  "slideshow_active": true,
  "wifi_rssi": -45
}
```

## File Structure

```
.
├── src/
│   ├── main.cpp                    # Main application
│   ├── filesystem_manager.h/cpp    # LittleFS image storage
│   ├── night_mode.h/cpp            # Night mode scheduling
│   ├── photo_frame.h/cpp           # Slideshow logic
│   ├── message_handler.h/cpp       # Message queue system
│   ├── web_server.h/cpp            # HTTP server & API
│   └── ui/
│       ├── ui_themes.h/cpp         # Day/night themes
│       ├── ui_photo_frame.h/cpp    # Photo frame UI
│       └── ui_message.h/cpp        # Message overlay UI
├── data/
│   └── index.html                  # Web control interface
├── lib/
│   ├── ui/                         # SquareLine Studio UI
│   ├── ESP32_Display_Panel/        # Display driver
│   └── ESP32_IO_Expander/          # IO expander driver
└── platformio.ini                  # Build configuration
```

## Libraries

The project uses these libraries (automatically installed via PlatformIO):

- **lvgl/lvgl@8.3.8** - Graphics library
- **me-no-dev/ESP Async WebServer** - Async HTTP server
- **me-no-dev/AsyncTCP** - Async TCP library
- **lorol/LittleFS_esp32** - File system
- **bblanchon/ArduinoJson** - JSON parsing

Plus the custom libraries from Waveshare:
- **ESP32_Display_Panel** - Display driver with CH422G support
- **ESP32_IO_Expander** - IO expander for the board

## PlatformIO Configuration

```ini
[env:esp32s3box]
platform = espressif32
board = esp32s3box
framework = arduino
monitor_speed = 115200
board_upload.flash_size = 8MB
build_flags = 
	-D BOARD_HAS_PSRAM
	-D LV_CONF_INCLUDE_SIMPLE
	-I lib
board_build.arduino.memory_type = qio_opi
board_build.f_flash = 80000000L
board_build.flash_mode = qio
lib_deps = 
	lvgl/lvgl@8.3.8
	me-no-dev/ESP Async WebServer@^1.2.3
	me-no-dev/AsyncTCP@^1.1.1
	lorol/LittleFS_esp32@^1.0.6
	bblanchon/ArduinoJson@^7.0.3
```

## SquareLine Studio

The UI is created with SquareLine Studio using the Arduino with TFT_eSPI template. The exported UI files are in `lib/ui/`.

To modify the UI:
1. Open the project in SquareLine Studio (`squareline/waveshare43-minimal.spj`)
2. Make your changes
3. Export to Template Project
4. Copy the `ui` folder to `lib/ui/`

## Security Considerations

- **File validation:** All uploaded files are checked for valid image magic bytes
- **Size limits:** Maximum file size enforced (default 512KB)
- **Authentication:** Optional password protection for API endpoints
- **Input sanitization:** Message text and filenames are validated
- **Network isolation:** Only accessible on local network

## Memory Management

- **LVGL buffer:** 800 × 20 pixels using internal SRAM
- **Image storage:** LittleFS partition on flash memory
- **Upload buffer:** Dynamically allocated during uploads
- **Message queue:** Limited to prevent memory overflow

## Troubleshooting

### WiFi won't connect
- Check SSID and password in `src/main.cpp`
- Ensure WiFi is 2.4GHz (ESP32 doesn't support 5GHz)
- Check serial monitor for connection status

### Images won't upload
- Ensure file is under 512KB
- Only JPEG, PNG, and BMP are supported
- Check serial monitor for error messages
- Verify filesystem initialized successfully

### Display artifacts or flickering
- This is a known issue with animations on the Waveshare board
- Reduce animation speeds in LVGL settings

### Out of memory errors
- Reduce number of stored images
- Lower LVGL_BUF_SIZE if needed
- Use smaller image files

## Future Enhancements

Potential additions:
- Settings UI for configuring features without code changes
- Sunrise/sunset API integration for automatic night mode timing
- Support for animated GIFs
- Integration with smart home systems (MQTT, Home Assistant)
- Weather widget with online data
- Calendar integration
- Voice message support (text-to-speech)

## Contributing

Feedback and contributions welcome! Please open an issue or pull request on GitHub.

## License

This project builds upon the original Waveshare demo. Check individual library licenses for details.

## Credits

- Original demo by Waveshare
- LVGL graphics library
- SquareLine Studio for UI design
- ESP32 Arduino framework
- PlatformIO build system

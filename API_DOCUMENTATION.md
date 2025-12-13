# ESP32 Smart Display - API Documentation

## Overview

This document describes the REST API endpoints available on the ESP32 Smart Display web server.

Base URL: `http://esp32-display.local` or `http://<device-ip>`

## Authentication

If authentication is enabled (via `webServer.setPassword()`), include the password parameter in POST requests:

```
POST /api/endpoint?password=your_password
```

Or as form data:
```
password=your_password
```

## Endpoints

### Upload Image

Upload an image to the display's file system.

**Endpoint:** `POST /api/upload-image`

**Content-Type:** `multipart/form-data`

**Parameters:**

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| image | file | Yes | Image file (JPEG, PNG, or BMP) |
| show_now | boolean | No | Immediately display this image |
| temporary | boolean | No | Delete after showing once |

**Example Request:**

```bash
curl -X POST \
  -F "image=@photo.jpg" \
  -F "show_now=true" \
  http://esp32-display.local/api/upload-image
```

**Success Response (200):**

```json
{
  "status": "success",
  "filename": "photo.jpg"
}
```

**Error Responses:**

| Code | Description |
|------|-------------|
| 400 | No file uploaded or invalid file type |
| 401 | Unauthorized (password required) |
| 413 | File too large (>512KB default) |
| 500 | Failed to save image |

---

### Send Message

Send a text message to display on the screen.

**Endpoint:** `POST /api/send-message`

**Content-Type:** `application/x-www-form-urlencoded` or `multipart/form-data`

**Parameters:**

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| message | string | Yes | Message text (max 500 chars) |
| title | string | No | Message title |
| priority | integer | No | 0=Normal, 1=Important, 2=Urgent (default: 0) |
| duration | integer | No | Display duration in seconds, -1=permanent (default: 10) |

**Example Request:**

```bash
curl -X POST \
  -d "message=Temperature is 25°C" \
  -d "title=Weather Alert" \
  -d "priority=1" \
  -d "duration=30" \
  http://esp32-display.local/api/send-message
```

**Example with JSON:**

```bash
curl -X POST \
  -H "Content-Type: application/x-www-form-urlencoded" \
  -d "message=Hello World&priority=2&duration=10" \
  http://esp32-display.local/api/send-message
```

**Success Response (200):**

```json
{
  "status": "success",
  "message_id": 42
}
```

**Priority Levels:**
- **0 (Normal):** Toast notification at bottom of screen
- **1 (Important):** Modal dialog with orange accent
- **2 (Urgent):** Full-screen interrupt with red accent

**Error Responses:**

| Code | Description |
|------|-------------|
| 400 | Missing required message parameter |
| 401 | Unauthorized |

---

### List Images

Get a list of all images stored on the device.

**Endpoint:** `GET /api/images`

**Parameters:** None

**Example Request:**

```bash
curl http://esp32-display.local/api/images
```

**Success Response (200):**

```json
[
  {
    "filename": "sunset.jpg",
    "size": 128456,
    "uploaded": 1702484400
  },
  {
    "filename": "family.png",
    "size": 256789,
    "uploaded": 1702488000
  }
]
```

**Fields:**
- `filename`: Name of the image file
- `size`: File size in bytes
- `uploaded`: Unix timestamp of upload time

---

### Delete Image

Delete a specific image from the file system.

**Endpoint:** `DELETE /api/images/{filename}`

**Parameters:**

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| filename | string | Yes | Name of image to delete (in URL path) |

**Example Request:**

```bash
curl -X DELETE http://esp32-display.local/api/images/sunset.jpg
```

**Success Response (200):**

```json
{
  "status": "success"
}
```

**Error Responses:**

| Code | Description |
|------|-------------|
| 401 | Unauthorized |
| 404 | Image not found |

---

### Get Status

Get current system status and statistics.

**Endpoint:** `GET /api/status`

**Parameters:** None

**Example Request:**

```bash
curl http://esp32-display.local/api/status
```

**Success Response (200):**

```json
{
  "free_space": 1048576,
  "total_space": 1572864,
  "image_count": 5,
  "slideshow_active": true,
  "wifi_rssi": -45
}
```

**Fields:**
- `free_space`: Available storage in bytes
- `total_space`: Total storage capacity in bytes
- `image_count`: Number of images stored
- `slideshow_active`: Whether photo frame mode is active
- `wifi_rssi`: WiFi signal strength in dBm

---

### Display Now

Immediately display a specific image or message.

**Endpoint:** `POST /api/display-now`

**Content-Type:** `application/x-www-form-urlencoded`

**Parameters:**

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| type | string | Yes | "image" or "message" |
| content | string | Yes | Filename (for image) or text (for message) |
| duration | integer | No | Display duration in seconds |

**Example Request (Image):**

```bash
curl -X POST \
  -d "type=image" \
  -d "content=sunset.jpg" \
  -d "duration=60" \
  http://esp32-display.local/api/display-now
```

**Example Request (Message):**

```bash
curl -X POST \
  -d "type=message" \
  -d "content=Break time!" \
  -d "duration=10" \
  http://esp32-display.local/api/display-now
```

**Success Response (200):**

```json
{
  "status": "success"
}
```

**Error Responses:**

| Code | Description |
|------|-------------|
| 400 | Missing or invalid parameters |
| 401 | Unauthorized |
| 404 | Image not found (for type=image) |

---

## Error Response Format

All error responses follow this format:

```json
{
  "error": "Description of the error"
}
```

---

## Rate Limiting

To prevent abuse, the API implements basic rate limiting:
- Maximum 60 requests per minute per IP address
- Upload size limited to 512KB per file (configurable)

---

## Code Examples

### Python

```python
import requests
import json

base_url = "http://esp32-display.local"

# Upload image
def upload_image(filepath):
    with open(filepath, 'rb') as f:
        files = {'image': f}
        response = requests.post(f"{base_url}/api/upload-image", files=files)
        return response.json()

# Send message
def send_message(text, priority=0, duration=10):
    data = {
        'message': text,
        'priority': priority,
        'duration': duration
    }
    response = requests.post(f"{base_url}/api/send-message", data=data)
    return response.json()

# Get images
def list_images():
    response = requests.get(f"{base_url}/api/images")
    return response.json()

# Delete image
def delete_image(filename):
    response = requests.delete(f"{base_url}/api/images/{filename}")
    return response.json()

# Get status
def get_status():
    response = requests.get(f"{base_url}/api/status")
    return response.json()

# Usage
print(upload_image('photo.jpg'))
print(send_message('Hello from Python!', priority=1, duration=15))
print(list_images())
print(get_status())
```

### Node.js

```javascript
const axios = require('axios');
const FormData = require('form-data');
const fs = require('fs');

const baseUrl = 'http://esp32-display.local';

// Upload image
async function uploadImage(filepath) {
  const form = new FormData();
  form.append('image', fs.createReadStream(filepath));
  
  const response = await axios.post(`${baseUrl}/api/upload-image`, form, {
    headers: form.getHeaders()
  });
  return response.data;
}

// Send message
async function sendMessage(text, priority = 0, duration = 10) {
  const params = new URLSearchParams();
  params.append('message', text);
  params.append('priority', priority);
  params.append('duration', duration);
  
  const response = await axios.post(`${baseUrl}/api/send-message`, params);
  return response.data;
}

// Get images
async function listImages() {
  const response = await axios.get(`${baseUrl}/api/images`);
  return response.data;
}

// Delete image
async function deleteImage(filename) {
  const response = await axios.delete(`${baseUrl}/api/images/${filename}`);
  return response.data;
}

// Get status
async function getStatus() {
  const response = await axios.get(`${baseUrl}/api/status`);
  return response.data;
}

// Usage
(async () => {
  console.log(await uploadImage('photo.jpg'));
  console.log(await sendMessage('Hello from Node.js!', 1, 15));
  console.log(await listImages());
  console.log(await getStatus());
})();
```

### Bash (curl)

```bash
#!/bin/bash

BASE_URL="http://esp32-display.local"

# Upload image
upload_image() {
  curl -X POST -F "image=@$1" "${BASE_URL}/api/upload-image"
}

# Send message
send_message() {
  curl -X POST \
    -d "message=$1" \
    -d "priority=${2:-0}" \
    -d "duration=${3:-10}" \
    "${BASE_URL}/api/send-message"
}

# List images
list_images() {
  curl "${BASE_URL}/api/images" | jq .
}

# Delete image
delete_image() {
  curl -X DELETE "${BASE_URL}/api/images/$1"
}

# Get status
get_status() {
  curl "${BASE_URL}/api/status" | jq .
}

# Usage
upload_image "photo.jpg"
send_message "Hello from Bash!" 1 15
list_images
get_status
```

---

## Best Practices

1. **Check response status codes** - Always verify the HTTP status code before processing the response
2. **Handle errors gracefully** - Implement retry logic for network failures
3. **Validate file sizes** - Check image file size before uploading
4. **Use appropriate priorities** - Reserve urgent (2) for critical alerts only
5. **Limit message length** - Keep messages concise for better display
6. **Monitor storage** - Check `/api/status` periodically to avoid running out of space
7. **Clean up old images** - Delete images you no longer need to free up space

---

## Troubleshooting

### Connection refused
- Verify the device is on the same network
- Check the IP address in serial monitor
- Try using IP address instead of `esp32-display.local`

### Uploads failing
- Ensure file size is under 512KB
- Verify file type (JPEG, PNG, or BMP only)
- Check available storage with `/api/status`

### Authentication errors
- Include password parameter if authentication is enabled
- Verify password is correct

### Images not displaying
- Check that image format is supported
- Verify image was uploaded successfully
- Check serial monitor for error messages

---

## Support

For issues or questions:
1. Check the serial monitor output for errors
2. Verify API endpoint URLs and parameters
3. Review this documentation
4. Open an issue on GitHub
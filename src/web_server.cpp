#include "web_server.h"
#include "filesystem_manager.h"
#include "message_handler.h"
#include "photo_frame.h"
#include <ArduinoJson.h>

WebServerManager webServer;

static String uploadFilename = "";
static std::vector<uint8_t> uploadBuffer;

void WebServerManager::begin(const String& deviceName) {
    mdnsName = deviceName;
    
    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected, web server not started");
        return;
    }
    
    // Setup mDNS
    if (MDNS.begin(mdnsName.c_str())) {
        MDNS.addService("http", "tcp", 80);
        Serial.printf("mDNS responder started: http://%s.local\n", mdnsName.c_str());
    }
    
    // Create server
    server = new AsyncWebServer(80);
    
    setupRoutes();
    
    server->begin();
    serverRunning = true;
    
    Serial.printf("Web server started on http://%s\n", WiFi.localIP().toString().c_str());
}

void WebServerManager::update() {
    // Nothing needed for async server
}

void WebServerManager::setupRoutes() {
    // Serve main page
    server->on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", 
            "<!DOCTYPE html>"
            "<html><head><title>ESP32 Display Control</title>"
            "<style>body{font-family:Arial;max-width:800px;margin:50px auto;padding:20px;}"
            ".section{margin:30px 0;padding:20px;border:1px solid #ccc;border-radius:8px;}"
            "button{padding:10px 20px;margin:5px;cursor:pointer;}"
            "input[type='file'],textarea{width:100%;padding:10px;margin:10px 0;}"
            "select{padding:10px;margin:5px;}</style></head>"
            "<body><h1>🖼️ ESP32 Smart Display Control</h1>"
            "<div class='section'><h2>📤 Upload Image</h2>"
            "<form action='/api/upload-image' method='POST' enctype='multipart/form-data'>"
            "<input type='file' name='image' accept='image/*' required>"
            "<button type='submit'>Upload Image</button></form></div>"
            "<div class='section'><h2>💬 Send Message</h2>"
            "<form action='/api/send-message' method='POST'>"
            "<textarea name='message' rows='4' placeholder='Enter your message...'></textarea>"
            "<select name='priority'>"
            "<option value='0'>Normal</option>"
            "<option value='1'>Important</option>"
            "<option value='2'>Urgent</option></select>"
            "<select name='duration'>"
            "<option value='5'>5 seconds</option>"
            "<option value='10'>10 seconds</option>"
            "<option value='30'>30 seconds</option>"
            "<option value='-1'>Until dismissed</option></select>"
            "<button type='submit'>Send Message</button></form></div>"
            "<div class='section'><h2>🖼️ Current Images</h2>"
            "<div id='imageList'><a href='/api/images'>View Images (JSON)</a></div></div>"
            "</body></html>");
    });
    
    // API: Upload image
    server->on("/api/upload-image", HTTP_POST,
        [this](AsyncWebServerRequest* request) {
            if (!checkAuth(request)) {
                request->send(401, "application/json", "{\"error\":\"Unauthorized\"}");
                return;
            }
            
            if (uploadBuffer.empty()) {
                request->send(400, "application/json", "{\"error\":\"No file uploaded\"}");
                return;
            }
            
            bool success = fsManager.saveImage(uploadBuffer.data(), uploadBuffer.size(), uploadFilename, false);
            uploadBuffer.clear();
            
            if (success) {
                request->send(200, "application/json", 
                    "{\"status\":\"success\",\"filename\":\"" + uploadFilename + "\"}");
            } else {
                request->send(500, "application/json", "{\"error\":\"Failed to save image\"}");
            }
        },
        [this](AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
            handleUploadImage(request, filename, index, data, len, final);
        }
    );
    
    // API: Send message
    server->on("/api/send-message", HTTP_POST, [this](AsyncWebServerRequest* request) {
        handleSendMessage(request);
    });
    
    // API: Get images list
    server->on("/api/images", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleGetImages(request);
    });
    
    // API: Delete image
    server->on("^\\/api\\/images\\/(.+)$", HTTP_DELETE, [this](AsyncWebServerRequest* request) {
        handleDeleteImage(request);
    });
    
    // API: Get status
    server->on("/api/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        handleGetStatus(request);
    });
    
    // API: Display now
    server->on("/api/display-now", HTTP_POST, [this](AsyncWebServerRequest* request) {
        handleDisplayNow(request);
    });
}

void WebServerManager::handleUploadImage(AsyncWebServerRequest* request, String filename, 
                                        size_t index, uint8_t* data, size_t len, bool final) {
    if (index == 0) {
        uploadBuffer.clear();
        uploadFilename = filename;
        Serial.printf("Upload started: %s\n", filename.c_str());
    }
    
    // Check size limit
    if (uploadBuffer.size() + len > maxUploadSize) {
        Serial.println("Upload too large");
        uploadBuffer.clear();
        return;
    }
    
    // Efficiently append data using insert
    uploadBuffer.insert(uploadBuffer.end(), data, data + len);
    
    if (final) {
        Serial.printf("Upload finished: %d bytes\n", uploadBuffer.size());
    }
}

void WebServerManager::handleSendMessage(AsyncWebServerRequest* request) {
    if (!checkAuth(request)) {
        request->send(401, "application/json", "{\"error\":\"Unauthorized\"}");
        return;
    }
    
    String message = "";
    String title = "";
    int duration = 10;
    MessagePriority priority = PRIORITY_NORMAL;
    
    if (request->hasParam("message", true)) {
        message = request->getParam("message", true)->value();
    }
    if (request->hasParam("title", true)) {
        title = request->getParam("title", true)->value();
    }
    if (request->hasParam("duration", true)) {
        duration = request->getParam("duration", true)->value().toInt();
    }
    if (request->hasParam("priority", true)) {
        int p = request->getParam("priority", true)->value().toInt();
        priority = (MessagePriority)p;
    }
    
    uint32_t msgId = messageHandler.addMessage(message, title, duration, priority);
    
    request->send(200, "application/json", 
        "{\"status\":\"success\",\"message_id\":" + String(msgId) + "}");
}

void WebServerManager::handleGetImages(AsyncWebServerRequest* request) {
    std::vector<ImageInfo> images = fsManager.listImages();
    
    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    
    for (const auto& img : images) {
        JsonObject obj = array.add<JsonObject>();
        obj["filename"] = img.filename;
        obj["size"] = img.size;
        obj["uploaded"] = img.uploaded;
    }
    
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
}

void WebServerManager::handleDeleteImage(AsyncWebServerRequest* request) {
    if (!checkAuth(request)) {
        request->send(401, "application/json", "{\"error\":\"Unauthorized\"}");
        return;
    }
    
    // Extract filename from URL path
    String path = request->url();
    int lastSlash = path.lastIndexOf('/');
    String filename = path.substring(lastSlash + 1);
    
    bool success = fsManager.deleteImage(filename);
    
    if (success) {
        request->send(200, "application/json", "{\"status\":\"success\"}");
    } else {
        request->send(404, "application/json", "{\"error\":\"Image not found\"}");
    }
}

void WebServerManager::handleGetStatus(AsyncWebServerRequest* request) {
    JsonDocument doc;
    doc["free_space"] = fsManager.getFreeSpace();
    doc["total_space"] = fsManager.getTotalSpace();
    doc["image_count"] = fsManager.listImages().size();
    doc["slideshow_active"] = photoFrame.isActive();
    doc["wifi_rssi"] = WiFi.RSSI();
    
    String output;
    serializeJson(doc, output);
    request->send(200, "application/json", output);
}

void WebServerManager::handleDisplayNow(AsyncWebServerRequest* request) {
    if (!checkAuth(request)) {
        request->send(401, "application/json", "{\"error\":\"Unauthorized\"}");
        return;
    }
    
    String type = "";
    String content = "";
    int duration = 10;
    
    if (request->hasParam("type", true)) {
        type = request->getParam("type", true)->value();
    }
    if (request->hasParam("content", true)) {
        content = request->getParam("content", true)->value();
    }
    if (request->hasParam("duration", true)) {
        duration = request->getParam("duration", true)->value().toInt();
    }
    
    if (type.length() == 0 || content.length() == 0) {
        request->send(400, "application/json", "{\"error\":\"Missing type or content parameter\"}");
        return;
    }
    
    if (type == "message") {
        // Send message with urgent priority to display immediately
        messageHandler.addMessage(content, "", duration, PRIORITY_URGENT);
        request->send(200, "application/json", "{\"status\":\"success\",\"type\":\"message\"}");
    } else if (type == "image") {
        // Check if image exists
        if (!fsManager.imageExists(content)) {
            request->send(404, "application/json", "{\"error\":\"Image not found\"}");
            return;
        }
        
        // Stop current slideshow and display this image
        photoFrame.stop();
        // TODO: Display single image - requires PhotoFrame enhancement
        // For now, just add to slideshow and start it
        photoFrame.start();
        
        request->send(200, "application/json", "{\"status\":\"success\",\"type\":\"image\"}");
    } else {
        request->send(400, "application/json", "{\"error\":\"Invalid type. Must be 'image' or 'message'\"}");
    }
}

bool WebServerManager::checkAuth(AsyncWebServerRequest* request) {
    if (authPassword.length() == 0) {
        return true;  // No authentication required
    }
    
    if (request->hasParam("password", true)) {
        return request->getParam("password", true)->value() == authPassword;
    }
    
    return false;
}

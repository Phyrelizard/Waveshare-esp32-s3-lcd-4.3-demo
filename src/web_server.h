#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

class WebServerManager {
public:
    void begin(const String& deviceName = "esp32-display");
    void update();
    
    void setPassword(const String& password) { authPassword = password; }
    void setMaxUploadSize(size_t size) { maxUploadSize = size; }
    
    bool isRunning() { return serverRunning; }
    String getDeviceName() { return mdnsName; }
    
private:
    AsyncWebServer* server = nullptr;
    bool serverRunning = false;
    String authPassword = "";
    size_t maxUploadSize = 512 * 1024;  // 512KB default
    String mdnsName = "esp32-display";
    
    void setupRoutes();
    void handleUploadImage(AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final);
    void handleSendMessage(AsyncWebServerRequest* request);
    void handleGetImages(AsyncWebServerRequest* request);
    void handleDeleteImage(AsyncWebServerRequest* request);
    void handleGetStatus(AsyncWebServerRequest* request);
    void handleDisplayNow(AsyncWebServerRequest* request);
    
    bool checkAuth(AsyncWebServerRequest* request);
};

extern WebServerManager webServer;

#endif

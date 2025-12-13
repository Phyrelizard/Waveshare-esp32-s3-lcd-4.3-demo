#include "wifi_manager.h"
#include "config.h"

WiFiManager::WiFiManager() : _lastReconnectAttempt(0), _connected(false) {
}

void WiFiManager::begin(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;
    
    WiFi.mode(WIFI_STA);
    connect();
}

void WiFiManager::connect() {
    Serial.println("Connecting to WiFi...");
    Serial.print("SSID: ");
    Serial.println(_ssid);
    
    WiFi.begin(_ssid, _password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        _connected = true;
        Serial.println("\nWiFi connected!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Signal strength: ");
        Serial.print(WiFi.RSSI());
        Serial.println(" dBm");
    } else {
        _connected = false;
        Serial.println("\nWiFi connection failed!");
    }
}

void WiFiManager::reconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected, attempting to reconnect...");
        WiFi.disconnect();
        delay(100);
        WiFi.begin(_ssid, _password);
    }
}

void WiFiManager::loop() {
    bool currentStatus = (WiFi.status() == WL_CONNECTED);
    
    if (currentStatus != _connected) {
        _connected = currentStatus;
        if (_connected) {
            Serial.println("WiFi reconnected!");
        } else {
            Serial.println("WiFi connection lost!");
        }
    }
    
    if (!_connected) {
        unsigned long now = millis();
        if (now - _lastReconnectAttempt > WIFI_RECONNECT_INTERVAL) {
            _lastReconnectAttempt = now;
            reconnect();
        }
    }
}

bool WiFiManager::isConnected() {
    return _connected;
}

int WiFiManager::getSignalStrength() {
    if (_connected) {
        return WiFi.RSSI();
    }
    return -100;  // No signal
}

String WiFiManager::getLocalIP() {
    if (_connected) {
        return WiFi.localIP().toString();
    }
    return "0.0.0.0";
}

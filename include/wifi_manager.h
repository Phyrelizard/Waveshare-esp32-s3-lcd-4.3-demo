#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

class WiFiManager {
public:
    WiFiManager();
    void begin(const char* ssid, const char* password);
    void loop();
    bool isConnected();
    int getSignalStrength();  // Returns RSSI
    String getLocalIP();
    
private:
    const char* _ssid;
    const char* _password;
    unsigned long _lastReconnectAttempt;
    bool _connected;
    
    void connect();
    void reconnect();
};

#endif // WIFI_MANAGER_H

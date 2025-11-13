#pragma once
#define MYDEBUG 0
#include "mydebug.h"


#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "SPIFFS.h"
#include <ESPmDNS.h>
#include <functional>
#include <cstring>   // für memset, strlen
 
#include "basisinterface.h"

class SmartHomeSettings : public BasisInterface
{
public:
    SmartHomeSettings(const char* apPrefix = "SmartHomeSettings_",
                      const char* settingsFile = "/settings.txt");

    bool isConfigured() const { return _configured; }

    // Getter liefern jetzt C-Strings (const char*)
    const char* getWifiSsid()       const { return _settings.wifiSsid; }
    const char* getWifiPassword()   const { return _settings.wifiPassword; }
    const char* getMqttPassword()   const { return _settings.mqttPassword; }
    const char* getMqttUser()       const { return _settings.mqttUser; }
    const char* getMqttServeraddr() const { return _settings.mqttServeraddr; }
    const char* getMqttName()       const { return _settings.mqttName; }
    uint8_t     getCanAddr()        const { return _settings.canAddr; }

    void deleteSettings();
	
	
	void update();
    bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
private:
    // maximale Länge inkl. '\0' (also max. 29 sichtbare Zeichen)
    static const uint8_t MAX_STR_LEN = 30;

    struct Settings {
        char wifiSsid[MAX_STR_LEN];
        char wifiPassword[MAX_STR_LEN];
        char mqttPassword[MAX_STR_LEN];
        char mqttUser[MAX_STR_LEN];
        char mqttServeraddr[MAX_STR_LEN];
        char mqttName[MAX_STR_LEN];
        uint8_t canAddr = 0;
    };

    Settings   _settings;
    WebServer  _server;
    String     _apPrefix;
    String     _settingsFile;
    bool       _configured;

    void init();

    bool mountSpiffs();
    bool loadSettings();
    bool saveSettings();
    
    bool settingsValid(const Settings& s) const;
    void printSettingsToSerial() const;

    void startConfigPortal();
    void handleRoot();
    void handleSave();
    void handleNotFound();
};

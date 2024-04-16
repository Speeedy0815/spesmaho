
#pragma once


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"

#else
#include "WProgram.h"
#endif

#include "ethernetmqtt.h"



#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else
#error "Bibliothek nur für ESP8266 oder ESP32 getestet"
#endif




class ESP_Wifi_MQTT : public EthernetMQTT
{
public:
	ESP_Wifi_MQTT(const char* server, const char* MYMQTTNAME, const char* MYMQTTUS, const char* MYMQTTPW, const char* SSID, const char* WIFIPW);
	ESP_Wifi_MQTT(const char* server, const char* MYMQTTNAME, const char* MYMQTTUS, const char* MYMQTTPW, const char* SSID, const char* WIFIPW, uint16_t brokerport);
	~ESP_Wifi_MQTT() {};
private:
	void init(const char* SSID, const char* WIFIPW);
	WiFiClient ethClient;
protected:
};



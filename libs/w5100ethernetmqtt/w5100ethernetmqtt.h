
#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"

#else
#include "WProgram.h"
#endif

#include "ethernetmqtt.h"

#if defined(__AVR__)
#include <Ethernet.h>  
#endif
#if defined(ESP32) ||  defined(ESP8266)
#include "Ethernetega.h"
#endif
#if defined(ESP32) ||  defined(ESP8266)

#ifdef ESP32
 
#include "Wifi.h"
#else
#include <ESP8266WiFi.h>
#endif

void GLOGetETHMacFromWifiMac(uint8_t* puffermac);
#endif

#include "hilfe.h"


class MqttCommunication : public EthernetMQTT
{
public:

	MqttCommunication(const char* server, uint8_t* mac, const char* MYMQTTNAME, const char* MYMQTTUS, const char* MYMQTTPW, uint8_t Resetpin, uint8_t CSPin);
	MqttCommunication(const char* server, uint8_t* mac, const char* MYMQTTNAME, const char* MYMQTTUS, const char* MYMQTTPW, uint8_t Resetpin);

	~MqttCommunication() {};

private:

	void init(uint8_t* mac, uint8_t Resetpin, uint8_t cspin);
	EthernetClient ethClient;

protected:


};

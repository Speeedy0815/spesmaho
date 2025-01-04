
#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"

#else
#include "WProgram.h"
#endif

#include "ethernetmqtt.h"

 
#include "Ethernet2.h"  
 

void GLOGetETHMacFromWifiMac(uint8_t* puffermac);
 

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


#pragma once

#define MYDEBUG 0
#include "mydebug.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"

#else
#include "WProgram.h"
#endif


#include <PubSubClient.h>  
#include "basiskommunikation.h"
#include "Client.h"
#include "hilfe.h" // Da ist die Callbackfunktion

class EthernetMQTT : public Basiskommunikation
{
public:

	EthernetMQTT(const char* server, const char* MYMQTTNAME, const char* MYMQTTUS, const char* MYMQTTPW, Client& client, uint16_t brokerport);
	~EthernetMQTT() {};
	void update();
	void subscribeextra(const char* topic);  // für den CAN geacht
	//void sendmessage(const char* topic, const char* msgbuffer);
	void setCallback(MQTT_CALLBACK_SIGNATURE);
private:
	//settings
	Client* _client; // Netzwerkclient vom Parent 
	void dopublish(const char* topic, const byte* msgbuffer, uint8_t len);

protected:

	PubSubClient MqttClient;

	void reconnect();


	const char* _MQTTPW;
	const char* _MQTTUS;
};



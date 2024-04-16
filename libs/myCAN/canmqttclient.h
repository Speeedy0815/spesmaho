
#pragma once


#include "basiskommunikation.h"
#include "cangemeinsam.h"
#include <PubSubClient.h>  //MQTT_CALLBACK_SIGNATURE 
#include "canbasis.h"

class CANMQTT : public Basiskommunikation
{
public:
	CANMQTT(uint8_t nr,  canbasis* hardware);
	~CANMQTT() {};
	void update();
	//void dopublish(const char* topic, const char* msgbuffer);
	void dopublish(const char* topic, const byte* msgbuffer, uint8_t len);
	void setCallback(MQTT_CALLBACK_SIGNATURE);
	virtual void subscribeextra(const char* topic) {};
private:
	char namenbuffer[4]; //max 3 + Stoppzeichen
	uint8_t _nr;
	MQTT_CALLBACK_SIGNATURE;
	
	CANMYNachricht NACHR;
	CANInterface _CANInterface;

};



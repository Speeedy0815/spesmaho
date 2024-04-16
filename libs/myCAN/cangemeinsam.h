#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#include "canbasis.h"

const long CANSENDTOALL = 100;
const long CANTODEVICEOFFSET = 200;
const long CANFROMDEVICEOFFSET = 300;
const uint8_t CANNACHRPUFFVOLL = 0xff;










class CANInterface
{
private:
	canbasis * _canhardware;
	
public:
	CANInterface( canbasis * canhardware);
	~CANInterface() {};
	
	void publish(const char* topic, const byte* msgbuffer, uint8_t lenbuffer, long id);

	bool updateandismessage(mycan_frame* canMsg);
};


class CANMYNachricht
{
private:
	uint8_t nachricherhaltenbitweise = 0;
	uint8_t nachricherhaltenbitweisesoll = 0;
	uint8_t lentopic = 0;
	long lastCAnID = 0xffff;

	const static uint8_t MAXLEN = 64; // max 8 Nachrichten  --> wennn vergrößerung, dann umstellung der beiden oberen Dtentypen größer!!!
	uint8_t puffer[MAXLEN];

public:
	bool wertenachrichtaus(mycan_frame* canMsg);

	bool isinUse();
	long forWichCAN_IDUsed();


	char topic[MAXLEN];
	byte payload[MAXLEN];
	uint8_t lenpayload = 0;
};

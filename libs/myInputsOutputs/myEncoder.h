#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basiskommunikation.h"
#include "basisinterface.h"
#include "myBasisHW.h"
#include "hilfe.h"
#include <Rotary.h>

class myEncoder : public BasisInterface
{
private:
	Rotary rot;                         /* the rotary object which will be created*/
	bool _encoderPinALast = LOW;
	bool _encoderPinANow = LOW;
	uint8_t _encoderPinA;
	uint8_t _encoderPinB; 
	uint8_t _encnr;
	myBasisHW* _Hardware;
	const char* _topic = "Enc/";
	Basiskommunikation* _mqtt;
 
public:
	 
	myEncoder(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t encoderPinA, uint8_t encoderPinB, uint8_t encodernummer);
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

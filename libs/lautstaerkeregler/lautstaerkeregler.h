#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basiskommunikation.h"
#include "basisinterface.h"
#include "hilfe.h"
#include "mytimer.h"
#include "myBasisHW.h"

class MyLSR : public BasisInterface
{
private:
	myBasisHW* _Hardware;
	myTimer TIM;
	const char* _MYMQTTANTWORTADR = "Vol/";
	Basiskommunikation* _mqtt;
	uint8_t _datapin;
	uint8_t _clockpin;
	uint8_t _nummer;
	uint8_t maxi = 30;
	uint8_t lot[30] = { 0,1,2,3,4,5,6,7,8,10,11,12,13,15,16,18,19,21,23,25,27,30,33,36,40,44,49,56,66,83 };
	int Volumen = 0;

	void setVolume(uint8_t volume);
	void sendvol();
public:
	MyLSR(Basiskommunikation* mqtt,myBasisHW* Hardware, uint8_t datapin, uint8_t clockpin, uint8_t nummer);
	void update();

	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

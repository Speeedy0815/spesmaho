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

class MyHell : public BasisInterface
{
private:
	const char* _MYMQTTANTWORTADR = "Hell";

	Basiskommunikation* _mqtt;
	myTimer TIM;
	uint32_t sendezeit = 120000;
	const int potPin = 34;
	void send(uint32_t val);
	uint32_t lastsent = 0;
	uint32_t dif_whentosend = 900;
public:
	MyHell(Basiskommunikation* mqtt);
	void update();

	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

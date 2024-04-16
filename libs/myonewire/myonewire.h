#define MYDEBUG 0
#include "mydebug.h"

#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <OneWire.h>
#include "basiskommunikation.h"
#include "basisinterface.h"
#include "hilfe.h"
#include "mytimer.h"

class XOnewire : public BasisInterface
{
public:
	XOnewire(byte onewirepin, Basiskommunikation* mqtt);
	XOnewire(byte onewirepin, Basiskommunikation* mqtt,bool useonytemperature);
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
private:
	const uint32_t sendebegrenzunginms_Ibutton = 2000;
	const uint32_t sendebegrenzunginms_Temperature = 30000;
	
	byte addr[20];
	OneWire _onew;
	Basiskommunikation* _mqtt;
	bool usekeys = true;

	myTimer TimerTemp;
	myTimer TimerSchl;
	bool button_darfsenden = true;
	 

	char sendebuffer[30];


	const char* _MYMQTTANTTEMP = "Temperaturen/";
	const char* _MYMQTTANTSCH = "Schluessel";

	void updateTempsensor();
	void updSchluesselandSend();
};



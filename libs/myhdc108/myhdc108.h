#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basiskommunikation.h"
#include "basisinterface.h"
#include "hilfe.h"
#include "mytimer.h"

#include <Wire.h>
#include "ClosedCube_HDC1080.h"




class MyHDC108 : public BasisInterface
{
public:
	MyHDC108(Basiskommunikation* mqtt);
	myTimer Sendetimer;
	myTimer Updatetimer;
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
	double getTemperature();
	double getHuminity();
	void sendeSensoren();
private:

	void updateSensoren();
	  ClosedCube_HDC1080 hdc1080;
	
	
 
	double lasttemp = 0.0;
	double lasthum = 0.0;


	const uint32_t sendebegrenzunginms_Temperature = 30000;
	const uint32_t updatetimerzeit = 1000;
	Basiskommunikation* _mqtt;

	char sendebuffer[30];
	const char* _MYMQTTANTTEMP = "Temp";
	const char* _MYMQTTANTHUMI = "Hum";

};

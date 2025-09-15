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
#include "mytimer.h"
#include "hilfe.h"


class MyBeep : public BasisInterface
{
private:  

	myTimer TIMER;
	uint16_t TIMERZeit = 5; //ms  --> achtung nicht �ndern, der Timer wird doppelt verwendet!!!!!


	int freq = 500;
	 
	const int resolution = 8;
	const int ledPin = 0;  
	uint8_t _pwmchannel = MYPINUNUSED;
	 
	Basiskommunikation * _mqtt;
	 

	uint16_t dauer = 0;
public: 
	MyBeep(Basiskommunikation* mqtt,uint8_t pwmchannel); 
	void update(); 
    
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

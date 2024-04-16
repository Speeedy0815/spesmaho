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
#include "hilfe.h"   //für  zahlaustextvonlen 
#include "myBasisHW.h"



class myOutput: public BasisInterface
{
	
private:
	Basiskommunikation* _mqtt;
	uint8_t _logischenummer;  //logische Pinnummer zum raussenden
	uint8_t _hardwarepinnummer;
	const   char* _topic = "Aus/";
	myBasisHW* _Hardware;
	bool _logiktauschen = false;
	bool _lastZustand = false;
 
	void toggle();
	void _sende_nachricht(bool an);
	
	uint32_t naechster_zykl = 0;
	uint32_t halbePeriode = 0;
	uint32_t Anzahltodo = 0;
	
 
public:
	
	void setState(bool State);
	void setTimer(uint32_t dauer);
	void setBlink(uint32_t HalfPeriod_ms, uint32_t periodes);
	void onoff(bool an);
	void refresh();
	

	
	myOutput(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t logischenummer, uint8_t hardwarepinnummer, bool logiktauschen);
	void update(); 
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
 

};

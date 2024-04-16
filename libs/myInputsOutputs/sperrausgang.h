#pragma once

#define MYDEBUG 0
#include "mydebug.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basisinterface.h"
#include "myBasisHW.h"

class SperrAusgang : public BasisInterface
{
private:  
	uint8_t * _adressliste;
	uint8_t _anzahl_ausgaenge;
	myBasisHW* _Hardware;
public: 
	SperrAusgang(myBasisHW* Hardware, uint8_t * Adressliste,uint8_t laenge);
	void update(); 
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

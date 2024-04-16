#pragma once
#define MYDEBUG 0
#include "mydebug.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basisinterface.h" 

class myRGBWbasis : public BasisInterface
{
private:  
	uint8_t _bandnummer = 0;
public: 

	myRGBWbasis(uint8_t bandnummer) { _bandnummer = bandnummer; };
	virtual void setWRGB(uint8_t nr,uint32_t wrgb ) = 0;
	virtual void update() = 0; 
    bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
	virtual void sethell(uint8_t helligkeit) = 0;
};

#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "basiskommunikation.h"
#include <DmxSimple.h>
#include "basisinterface.h"


class myDMXDimmer : public BasisInterface
{
private:  
	 
 	 
	 
public: 
	myDMXDimmer(uint8_t pinNr);
	void update(){}; 
    
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

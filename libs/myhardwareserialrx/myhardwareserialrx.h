#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include "basiskommunikation.h"
#include "basisinterface.h"
 


class MyHsRx : public BasisInterface
{
private:  
 	const char * _MYMQTTANTWORTADR= "BCode";;
	Basiskommunikation * _mqtt; 
 
  char zahl [30] = ""; 
  uint8_t ziffernzaehler = 0;  

public: 
	MyHsRx(Basiskommunikation* mqtt);
	void update(); 
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

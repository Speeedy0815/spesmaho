#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include "basiskommunikation.h"
#include "basisinterface.h"
#include <SoftwareSerial.h>


class MySsRx : public BasisInterface
{
private:  
 	const char * _MYMQTTANTWORTADR;
	Basiskommunikation * _mqtt; 
	SoftwareSerial _mySerial; // RX, TX
  char zahl [30] = ""; 
  uint8_t ziffernzaehler = 0;  

public: 
	MySsRx(Basiskommunikation* mqtt,const char * MYMQTTANTWORTADR,uint8_t RX_Pin);
	void update(); 
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

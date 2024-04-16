
#define MYDEBUG 0
#include "mydebug.h"

#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "basiskommunikation.h"
//#include "PinDefinitionsAndMore.h"

#include "basisinterface.h"
#include "myUhrBasis.h"
#include <ESP32Time.h>

extern ESP32Time ESPrtc;   
class myUhrESP : public myUhrBasis, public BasisInterface

{
private:
	bool _istgestellt = false;

public:
	void setUhrzeit(unsigned long  timestamp);
	uint32_t getUhrzeit();
	bool istgestellt();
	int getSecond();
	int getDay();
	String getTime(String format);
	myUhrESP(uint32_t zeitzone)  
	{
		ESPrtc.offset = zeitzone;
	};  


	void update() {};
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

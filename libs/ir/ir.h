
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



class MyIr : public BasisInterface
{
private:  
	 
	void send(uint8_t type, uint16_t aAddress, uint16_t aData_aCommand);
	Basiskommunikation * _mqtt;
	bool _enable_send = false;
	const char* _MYMQTTANTWORTADR = "IR";
public: 
	MyIr(Basiskommunikation* mqtt,uint8_t myREC_PIN,bool enable_send);
	void update(); 
    
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

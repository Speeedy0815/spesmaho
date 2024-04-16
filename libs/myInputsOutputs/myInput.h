#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include <inttypes.h>
#include "myDebouncer.h"
#include "basiskommunikation.h"


#include "OneButton.h"
#include "hilfe.h"
#include "mytimer.h"


 
class myInput
{
	
private:
	Basiskommunikation* _mqtt;
	uint8_t pinnummer;  //logische Pinnummer zum raussenden
	myDebouncer DB20;
	OneButton BUTT;

	const   char* _topic = "Tas/";

 

	//functions
	void mysendmessage(char* inhalt);
	void sendestate();
public:
	
	
	myInput(Basiskommunikation* mqtt, uint8_t pinnummer, bool activateLongPress);
	void update(bool currentState);
	void begin(bool currentState);
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
 

};

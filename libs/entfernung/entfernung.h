// entfernung.h

#ifndef _ENTFERNUNG_h
#define _ENTFERNUNG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


 
 

#include "hilfe.h"
#include "basiskommunikation.h"

class Entf: public BasisInterface 
{
public:
	Entf( Basiskommunikation* mqtt,uint8_t PINTRIGGER,uint8_t PINECHO,const char * MYMQTTANTWORTADR);
	void update();
private:
	uint8_t _PINTRIGGER;
	uint8_t _PINECHO;
	const uint32_t sendebegrenzunginms = 3600000;//einmal pro stunde
	const char * _MYMQTTANTWORTADR;
  
	Basiskommunikation* _mqtt;
	unsigned long lastmessage_sent = 0;
	 
	bool darfichsenden = true;
 
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
	

};
#endif





 
 


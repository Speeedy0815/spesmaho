// 
// 
// 

#include "entfernung.h"

bool Entf::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	return false; //nur senden, nichts empfangen
}
	


Entf::Entf( Basiskommunikation* mqtt,uint8_t PINTRIGGER,uint8_t PINECHO,const char * MYMQTTANTWORTADR)
{
	
	_PINTRIGGER = PINTRIGGER;
	_PINECHO = PINECHO;
	
	_MYMQTTANTWORTADR = MYMQTTANTWORTADR;
	pinMode(_PINTRIGGER, OUTPUT);
	pinMode(_PINECHO, INPUT);
	digitalWrite(_PINTRIGGER, HIGH); //Signal abschalten

	_mqtt = mqtt;


}
void Entf::update()
{

	unsigned long jetzt = millis();
	if (!darfichsenden)
	{
		if ((jetzt - lastmessage_sent) > sendebegrenzunginms)
		{
			darfichsenden = true;
		}
	}
	 

	if (darfichsenden)
	{
		    
	 
		long entfernung = 0;
		long zeit = 0;

		digitalWrite(_PINTRIGGER, LOW);
		delayMicroseconds(3);
		noInterrupts();
		digitalWrite(_PINTRIGGER, HIGH); //Trigger Impuls 10 us
		delayMicroseconds(10);
		digitalWrite(_PINTRIGGER, LOW);
		zeit = pulseIn(_PINECHO, HIGH,24000); // Echo-Zeit messen
		interrupts();

		zeit = (zeit / 2); // Zeit halbieren
		entfernung = zeit / 29.1; // Zeit in Zentimeter umrechnen
	 


		Serial.println(entfernung);

 
		char sendebuffer[33];

		strcpy(sendebuffer, "");


	 
			ltoa(entfernung, sendebuffer, 10);

			_mqtt->sendmessage(_MYMQTTANTWORTADR, sendebuffer);

			lastmessage_sent = jetzt;
			darfichsenden = false;
	}
	
}






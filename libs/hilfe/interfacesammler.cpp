#include "interfacesammler.h"
Interfacesammler::Interfacesammler(uint8_t maxanzahl)
{
	_max = maxanzahl;
	//Serial.print("Maxanzahl: ");
	//Serial.println(maxanzahl);
	interfaceliste = new BasisInterface* [maxanzahl];
}
void Interfacesammler::addInt(BasisInterface* Intf)
{
 
	 
	if (count >= _max)
	{
		Serial.println("Interfacesammler voll. Anzahl erhoehen!");
		while (1) { delay(1); } // Stoppe Controller
	}
	else
	{
		interfaceliste[count] = Intf;
		count++;
	}

}
void Interfacesammler::run()
{
 
	while (1)
	{
		for (uint8_t b = 0; b < count; b++)
		{
			#if MYDEBUG == 1
				debug("i: "); debug(b); debug(" : ");
				uint32_t vorher = millis();
			#endif
			 
			

			
			interfaceliste[b]->update();
			#if MYDEBUG == 1
				uint32_t nachher = millis();
				debugln(nachher - vorher);
			 #endif
		}
	};
}
void Interfacesammler::runcallback(char* topic, byte* payload, unsigned int length)
{
	debug("topic: "); debugln(topic);
	debug("payload: "); debugln((char*)payload);
	debug("length: "); debugln(length);

	for (uint8_t b = 0; b < count; b++)
	{
		if (true == interfaceliste[b]->callbackismineanddo(topic, payload, length))
		{
			debug(F("Ger nr: ")); debug(b); debugln(" angespr.");
			break;
		}
	}
}


 
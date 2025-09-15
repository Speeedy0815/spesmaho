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

void Interfacesammler::runonce()
{
 
   
        for (uint8_t b = 0; b < count; b++)
        {
            #if MYDEBUG >= 3
                debug("i: "); debug(b); debug(" : ");
                uint32_t vorher = millis();
            #endif
			#if MYDEBUG >= 2
			//Wifi sorgt für ständige änderung auf dem heap....deswegen musste ich es doch wegschaltern
            int *heapVariable = new int;
            int stackVariable;

            unsigned int currentHeapAddr = (unsigned int)heapVariable;
            unsigned int currentStackAddr = (unsigned int)&stackVariable;

            if (currentHeapAddr != lastHeapAddr || currentStackAddr != lastStackAddr)
            {
                Serial.print("Heap:");
                Serial.print(currentHeapAddr, HEX);
                Serial.print(" Stack:");
                Serial.println(currentStackAddr, HEX);

                lastHeapAddr = currentHeapAddr;
                lastStackAddr = currentStackAddr;
            }

            delete heapVariable;
			#endif
            interfaceliste[b]->update();

            #if MYDEBUG == 2
                uint32_t nachher = millis();
                debugln(nachher - vorher);
            #endif
        }
   
}

void Interfacesammler::run()
{
	#if MYDEBUG >= 1
    unsigned int lastHeapAddr = 0;
    unsigned int lastStackAddr = 0;
	#endif
	Serial.println("Start Cyclic");
    while (1)
    {
        this->runonce();
    };
}

void Interfacesammler::runcallback(char* topic, byte* payload, unsigned int length)
{
	//debug("topic: "); debugln(topic);
	//debug("payload: "); debugln((char*)payload);
	//debug("length: "); debugln(length);

	for (uint8_t b = 0; b < count; b++)
	{
		if (true == interfaceliste[b]->callbackismineanddo(topic, payload, length))
		{
			debug(F("Ger nr: ")); debug(b); debugln(" angespr.");
			break;
		}
	}
}


 
#include "myDMXDimmer.h"




myDMXDimmer::myDMXDimmer(uint8_t pinNr)
{
	DmxSimple.usePin(pinNr);
	
	
	//erste 11 Devices initialisieren, ggf. erhoehen
	for(uint8_t i = 0; i<10;i++)
	{
		DmxSimple.write(i, 0);
	}
	
	
	
	
}


bool  myDMXDimmer::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	//fixeLänge
	//Mcccvvv
	if (payload[0] != 'M')
		return false;
	if (length != 7)
	{
		Serial.print("LF: "); //Längefalsch
		Serial.println(length);
		return false;
	}
	


	int mychan = 0;
	uint8_t myvalue = 0;

	//Reihenfolge wichtig --> Hack
	myvalue = atoi((char*)payload + 4);
	payload[4] = 0; //String zwischendrin beenden
	mychan = atoi((char*)payload + 1);
 

	DmxSimple.write(mychan, myvalue);


	return true;
}

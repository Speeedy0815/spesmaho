#include "myUhrESP.h"


ESP32Time ESPrtc(0);  // offset in seconds GMT+1


void myUhrESP::setUhrzeit(unsigned long timestamp)
{
	ESPrtc.setTime(timestamp);
	_istgestellt = true;
}
uint32_t myUhrESP::getUhrzeit()
{}
bool myUhrESP::istgestellt()
{
	return _istgestellt;
}

String myUhrESP::getTime(String format)
{
	return (ESPrtc.getTime(format));
}



 


int myUhrESP::getSecond()
{
	return (ESPrtc.getSecond());
}
int myUhrESP::getDay()
{
	return (ESPrtc.getDay());
}

bool myUhrESP::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length <= 3)
		return false;
	if (payload[0] != 't')
		return false;
 

	debugln("Uhr");
	debugln((char*)payload);

	unsigned long zeitstempel = strtoul((char*)payload + 1, NULL, 10);
	debugln(zeitstempel);
	setUhrzeit(zeitstempel);


	debugln(ESPrtc.getTime("%A, %B %d %Y %H:%M:%S"));   // (String) returns time with specified format 

	debug("istgestellt: ");
	debugln(istgestellt());
	return true;
}

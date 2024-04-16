
#include "serialdummymqtt.h"


//##############################################################################
// ab Hier SerialDummyinterface




SerialDummy::SerialDummy(const char * MYMQTTNAME)
:
Basiskommunikation(MYMQTTNAME)
{
}


void SerialDummy::dopublish(const char* topic, const byte* msgbuffer,uint8_t len) 
{
	Serial.print("SenMes: ");
	Serial.print((char*)msgbuffer);
	Serial.print(" in Topic: ");
	Serial.println(topic);	 
} 





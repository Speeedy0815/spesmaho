
#include "w5100ethernetmqtt.h"



 
MqttCommunication::MqttCommunication(const char* server, uint8_t* mac, const char* MYMQTTNAME, const char* MYMQTTUS, const char* MYMQTTPW, uint8_t Resetpin, uint8_t CSPin)
	:
	EthernetMQTT(server, MYMQTTNAME, MYMQTTUS, MYMQTTPW, ethClient, 1883)
{
	init(mac, Resetpin, CSPin);
}
MqttCommunication::MqttCommunication(const char* server, uint8_t* mac, const char* MYMQTTNAME, const char* MYMQTTUS, const char* MYMQTTPW, uint8_t Resetpin)
	:
	EthernetMQTT(server, MYMQTTNAME, MYMQTTUS, MYMQTTPW, ethClient, 1883)
{
	init(mac, Resetpin, MYPINUNUSED);
}



void MqttCommunication::init(uint8_t* mac, uint8_t Resetpin, uint8_t cspin)
{
	if (MYPINUNUSED != Resetpin)
	{
		debugln(F("Reset Ethernet"));
		pinMode(Resetpin, OUTPUT);
		digitalWrite(Resetpin, 0);
		delay(700);
		digitalWrite(Resetpin, 1);
		delay(700);
	}
	SPI.begin();
	if (cspin != MYPINUNUSED)  //Originalpin verwenden
	{
		debug(F("Use CS Pin: "));
		debugln(cspin);
		Ethernet.init(cspin);
		
	}
	debugln(F("Start Ethernet begin"));
	
	const unsigned long dhcpTimeout       = 10000;  // z.B. max. 10 s auf DHCP warten
	const unsigned long responseTimeout   = 4000;   // Timeout pro Antwort

 
	if (Ethernet.begin(mac, dhcpTimeout, responseTimeout) == 0) {
		Serial.println(F("Failed DHCP"));

		if (Ethernet.hardwareStatus() == EthernetNoHardware) {
			debugln("Ethernet shield was not found."); // Sorry, can't run without hardware. :(
		}
		else if (Ethernet.linkStatus() == LinkOFF) {
			debugln("Ethernet cable is not connected.");
		}
		delay(3000);
	}

	Serial.print(F("My IP address: "));
	Serial.println(Ethernet.localIP());

	this->update(); //einmalig aufrufen zum verbinden
}





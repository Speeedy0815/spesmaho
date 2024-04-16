
#include "w5100ethernetmqtt.h"



//##############################################################################
// MqttCommunication  für W5100 Ethernetmodul
#if defined(ESP32) ||  defined(ESP8266)

void GLOGetETHMacFromWifiMac(uint8_t* puffermac)
{
	uint8_t mac[6];
	WiFi.macAddress(mac);
	for (uint8_t i = 0; i < 6; i++)
	{
		puffermac[i] = mac[i];
	}
	puffermac[2] += 1; //Unterschied für Ethernetmacadresse machen;
}
#endif

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

		pinMode(Resetpin, OUTPUT);
		digitalWrite(Resetpin, 0);
		delay(700);
		digitalWrite(Resetpin, 1);
		delay(700);
	}

	if (cspin != MYPINUNUSED)  //Originalpin verwenden
	{
		debug(F("Use CS Pin: "));
		debugln(cspin);
		Ethernet.init(cspin);
	}
	if (Ethernet.begin(mac) == 0) {
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






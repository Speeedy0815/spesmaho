#include "hilfe.h"




uint8_t plausibel(uint8_t zahl, uint8_t min, uint8_t max)
{
	if (zahl < min)
	{
		return 0;
	}
	if (zahl > max)
	{
		return 0;
	}
	return 1;
}
 

uint32_t zahlaustextvonlen(const char* eingabe, uint8_t stellebeginn, uint8_t len)
{

	char* hilfsstring = new char[len + 1]; // für Stringende
	memcpy(hilfsstring, eingabe + stellebeginn, len);
	hilfsstring[len] = 0;//Stringende
	uint32_t ergebnis = atol(hilfsstring);
	delete[] hilfsstring;
	return ergebnis;
}


void  GLOBmysendmessageWithAddr(Basiskommunikation* mqtt, const char* inhalt, const  char* topic, uint8_t addr)
{
	
	uint8_t ende = strlen(topic); // Todo
	char*  puffer;
	puffer = new char[ende + 2 + 1]; //2 für die Nummer + 1 Stringende

	strcpy(puffer, topic);

	puffer[ende] = (addr / 10) + 48;
	puffer[ende + 1] = (addr % 10) + 48;
	puffer[ende + 2] = 0x00; //Stringende

	mqtt->sendmessage(puffer, inhalt);
	delete[] puffer;
}




void GLOBcallback(char* topic, byte* payload, unsigned int length)
{
	 
	uint8_t len_t = strlen(topic);
	


	if (len_t >= GLOBmaxlentopic)
	{
		debugln("Top zu lang.");
		debugln(topic);
		return;
	}
	 
	if (length >= GLOBmaxlenpayload)
	{
		debug("Payload zu lang :");
		Serial.print(length);
		debug(" Zeichen");
		return;
	}


	char* puffertopic;
	puffertopic = new char[len_t + 1];
	byte* puffepayload;
	puffepayload = new byte[length + 1];

	strcpy(puffertopic, topic);
	memcpy(puffepayload, payload, length);
	puffepayload[length] = 0; // abschließen, falls String
	debug("empfange payload: ");debug((char*)puffepayload);	debug(" in: ");	debugln(puffertopic); debug(" Len: "); debugln(length);
	
	GlobInterfaces.runcallback(puffertopic, puffepayload, length);
 

	delete[] puffertopic;
	delete[] puffepayload;
	debugln("End");
}

void HAUPTSCHLEIFE()
{
	GlobInterfaces.run();

 

}
 

void globaleinitialisierung()
{
	//while (!Serial); --> Das hällt der ESP nicht aus, warum auch immer
	Serial.begin(115200);
	Serial.println();  //Die Schleife ist für den ESP Notwendig, warum auch immer startet er die Serielle schnittstelle nicht schnell genug
	for (uint8_t j = 0; j < 10; j++) { Serial.print(F("....")); }  Serial.println(F("."));
	debugln("Start");

	 
	Serial.print("Version: ");
	Serial.println(GLOBALVERSION); 

#if defined(ESP32)
	Serial.println(F("Disable BT and Wifi."));
	WiFi.mode(WIFI_OFF);
	btStop();
#endif





}

void GLOinitCS(uint8_t pin)
{
	//sagt einfach nur CS auf true --> um den SPI leer zu machen
	digitalWrite(pin, true);
	pinMode(pin, OUTPUT);
 
}
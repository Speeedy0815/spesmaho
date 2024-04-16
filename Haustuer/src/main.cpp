#include "w5100ethernetmqtt.h"

#include "settingsmqttsecret.h"
#include "phone.h"
#include "myonewire.h" 
#include "entfernung.h" 
#include "arduinowatchdog.h"
#include "myBasisHW_GPIO.h" //Hardware
#include "myInputs.h"
#include "myOutputs.h"

#include "hilfe.h"


Interfacesammler GlobInterfaces(11);


void setup()
{
	globaleinitialisierung();
  //###########################################################################################################################
  ArduinoHWWTD HWWTD; //warum ohne Klammern???? CCC? Das verstehe ich nicht, achtung an der Stelle habe ich das nicht getestet, könnte auch mit Klammern gehen
  GlobInterfaces.addInt(&HWWTD);
	//###########################################################################################################################
	byte MYMACADDR[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; 
	const char* MYMQTTNAME = "/Tuer";
	const uint8_t RESPINETHERNET = MYPINUNUSED;
	MqttCommunication mqtt(MYSERVERADDR, MYMACADDR, MYMQTTNAME, MYMQTTUSER, MYMQTTPASSW, RESPINETHERNET);
 
	//SerialDummy mqtt(MYMQTTNAME);
	GlobInterfaces.addInt(&mqtt);

	//###########################################################################################################################
	const uint8_t PINRELAIS = A6;
	const uint8_t PINCS = 2;
	const uint8_t PINDATA = 4;
	const uint8_t PINCLK = 3;
	Phone Telefon(PINRELAIS, PINCS, PINDATA, PINCLK);

	GlobInterfaces.addInt(&Telefon);
	//###########################################################################################################################
	const uint8_t ADDRESSE1WIREPIN = 7;

	XOnewire myonewire(ADDRESSE1WIREPIN, &mqtt);
	GlobInterfaces.addInt(&myonewire);
	//###########################################################################################################################
	const uint8_t PINTRIGGER = A5;
	const uint8_t PINECHO = A4;
	const char* MYMQTTENTFERNUNGSTRING = "Oelstand";
	Entf oelstand(&mqtt, PINTRIGGER, PINECHO, MYMQTTENTFERNUNGSTRING);
	GlobInterfaces.addInt(&oelstand);
	//###########################################################################################################################
	myBasisHW_GPIO HARDW;  //HARDW()         //warum ohne Klammern???? CCC? Das verstehe ich nicht
	GlobInterfaces.addInt(&HARDW);
	//###########################################################################################################################

	uint8_t       MCUSETTINGS1eingangssadressenlen = 4;
	uint8_t       MCUSETTINGS1eingangsadressenpin[] = { A0,A1,A2,A3 };  // bewmelder, taste auf incgeber
	uint8_t       MCUSETTINGS1eingangsadressenlog[] = { 0, 1,2,3 };
	bool       MCUSETTINGS1eingangactivatelong[] = { false,false,false,false };
	bool       MCUSETTINGS1turnlogic[] = { true,true,true,true };


	myInputs IN(&mqtt, &HARDW, MCUSETTINGS1eingangssadressenlen, MCUSETTINGS1eingangsadressenpin, MCUSETTINGS1eingangsadressenlog, MCUSETTINGS1eingangactivatelong, MCUSETTINGS1turnlogic);
	GlobInterfaces.addInt(&IN);
	//###########################################################################################################################
	const int   LAENGEAUSGAENGE = 10;
	uint8_t ADRESSENPINAUSGAENGE[] = { 6,5,14,15,16,17,18,19,20,21 };
	uint8_t     ADRESSENLOGISCHAUSGAENGE[] = { 0,1,2,3,4,5,6,7,8,9 };
	bool        AUSGAENGEDREHEN[] = { true,true,true,true,true,true,true,true,true,true };

	myOutputs OUT(&mqtt, &HARDW, LAENGEAUSGAENGE, ADRESSENPINAUSGAENGE, ADRESSENLOGISCHAUSGAENGE, AUSGAENGEDREHEN);
	GlobInterfaces.addInt(&OUT);
  
  //###########################################################################################################################
	WTD GLOBWathdog(&mqtt);
  GlobInterfaces.addInt(&GLOBWathdog);
	//###########################################################################################################################
	HAUPTSCHLEIFE();
}
void loop() {}



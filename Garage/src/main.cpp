#include "w5100ethernetmqtt.h"
#include "settingsmqttsecret.h"
#include "serialdummymqtt.h"
#include "sperrausgang.h"

#include "arduinowatchdog.h"
#include "myonewire.h" 
#include "canumsetzer.h"
#include "myBasisHW_MCP.h" //Hardware an einem MCP

#include "myBasisHW_GPIO.h" //Hardware
#include "myOutputs.h"
#include "myInputs.h"



#include "hilfe.h"
#include "basisinterface.h"
#include "canhardMCP2515.h"

Interfacesammler GlobInterfaces(16);


void setup()
{
	globaleinitialisierung();  
  //###########################################################################################################################
  ArduinoHWWTD HWWTD; //warum ohne Klammern???? CCC? Das verstehe ich nicht, achtung an der Stelle habe ich das nicht getestet, könnte auch mit Klammern gehen
  GlobInterfaces.addInt(&HWWTD);
	//###########################################################################################################################
	byte MYMACADDR[6] = { 0x74,0x62,0x35,0x34,0x10,0x75 };
	//byte MYSERVERADDR[4] = { 192, 168, 1, 117 };
	
	const char* MYMQTTNAME = "/Garage";
	const uint8_t RESPINETHERNET = 9;
	MqttCommunication mqtt(MYSERVERADDR, MYMACADDR, MYMQTTNAME, MYMQTTUSER, MYMQTTPASSW, RESPINETHERNET);
 
	//SerialDummy mqtt(MYMQTTNAME);
	GlobInterfaces.addInt(&mqtt);
	//###########################################################################################################################
	const uint8_t CS_RXpin = 8;
	const uint8_t IRQ_TXpin = 2;   //Achtung Hardwarefehler (Originalpin 7 ist nicht interruptfähig!!!!!)
	const long geschwindigkeit = 125E3;

	canhardMCP2515 canHW(CS_RXpin, IRQ_TXpin, geschwindigkeit);
	CanUmsetzer UMS(&mqtt, &canHW);
	GlobInterfaces.addInt(&UMS);
	//##########################################################################################################################
	const uint8_t ADDRESSE1WIREPIN = 49;
	
	XOnewire myonewire(ADDRESSE1WIREPIN, &mqtt);
	GlobInterfaces.addInt(&myonewire);
	//###########################################################################################################################
	myBasisHW_GPIO HARDW;  //HARDW()         //warum ohne Klammern???? CCC? Das verstehe ich nicht
	GlobInterfaces.addInt(&HARDW);
	//###########################################################################################################################
	const uint8_t LAENGESPERRAUSGAENGE = 4;
	uint8_t ADRESSENAUSGAENGE[] = { 39,45,43,41 };

	SperrAusgang Audioausgaenge(&HARDW,ADRESSENAUSGAENGE, LAENGESPERRAUSGAENGE);
	GlobInterfaces.addInt(&Audioausgaenge);

	//###########################################################################################################################
	myBasisHW_MCP_Resetit(A12); //Resetpin ist an A12 Angeschlossen
	//###########################################################################################################################
	const uint8_t ADDRMCP1 = 0;

	myBasisHW_MCP MCP1(ADDRMCP1);
	GlobInterfaces.addInt(&MCP1);
	//###########################################################################################################################
	const uint8_t ADDRMCP2 = 1;
	
	myBasisHW_MCP MCP2(ADDRMCP2);
	GlobInterfaces.addInt(&MCP2);

	//###########################################################################################################################
	uint8_t       MCUSETTINGS1eingangssadressenlen = 16;
	uint8_t       MCUSETTINGS1eingangsadressenpin[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };  // bewmelder, taste auf incgeber
	uint8_t       MCUSETTINGS1eingangsadressenlog[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	bool       MCUSETTINGS1eingangactivatelong[] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false };
	bool       MCUSETTINGS1turnlogic[] = { true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true };

	myInputs IN(&mqtt, &MCP1, MCUSETTINGS1eingangssadressenlen, MCUSETTINGS1eingangsadressenpin, MCUSETTINGS1eingangsadressenlog, MCUSETTINGS1eingangactivatelong, MCUSETTINGS1turnlogic);
	GlobInterfaces.addInt(&IN);
	//###########################################################################################################################
	const int   LAENGEAUSGAENGE = 16;
	uint8_t     ADRESSENHWAUSGAENGE[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	uint8_t     ADRESSENLOGISCHAUSGAENGE[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	bool        AUSGAENGEDREHEN[] = { true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true };

	myOutputs OUT(&mqtt, &MCP2, LAENGEAUSGAENGE, ADRESSENHWAUSGAENGE, ADRESSENLOGISCHAUSGAENGE, AUSGAENGEDREHEN);
	GlobInterfaces.addInt(&OUT);
	//###########################################################################################################################

	const int   LAENGEAUSGAENGEGPIO = 2;
	uint8_t     ADRESSENHWAUSGAENGEGPIO[] = {  46,44};
	uint8_t     ADRESSENLOGISCHAUSGAENGEGPIO[] = { 16,17 };
	bool        AUSGAENGEDREHENGPIO[] = { false,false };

	myOutputs OUTHW(&mqtt, &HARDW, LAENGEAUSGAENGEGPIO, ADRESSENHWAUSGAENGEGPIO, ADRESSENLOGISCHAUSGAENGEGPIO, AUSGAENGEDREHENGPIO);
	GlobInterfaces.addInt(&OUTHW);

	//###########################################################################################################################
 
	 

	HAUPTSCHLEIFE();
}
void loop() {}

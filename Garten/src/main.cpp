#include "w5100ethernetmqtt.h"
#include "settingsmqttsecret.h"
#include "serialdummymqtt.h"
#include "sperrausgang.h"
#include "arduinowatchdog.h"
#include "myonewire.h" 
#include "myBasisHW_MCP.h" //Hardware an einem MCP

#include "myBasisHW_GPIO.h" //Hardware
#include "myOutputs.h"
#include "myInputs.h"



#include "hilfe.h"
#include "basisinterface.h"


Interfacesammler GlobInterfaces(16);


void setup()
{
	globaleinitialisierung();
  //###########################################################################################################################
  ArduinoHWWTD HWWTD; //warum ohne Klammern???? CCC? Das verstehe ich nicht, achtung an der Stelle habe ich das nicht getestet, könnte auch mit Klammern gehen
  GlobInterfaces.addInt(&HWWTD);
	//###########################################################################################################################
	byte MYMACADDR[6] = { 0x74,0x62,0x35,0x34,0x10,0 };
	  
	const char* MYMQTTNAME = "/Garten";
	const uint8_t RESPINETHERNET = 9;
	MqttCommunication mqtt(MYSERVERADDR, MYMACADDR, MYMQTTNAME, MYMQTTUSER, MYMQTTPASSW, RESPINETHERNET);
 
	GlobInterfaces.addInt(&mqtt);
	//###########################################################################################################################

	const uint8_t ADDRESSE1WIREPIN = 49;
	
	XOnewire myonewire(ADDRESSE1WIREPIN, &mqtt);
	GlobInterfaces.addInt(&myonewire);

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
	const uint8_t ADDRMCP3 = 2;
	
	myBasisHW_MCP MCP3(ADDRMCP3);
	GlobInterfaces.addInt(&MCP3);
	//###########################################################################################################################
	myBasisHW_GPIO HARDW;  //HARDW()         //warum ohne Klammern???? CCC? Das verstehe ich nicht
	GlobInterfaces.addInt(&HARDW);
	//###########################################################################################################################
	uint8_t       MCUSETTINGS1eingangssadressenlen = 16;
	uint8_t       MCUSETTINGS1eingangsadressenpin[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };  
	uint8_t       MCUSETTINGS1eingangsadressenlog[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	bool       MCUSETTINGS1eingangactivatelong[] = { true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true };
	bool       MCUSETTINGS1turnlogic[] = { true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true };

	myInputs IN(&mqtt, &MCP1, MCUSETTINGS1eingangssadressenlen, MCUSETTINGS1eingangsadressenpin, MCUSETTINGS1eingangsadressenlog, MCUSETTINGS1eingangactivatelong, MCUSETTINGS1turnlogic);
	GlobInterfaces.addInt(&IN);
	//###########################################################################################################################
	const int   LAENGEAUSGAENGE = 16;
	uint8_t     ADRESSENHWAUSGAENGE[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	uint8_t     ADRESSENLOGISCHAUSGAENGE[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	bool        AUSGAENGEDREHEN[] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false };

	myOutputs OUT2(&mqtt, &MCP2, LAENGEAUSGAENGE, ADRESSENHWAUSGAENGE, ADRESSENLOGISCHAUSGAENGE, AUSGAENGEDREHEN);
	GlobInterfaces.addInt(&OUT2);
		//###########################################################################################################################
	const int   LAENGEAUSGAENGE3 = 16;
	uint8_t     ADRESSENHWAUSGAENGE3[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	uint8_t     ADRESSENLOGISCHAUSGAENGE3[] = { 16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
	bool        AUSGAENGEDREHEN3[] = { true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true };

	myOutputs OUT3(&mqtt, &MCP3, LAENGEAUSGAENGE3, ADRESSENHWAUSGAENGE3, ADRESSENLOGISCHAUSGAENGE3, AUSGAENGEDREHEN3);
	GlobInterfaces.addInt(&OUT3);
	
	//###########################################################################################################################

 	uint8_t       MCUSETTINGS1eingangssadressenlen2 = 2;  // die 2 Taster
	uint8_t       MCUSETTINGS1eingangsadressenpin2[] = { 31,33 };  
	uint8_t       MCUSETTINGS1eingangsadressenlog2[] = { 16,17 };
	bool       MCUSETTINGS1eingangactivatelong2[] = { false,false };
	bool       MCUSETTINGS1turnlogic2[] = { true,true  };

	myInputs IN2(&mqtt, &HARDW, MCUSETTINGS1eingangssadressenlen2, MCUSETTINGS1eingangsadressenpin2, MCUSETTINGS1eingangsadressenlog2, MCUSETTINGS1eingangactivatelong2, MCUSETTINGS1turnlogic2);
	GlobInterfaces.addInt(&IN2);

	//###########################################################################################################################
	
	WTD GLOBWathdog(&mqtt);
	GlobInterfaces.addInt(&GLOBWathdog);
	 //###########################################################################################################################

	HAUPTSCHLEIFE();
}
void loop() {}

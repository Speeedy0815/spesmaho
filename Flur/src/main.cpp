#include "hilfe.h"
#include "w5100ethernetmqtt.h"
#include "settingsmqttsecret.h"
#include "serialdummymqtt.h"
#include "sperrausgang.h"
#include "lautstaerkeregler.h"
#include "arduinowatchdog.h"
#include "myonewire.h" 
#include "canumsetzer.h"
#include "myBasisHW_MCP.h" //Hardware an einem MCP
#include "mytastermatrix.h"
#include "arduinowatchdog.h"
#include "ledinterfacews2812.h"
#include "myBasisHW_GPIO.h" //Hardware
#include "myOutputs.h"
#include "myInputs.h"
#include "mypwmled.h"

#include "canbasis.h"
#include "canhardMCP2515.h"

#include "hilfe.h"
#include "basisinterface.h"
#include "interfacesammler.h"

Interfacesammler GlobInterfaces(16);

 



void setup()
{
	globaleinitialisierung();

  //###########################################################################################################################
  ArduinoHWWTD HWWTD; //warum ohne Klammern???? CCC? Das verstehe ich nicht, achtung an der Stelle habe ich das nicht getestet, könnte auch mit Klammern gehen
  GlobInterfaces.addInt(&HWWTD);
	//###########################################################################################################################
	byte MYMACADDR[6] = { 0x74,0x82,0x33,0x34,0x11,0x78 }; 
	const char* MYMQTTNAME = "/Flur";
	const uint8_t RESPINETHERNET = 9;

	MqttCommunication mqtt(MYSERVERADDR, MYMACADDR, MYMQTTNAME, MYMQTTUSER, MYMQTTPASSW, RESPINETHERNET);
	//SerialDummy mqtt(MYMQTTNAME);
	GlobInterfaces.addInt(&mqtt);
	//###########################################################################################################################
	const long geschwindigkeit = 125E3;
	const uint8_t CS_RXpin = 8;
	const uint8_t IRQ_TXpin = 18;   //Achtung Hardwarefehler (Originalpin 7 ist nicht interruptfähig!!!!!)


  //achtung, Canbusgeschwindikeit wirkt derzeit nicht!!!!
	canhardMCP2515 canHW(CS_RXpin, IRQ_TXpin, geschwindigkeit);
  
	CanUmsetzer UMS(&mqtt, &canHW);
	GlobInterfaces.addInt(&UMS);
	//##########################################################################################################################
	const uint8_t BANDNO0 = 0;
	const uint8_t BANDNO1PinR = 12;
	const uint8_t BANDNO1PinG = 3;
	const uint8_t BANDNO1PinB = 5;
	const uint8_t BANDNO1PinW = 6;

	mypwmled LEDBAND(BANDNO0, BANDNO1PinR, BANDNO1PinG, BANDNO1PinB, BANDNO1PinW);
	GlobInterfaces.addInt(&LEDBAND);
	//###########################################################################################################################
	myBasisHW_MCP_Resetit(A12); //Resetpin ist an A12 Angeschlossen
	//###########################################################################################################################
	const uint8_t ADDRMCP1 = 0;

	myBasisHW_MCP MCP1(ADDRMCP1);
	GlobInterfaces.addInt(&MCP1);
	//###########################################################################################################################
	const uint8_t MTXzeil1 = 3;
	const uint8_t MTXspal1 = 3;
	uint8_t       MTXZeilP1[] = { 2,3,4 };
	uint8_t       MTXSpalP1[] = { 7,6,5 };
	uint8_t MTXkeys1[MTXzeil1 * MTXspal1] = { 6,3,0,7,4,1,8,5,2 };

	myTastermatrix MTX1(&mqtt, &MCP1, MTXspal1, MTXzeil1, MTXSpalP1, MTXZeilP1, MTXkeys1);
	GlobInterfaces.addInt(&MTX1);
	//###########################################################################################################################
	const uint8_t MTXzeil2 = 3;
	const uint8_t MTXspal2 = 3;
	uint8_t       MTXZeilP2[] = { 13,12,11 };
	uint8_t       MTXSpalP2[] = { 8,9,10 };

	uint8_t MTXkeys2[MTXzeil2 * MTXspal2] = { 13,16,10,14,17,11,15,18,12 };

	myTastermatrix MTX2(&mqtt, &MCP1, MTXspal2, MTXzeil2, MTXSpalP2, MTXZeilP2, MTXkeys2);
	GlobInterfaces.addInt(&MTX2);
	//###########################################################################################################################
	uint8_t       MCUSETTINGS1eingangssadressenlen = 4;
	uint8_t       MCUSETTINGS1eingangsadressenpin[] = { 0,1,14,15 };

	//Bewegungsmelder, Klingel,????,Wohnungstür(Endlage)
	uint8_t       MCUSETTINGS1eingangsadressenlog[] = { 9,19,20,21 };
	bool       MCUSETTINGS1turnlogic[] = { false,true,true,true };
	bool       MCUSETTINGS1eingangactivatelong[] = { true,true,false,false };

	myInputs IN(&mqtt, &MCP1, MCUSETTINGS1eingangssadressenlen, MCUSETTINGS1eingangsadressenpin, MCUSETTINGS1eingangsadressenlog, MCUSETTINGS1eingangactivatelong, MCUSETTINGS1turnlogic);
	GlobInterfaces.addInt(&IN);
	//###########################################################################################################################
	const uint8_t BANDNO1 = 1;
	const uint8_t ANZAHLLEDs1 = 9;
	const uint8_t MAXBRIGHTNESS1 = 60;
	const uint8_t LEDWS28PIN1 = 31;
	const bool SpeichereWertefuerHelligkeitsaenderung1 = true;

	LEDW2812Interface LEDs1(BANDNO1, ANZAHLLEDs1, LEDWS28PIN1, MAXBRIGHTNESS1, SpeichereWertefuerHelligkeitsaenderung1);
	GlobInterfaces.addInt(&LEDs1);
	//###########################################################################################################################

	const uint8_t BANDNO2 = 2;
	const uint8_t ANZAHLLEDs2 = 9;
	const uint8_t MAXBRIGHTNESS2 = 60;
	const uint8_t LEDWS28PIN2 = 33;

	LEDW2812Interface LEDs2(BANDNO2, ANZAHLLEDs2, LEDWS28PIN2, MAXBRIGHTNESS2, SpeichereWertefuerHelligkeitsaenderung1);
	GlobInterfaces.addInt(&LEDs2);
	//###########################################################################################################################

	const bool SpeichereWertefuerHelligkeitsaenderung2 = false;
	const uint8_t BANDNO3 = 3;
	const uint8_t ANZAHLLEDs3 = 56;
	const uint8_t MAXBRIGHTNESS3 = 60;
	const uint8_t LEDWS28PIN3 = 47;

	LEDW2812Interface LEDs3(BANDNO3, ANZAHLLEDs3, LEDWS28PIN3, MAXBRIGHTNESS3, SpeichereWertefuerHelligkeitsaenderung2);
	GlobInterfaces.addInt(&LEDs3);
	//###########################################################################################################################
	const uint8_t ADDRESSE1WIREPIN = 49;
  bool nur_Temperaturen = true;

	XOnewire myonewire(ADDRESSE1WIREPIN, &mqtt,nur_Temperaturen);
	GlobInterfaces.addInt(&myonewire);
	//###########################################################################################################################
	WTD GLOBWathdog(&mqtt);
	GlobInterfaces.addInt(&GLOBWathdog);
	//###########################################################################################################################

	myBasisHW_GPIO HARDW;  //HARDW()         //warum ohne Klammern???? CCC? Das verstehe ich nicht
	GlobInterfaces.addInt(&HARDW);

	//###########################################################################################################################
	const int   LAENGEAUSGAENGE3 = 1;
	uint8_t     ADRESSENHWAUSGAENGE3[] = { A13 };
	uint8_t     ADRESSENLOGISCHAUSGAENGE3[] = { 0 };
	bool        AUSGAENGEDREHEN3[] = { false };

	//###########################################################################################################################

	//Eulen-LED
	myOutputs OUT3(&mqtt, &HARDW, LAENGEAUSGAENGE3, ADRESSENHWAUSGAENGE3, ADRESSENLOGISCHAUSGAENGE3, AUSGAENGEDREHEN3);
	GlobInterfaces.addInt(&OUT3);

	//###########################################################################################################################


	HAUPTSCHLEIFE();
}
void loop() {}

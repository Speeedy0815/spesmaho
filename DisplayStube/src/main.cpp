

#include "myheiztemp.h"
#include "mydisplay.h"
#include "mybeeper.h"
#include "myEncoderESP.h"
#include "myhelligkeit.h"
#include "ledinterfacews2812.h"
#include "ir.h"
#include "mytastermatrix.h"
#include "myInputs.h"
#include "myBasisHW_GPIO.h" //Hardware
//
#include "myBasisHW_MCP.h" //Hardware an einem MCP
#include "myhdc108.h"  //Temperatursensor + Luftfeuchte
#include "myUhrESP.h"	//Echtzeituhr ESP32
#include "serialdummymqtt.h"
#include "hilfe.h"
#include "basisinterface.h"
#include "arduinowatchdog.h"
#include "espwifimqtt.h"
#include "settingsmqttsecret.h"
#include "myotaupdater.h"
// Globale Settings
#include "mydisplaybeleuchter.h"


#include "canmqttclient.h"
#include "canbasis.h"
#include "canhardESP.h"
#include "mydisplayHeizungsteil.h"



Interfacesammler GlobInterfaces(20);

void setup()
{

	globaleinitialisierung();


	//const uint8_t CANADDRESSE = 0;  // Garage (an Garagenplatine)
  
	//const uint8_t CANADDRESSE = 0;  // Stube
	//const uint8_t CANADDRESSE = 1;  // kl Bad
	//const uint8_t CANADDRESSE = 2;  // Jace
	//const uint8_t CANADDRESSE = 3;  // Jack
	//const uint8_t CANADDRESSE = 4;  // SZ
	//const uint8_t CANADDRESSE = 5;  // MZ
	//const uint8_t CANADDRESSE = 6;  // gr Bad
	//const uint8_t CANADDRESSE = 7;  // WR




	const char* MYMQTTNAME = "/tester";



	canSettingloader CANaddrloader;  // auch hier wieder ohne Klammern, verstehe ich nicht
	const uint32_t geschwindigkeit = 125000; // scheint nicht zu wirken
	const uint8_t CANCS_RXpin = 16;
	const uint8_t CANIRQ_TXpin = 17;
	canhardESP canHW(CANCS_RXpin, CANIRQ_TXpin, geschwindigkeit);



	CANMQTT mqtt(CANaddrloader.getAddr(), &canHW); //nummer, rx und tx pin
	//ESP_Wifi_MQTT mqtt(MYSERVERADDR, MYMQTTNAME, MYMQTTUSER, MYMQTTPASSW, WIFISSID, WIFIPW);
	//SerialDummy mqtt(MYMQTTNAME);

	GlobInterfaces.addInt(&mqtt);
	GlobInterfaces.addInt(&CANaddrloader);
	//###########################################################################################################################
	myBasisHW_GPIO HARDW;  //HARDW()         //warum ohne Klammern???? CCC? Das verstehe ich nicht
	GlobInterfaces.addInt(&HARDW);
	//###########################################################################################################################
	const uint8_t ADDRMCP1 = 0;
	myBasisHW_MCP MCP1(ADDRMCP1);

	GlobInterfaces.addInt(&MCP1);
	////###########################################################################################################################
	const uint8_t MTXzeil = 3;
	const uint8_t MTXspal = 3;
	uint8_t       MTXZeilP[] = { 2,3,4 };
	uint8_t       MTXSpalP[] = { 5,6,7 };
	uint8_t MTXkeys[MTXzeil * MTXspal] = { 6,3,0,7,4,1,8,5,2 };

	myTastermatrix MTX(&mqtt, &MCP1, MTXspal, MTXzeil, MTXSpalP, MTXZeilP, MTXkeys);
	GlobInterfaces.addInt(&MTX);
	//###########################################################################################################################
	uint8_t       MCUSETTINGS1eingangssadressenlen = 2;
	uint8_t       MCUSETTINGS1eingangsadressenpin[] = { 8,11 };  // bewmelder, taste auf incgeber
	uint8_t       MCUSETTINGS1eingangsadressenlog[] = { 10,9 };
	bool       MCUSETTINGS1eingangactivatelong[] = { false,true };

	bool       MCUSETTINGS1turnlogic[] = { false,true };

	myInputs IN(&mqtt, &MCP1, MCUSETTINGS1eingangssadressenlen, MCUSETTINGS1eingangsadressenpin, MCUSETTINGS1eingangsadressenlog, MCUSETTINGS1eingangactivatelong, MCUSETTINGS1turnlogic);
	GlobInterfaces.addInt(&IN);
	//###########################################################################################################################
	const uint8_t ENCODERNUMMER = 0;
	uint8_t ENCODERPINA = 27;
	uint8_t ENCODERPINB = 26;


	myEncoderESP ENC(&mqtt, ENCODERPINA, ENCODERPINB, ENCODERNUMMER);

	GlobInterfaces.addInt(&ENC);
	//###########################################################################################################################
	const uint8_t IRREC_PIN = 32;
	const bool IR_ENABLE_SEND = false; //PIN is fix

	MyIr IRGeraet(&mqtt, IRREC_PIN, IR_ENABLE_SEND);
	GlobInterfaces.addInt(&IRGeraet);
	//###########################################################################################################################
	MyHell HELL(&mqtt);
	GlobInterfaces.addInt(&HELL);
	//###########################################################################################################################
	const uint8_t BANDNO1 = 0;
	const uint8_t ANZAHLLEDs1 = 1;
	const uint8_t MAXBRIGHTNESS1 = 50;
	const uint8_t LEDWS28PIN1 = 25;
	const bool SpeichereWertefuerHelligkeitsaenderung = true;

	LEDW2812Interface LEDs1(BANDNO1, ANZAHLLEDs1, LEDWS28PIN1, MAXBRIGHTNESS1, SpeichereWertefuerHelligkeitsaenderung);
	GlobInterfaces.addInt(&LEDs1);
	//###########################################################################################################################
	const uint8_t BANDNO2 = 1;
	const uint8_t ANZAHLLEDs2 = 9;
	const uint8_t MAXBRIGHTNESS2 = 60;
	const uint8_t LEDWS28PIN2 = 12;

	LEDW2812Interface LEDs2(BANDNO2, ANZAHLLEDs2, LEDWS28PIN2, MAXBRIGHTNESS2, SpeichereWertefuerHelligkeitsaenderung);
	GlobInterfaces.addInt(&LEDs2);
	//###########################################################################################################################
	MyBeep BEEP(&mqtt);
	GlobInterfaces.addInt(&BEEP);
	//###########################################################################################################################
	MyHDC108 TEMPHUM(&mqtt);
	GlobInterfaces.addInt(&TEMPHUM);


	//###########################################################################################################################	
	uint32_t zeitzone = 3600;

	myUhrESP Uhr(zeitzone);
	GlobInterfaces.addInt(&Uhr);
	//##########################################################################################################################
	const uint8_t fussbodenp = 12;
	const uint8_t heizkoerperpin = 13;

	HeizTemp Heizungsreglung(&mqtt, &MCP1, &TEMPHUM, fussbodenp, heizkoerperpin);
	GlobInterfaces.addInt(&Heizungsreglung);

	//##########################################################################################################################

	const uint8_t cs_display = 15;
	const uint8_t resPin = 4;


	const uint8_t ledPinhintergrundbel = 5;
	PWMBeleuchtungDisplay BEL(ledPinhintergrundbel);

	MyDisp DISP(&mqtt, &BEL, &ENC, &Uhr, resPin, cs_display);
	GlobInterfaces.addInt(&DISP);


	DisplayHeizungsteil HZ(&DISP, &TEMPHUM, &Heizungsreglung);
	GlobInterfaces.addInt(&HZ);


	//##########################################################################################################################
	bool onlyuseresetter = false;
  uint32_t Resetzeit = 300000; 
	WTD GLOBWathdog(&mqtt, onlyuseresetter,Resetzeit);
	GlobInterfaces.addInt(&GLOBWathdog);
	//###########################################################################################################################
   
  const char* Firmwarename = "DisplayStube.bin";
	OTAUpdater OTA(&mqtt,Firmwarename);
	OTA.setupdateW(&DISP);
	GlobInterfaces.addInt(&OTA);


	DISP.setOTAUpdater(&OTA);
	//###########################################################################################################################


	HAUPTSCHLEIFE();

}
void loop() {}

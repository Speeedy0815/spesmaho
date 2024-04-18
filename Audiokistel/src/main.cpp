


#include "w5100ethernetmqtt.h"


#include "myEncoderESP.h"
#include "ledinterfacews2812.h"
#include "ir.h" 
#include "myInputs.h"
#include "myBasisHW_GPIO.h" //Hardware

#include "myBasisHW_MCP.h" //Hardware an einem MCP 
#include "myUhrESP.h"	//Echtzeituhr ESP32
#include "serialdummymqtt.h"
#include "hilfe.h"
#include "basisinterface.h"
#include "arduinowatchdog.h"
#include "espwifimqtt.h"
#include "settingsmqttsecret.h"
#include "myotaupdater.h"
#include "myOutputs.h"
#include "sperrausgang.h"
#include "mySpiffsSettingsloader.h"
#include "lautstaerkeregler.h"
#include "myDMXDimmerESP.h"
// Globale Settings



#include "canmqttclient.h"
#include "canbasis.h"
#include "canhardESP.h"



#include "mydisplay.h"
#include "mydisplaybeleuchter.h"
#include "mydisplayAudioteil.h"

Interfacesammler GlobInterfaces(20);

void setup()
{

	globaleinitialisierung();
	const uint8_t CSETHERNET = 0;
	const int8_t CS_TFT = 4;
	const int8_t RES_TFT = -1;
	const uint8_t RESPINETHERNET = 26;
	const char* filepfad = "/MQTTName";
	const char* stdnamewennnichhstgesetztist = "/irgendeingeraet";

	GLOinitCS(CS_TFT);	//CS TFT
	GLOinitCS(33);	//CS Touch
	GLOinitCS(CSETHERNET);	//CS Ethernet

	uint8_t MYMACADDR[6];
	GLOGetETHMacFromWifiMac(MYMACADDR);


	mySpiffsSettingsloaderString Adressloader(filepfad, stdnamewennnichhstgesetztist);  // auch hier wieder ohne Klammern, verstehe ich nicht

	//CANMQTT mqtt(CANaddrloader.getAddr(), &canHW); //nummer, rx und tx pin
	//ESP_Wifi_MQTT mqtt(MYSERVERADDR, Adressloader.getSett(), MYMQTTUSER, MYMQTTPASSW, WIFISSID, WIFIPW);
	SerialDummy mqtt(Adressloader.getSett());
	//MqttCommunication mqtt(MYSERVERADDR, MYMACADDR, Adressloader.getSett(), MYMQTTUSER, MYMQTTPASSW, RESPINETHERNET, CSETHERNET);


	GlobInterfaces.addInt(&mqtt);
	GlobInterfaces.addInt(&Adressloader);

	//###########################################################################################################################
	myBasisHW_GPIO HARDW;  //HARDW()         //warum ohne Klammern???? CCC? Das verstehe ich nicht
	GlobInterfaces.addInt(&HARDW);
	//###########################################################################################################################
	const uint8_t ADDRMCP1 = 0;
	myBasisHW_MCP MCP1(ADDRMCP1);

	GlobInterfaces.addInt(&MCP1);

	//###########################################################################################################################
	uint8_t       MCUSETTINGS1eingangssadressenlen = 6;
	uint8_t       MCUSETTINGS1eingangsadressenpin[] = { 2,3,4,13,12,5 };  //5 Taster + Taster auf INC Geber
	uint8_t       MCUSETTINGS1eingangsadressenlog[] = { 0,1,2,3,4,5 };
	bool       MCUSETTINGS1eingangactivatelong[] = { true,true,true,true ,true ,true };

	bool       MCUSETTINGS1turnlogic[] = { true,true,true,true ,true ,true };

	myInputs IN(&mqtt, &MCP1, MCUSETTINGS1eingangssadressenlen, MCUSETTINGS1eingangsadressenpin, MCUSETTINGS1eingangsadressenlog, MCUSETTINGS1eingangactivatelong, MCUSETTINGS1turnlogic);
	GlobInterfaces.addInt(&IN);
	//###########################################################################################################################
	const uint8_t ENCODERNUMMER = 0;
	uint8_t ENCODERPINA = 36;
	uint8_t ENCODERPINB = 39;


	myEncoderESP ENC(&mqtt, ENCODERPINA, ENCODERPINB, ENCODERNUMMER);

	GlobInterfaces.addInt(&ENC);
	//###########################################################################################################################
	const uint8_t IRREC_PIN = 34;
	const bool IR_ENABLE_SEND = false; //PIN is fix

	MyIr IRGeraet(&mqtt, IRREC_PIN, IR_ENABLE_SEND);
	GlobInterfaces.addInt(&IRGeraet);

	//###########################################################################################################################
	const uint8_t LAENGESPERRAUSGAENGE = 4;
	uint8_t ADRESSENAUSGAENGE[] = { 8,9,10,11 };

	SperrAusgang Audioausgaenge(&MCP1, ADRESSENAUSGAENGE, LAENGESPERRAUSGAENGE);

	GlobInterfaces.addInt(&Audioausgaenge);

	//###########################################################################################################################
	const uint8_t BANDNO1 = 0;
	const uint8_t ANZAHLLEDs1 = 1;
	const uint8_t MAXBRIGHTNESS1 = 50;
	const uint8_t LEDWS28PIN1 = 12;
	const bool SpeichereWertefuerHelligkeitsaenderung = true;

	LEDW2812Interface LEDs1(BANDNO1, ANZAHLLEDs1, LEDWS28PIN1, MAXBRIGHTNESS1, SpeichereWertefuerHelligkeitsaenderung);
	GlobInterfaces.addInt(&LEDs1);

	//###########################################################################################################################	
	uint32_t zeitzone = 3600;

	myUhrESP Uhr(zeitzone);
	GlobInterfaces.addInt(&Uhr);

	//##########################################################################################################################

	const uint8_t LEDPin = 1;
	DigitalBeleuchtungDisplay BEL(&MCP1, LEDPin);

	MyDisp DISP(&mqtt, &BEL, &ENC, &Uhr, RES_TFT, CS_TFT);
	GlobInterfaces.addInt(&DISP);

	DisplayAudioteil Audioteil(&DISP);
	GlobInterfaces.addInt(&Audioteil);
	//##########################################################################################################################

	bool onlyuseresetter = true;
	WTD GLOBWathdog(&mqtt, onlyuseresetter);
	GlobInterfaces.addInt(&GLOBWathdog);
	//###########################################################################################################################
	const char* Firmwarename = "Audiokistel.elf";
	OTAUpdater OTA(&mqtt, Firmwarename);
	OTA.setupdateW(&DISP);
	GlobInterfaces.addInt(&OTA);
	DISP.setOTAUpdater(&OTA);

	//###########################################################################################################################

//entweder auf dem Audioboarad ist irgendwo was vertauscht oder in der Bibliothek, aufjedenfall sind hier clock und data vertauscht, so geht es. sollte aber eigentlich nicht
	const uint8_t CLOCKPIN = 7;
	const uint8_t DATAPIN = 6;
	const uint8_t NUMMERLAUSTSTAERKE = 0;

	MyLSR Lautstaerke(&mqtt, &MCP1, DATAPIN, CLOCKPIN, NUMMERLAUSTSTAERKE);

	GlobInterfaces.addInt(&Lautstaerke);

	//###########################################################################################################################

  const uint8_t ADDRESSEDMX = 32;  // in der Bibliothek steht ne 2??? 

  myDMXDimmerESP myDMX;
  GlobInterfaces.addInt(&myDMX);
  //###########################################################################################################################


	HAUPTSCHLEIFE();

}
void loop() {}

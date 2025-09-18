

#define SPD_USE_CANUMSETZER     1
#define SPD_USE_INPUTS          0
#define SPD_USE_TAS_MTX         0
#define SPD_USE_HW_GPIO         0 
#define SPD_USE_HW_MCP          0 

#define SPD_USE_IR              1  		//getestet
#define SPD_USE_LEDMTRX         1 		//getestet
#define SPD_USE_DMXDIMMER		1       //getestet
#define SPD_USE_LEDINTERF       1 

 
#define SPD_USE_OTAUPDATER      0    
#define SPD_USE_WTD             1   	//getestet
#define SPD_USE_SPIFFSSETTINGS  1 		//getestet
 
 
#define SPD_USE_MQTTVARIANT     1  
            //0: Serial 				//getestet
            //1: W5500Ethernet   	    //getestet
            //2: Wifi               
            //3: CAN


#include "hilfe.h"
#include "basisinterface.h"
#include "settingsmqttsecret.h"





#if SPD_USE_LEDMTRX == 1
  #include <myws2812ledmatrix.h>

  #include "myUhrESP.h"  //Echtzeituhr ESP32
#endif

#if SPD_USE_IR == 1
  #include "ir.h" 
#endif
#if SPD_USE_INPUTS == 1
  #include "myInputs.h"
#endif
#if SPD_USE_HW_MCP == 1
  #include "myBasisHW_MCP.h" //Hardware
#endif
#if SPD_USE_HW_GPIO == 1
  #include "myBasisHW_GPIO.h" //Hardware
#endif
#if SPD_USE_MQTTVARIANT == 0
  #include "serialdummymqtt.h" 
#endif
#if SPD_USE_MQTTVARIANT == 1
   #include "w5100ethernetmqtt.h"
#endif
#if SPD_USE_MQTTVARIANT == 2
  #include "espwifimqtt.h"
#endif

#if SPD_USE_WTD == 1
  #include "arduinowatchdog.h"
#endif
#if SPD_USE_OTAUPDATER == 1
  #include "myotaupdater.h"
#endif
#if SPD_USE_DMXDIMMER == 1
  #include "myDMXDimmerESP.h" 
#endif
#if SPD_USE_SPIFFSSETTINGS == 1
  #include "mySpiffsSettingsloader.h"
#endif

#if SPD_USE_CANUMSETZER == 1
	#include "canumsetzer.h"
	#include "canbasis.h"
	#include "canhardESP.h"
#endif
#if SPD_USE_LEDINTERF == 1
  #include "ws2812streifen.h"
#endif


Interfacesammler GlobInterfaces(20);

void setup()
{

	globaleinitialisierung();




 
	char MYMQTTNAME[50];  
	strcpy(MYMQTTNAME, "/tester");
 

#if SPD_USE_SPIFFSSETTINGS == 1

  const char* filepfad = "/MQTTName";
  mySpiffsSettingsloaderString Adressloader(filepfad, MYMQTTNAME);
  strcpy(MYMQTTNAME, Adressloader.getSett());
	
#endif


 
#if SPD_USE_MQTTVARIANT == 0
  SerialDummy mqtt(MYMQTTNAME);
#endif
#if SPD_USE_MQTTVARIANT == 1
  byte MYMACADDR[6];
  GLOGetETHMacFromWifiMac(MYMACADDR);
  const uint8_t RESPINETHERNET = 26;
  const uint8_t CSETHERNET = 14;
  MqttCommunication mqtt(MYSERVERADDR, MYMACADDR, MYMQTTNAME, MYMQTTUSER, MYMQTTPASSW, RESPINETHERNET,CSETHERNET);
#endif
 #if SPD_USE_MQTTVARIANT == 2
  ESP_Wifi_MQTT mqtt(MYSERVERADDR, MYMQTTNAME, MYMQTTUSER, MYMQTTPASSW, WIFISSID, WIFIPW);
#endif
  GlobInterfaces.addInt(&mqtt);




//###########################################################################################################################
 



#if SPD_USE_CANUMSETZER == 1  

  	const uint32_t geschwindigkeit = 125000; // scheint nicht zu wirken
  	const uint8_t CANCS_RXpin = 17;
  	const uint8_t CANIRQ_TXpin = 5;
 
  	//achtung, Canbusgeschwindikeit wirkt derzeit nicht!!!!
	canhardESP canHW(CANCS_RXpin, CANIRQ_TXpin, geschwindigkeit);
  
	CanUmsetzer UMS(&mqtt, &canHW);
	GlobInterfaces.addInt(&UMS);
#endif
//###########################################################################################################################





#if SPD_USE_SPIFFSSETTINGS == 1  
	GlobInterfaces.addInt(&Adressloader);
#endif 
	//###########################################################################################################################
#if SPD_USE_HW_GPIO == 1
  myBasisHW_GPIO HARDW;  //HARDW()         //warum ohne Klammern???? CCC? Das verstehe ich nicht
  GlobInterfaces.addInt(&HARDW);
#endif
	//###########################################################################################################################
#if SPD_USE_HW_MCP == 1  
  const uint8_t ADDRMCP1 = 0;
  myBasisHW_MCP MCP1(ADDRMCP1);
  GlobInterfaces.addInt(&MCP1);
#endif
	////###########################################################################################################################
#if SPD_USE_TAS_MTX == 1
	const uint8_t MTXzeil = 3;
	const uint8_t MTXspal = 3;
	uint8_t       MTXZeilP[] = { 2,3,4 };
	uint8_t       MTXSpalP[] = { 5,6,7 };
	uint8_t MTXkeys[MTXzeil * MTXspal] = { 6,3,0,7,4,1,8,5,2 };

	myTastermatrix MTX(&mqtt, &MCP1, MTXspal, MTXzeil, MTXSpalP, MTXZeilP, MTXkeys);
	GlobInterfaces.addInt(&MTX);
#endif
	//###########################################################################################################################
#if SPD_USE_INPUTS == 1
	uint8_t       MCUSETTINGS1eingangssadressenlen = 2;
	uint8_t       MCUSETTINGS1eingangsadressenpin[] = { 8,11 };  // bewmelder, taste auf incgeber
	uint8_t       MCUSETTINGS1eingangsadressenlog[] = { 10,9 };
	bool       MCUSETTINGS1eingangactivatelong[] = { false,true };

	bool       MCUSETTINGS1turnlogic[] = { false,true };

	myInputs IN(&mqtt, &MCP1, MCUSETTINGS1eingangssadressenlen, MCUSETTINGS1eingangsadressenpin, MCUSETTINGS1eingangsadressenlog, MCUSETTINGS1eingangactivatelong, MCUSETTINGS1turnlogic);
	GlobInterfaces.addInt(&IN);
#endif
	//###########################################################################################################################
#if SPD_USE_ENCODER == 1
	const uint8_t ENCODERNUMMER = 0;
	uint8_t ENCODERPINA = 27;
	uint8_t ENCODERPINB = 26;


	myEncoderESP ENC(&mqtt, ENCODERPINA, ENCODERPINB, ENCODERNUMMER);

	GlobInterfaces.addInt(&ENC);
#endif
	//###########################################################################################################################
#if SPD_USE_IR == 1
	const uint8_t IRREC_PIN = 16;
	const bool IR_ENABLE_SEND = false; //PIN is fix

	MyIr IRGeraet(&mqtt, IRREC_PIN, IR_ENABLE_SEND);
	GlobInterfaces.addInt(&IRGeraet);
#endif
	//###########################################################################################################################
#if SPD_USE_HELL_SENS == 1
	MyHell HELL(&mqtt);
	GlobInterfaces.addInt(&HELL);
#endif
	//###########################################################################################################################
#if SPD_USE_LEDMTRX == 1
	uint32_t zeitzone = 3600;

	myUhrESP Uhr(zeitzone);
	GlobInterfaces.addInt(&Uhr);

	const uint8_t LEDWS28PIN1 = 12;
 

	myLEDMatrix LEDMTX( LEDWS28PIN1, &Uhr);
	GlobInterfaces.addInt(&LEDMTX);
#endif
	//###########################################################################################################################
#if SPD_USE_BEEPER == 1
	MyBeep BEEP(&mqtt);
	GlobInterfaces.addInt(&BEEP);
#endif

	//##########################################################################################################################
#if SPD_USE_WTD == 1
	bool onlyuseresetter = false;
    uint32_t Resetzeit = 300000; 
  
	WTD GLOBWathdog(&mqtt, onlyuseresetter,Resetzeit);
	GlobInterfaces.addInt(&GLOBWathdog);
#endif
	//###########################################################################################################################
#if SPD_USE_OTAUPDATER == 1  
  const char* Firmwarename = "???.bin";
	OTAUpdater OTA(&mqtt,Firmwarename);
#if SPD_USE_DISP_UND_HEIZ == 1 
	OTA.setupdateW(&DISP);
#endif  
	GlobInterfaces.addInt(&OTA);

#if SPD_USE_DISP_UND_HEIZ == 1 
	DISP.setOTAUpdater(&OTA);
#endif  
#endif
	//###########################################################################################################################
#if SPD_USE_DMXDIMMER == 1
  myDMXDimmerESP myDMX;
  GlobInterfaces.addInt(&myDMX);
#endif 

	//###########################################################################################################################
#if SPD_USE_LEDINTERF == 1
	const uint8_t BANDNO1 = 0;
	const uint8_t ANZAHLLEDs1 = 50;
	const uint8_t MAXBRIGHTNESS1 = 50;
	const uint8_t LEDWS28PINBand = 33;
	const bool SpeichereWertefuerHelligkeitsaenderung = true;

	Ws2812streifen LEDs1(BANDNO1, ANZAHLLEDs1, LEDWS28PINBand, MAXBRIGHTNESS1, SpeichereWertefuerHelligkeitsaenderung);
	GlobInterfaces.addInt(&LEDs1);
#endif
	//###########################################################################################################################



	HAUPTSCHLEIFE();

}
void loop() {}

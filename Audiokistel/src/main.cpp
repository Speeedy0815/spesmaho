
#define SPD_USE_INPUTS          1 //getestet
#define SPD_USE_HW_GPIO         1 //gibts das hier ueberhaupt?
#define SPD_USE_HW_MCP          1 //getestet
#define SPD_USE_IR              1 //getestet
#define SPD_USE_ENCODER         1 
#define SPD_USE_LEDINTERF       1 
   
 
#define SPD_USE_OTAUPDATER      1  //getestet
#define SPD_USE_WTD             1  //getestet
#define SPD_USE_SPIFFSSETTINGS  1 
#define SPD_USE_DMXDIMMER       1  //getestet
#define SPD_USE_MQTTVARIANT     2  
            //0: Serial 
            //1: W5500Ethernet     //getestet
            //2: Wifi
            //3: CAN




 




#include "hilfe.h"
#include "basisinterface.h"
#include "settingsmqttsecret.h"


  
#if SPD_USE_DISPLAY_BEL == 1
  #include "mydisplaybeleuchter.h"
#endif
#if SPD_USE_HW_MCP == 1
  #include "myBasisHW_MCP.h" //Hardware
#endif
#if SPD_USE_HW_GPIO == 1
  #include "myBasisHW_GPIO.h" //Hardware
#endif
#if SPD_USE_INPUTS == 1
  #include "myInputs.h"
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
#if SPD_USE_MQTTVARIANT == 3
  //todo canheader heraussuchen
#endif
#if SPD_USE_DMXDIMMER == 1
  #include "myDMXDimmerESP.h" 
#endif
#if SPD_USE_WTD == 1
  #include "arduinowatchdog.h"
#endif
#if SPD_USE_SPIFFSSETTINGS == 1
  #include "mySpiffsSettingsloader.h"
#endif
#if SPD_USE_OTAUPDATER == 1
  #include "myotaupdater.h"
#endif
#if SPD_USE_LEDINTERF == 1
  #include "ledinterfacews2812.h"
#endif
#if SPD_USE_IR == 1
  #include "ir.h" 
#endif
#if SPD_USE_ENCODER == 1
  #include "myEncoderESP.h"
#endif

 

Interfacesammler GlobInterfaces(20);

void setup()
{

	globaleinitialisierung();


  const uint8_t CS_SD = 27;
  const uint8_t CS_TOUCH = 33;
  const uint8_t CS_TFT = 4;
 

 
	const uint8_t CSETHERNET = 14;
	const uint8_t RESPINETHERNET = 26;
	
	

//Initialisierung aller wichtigen Pins auf dem Bus  --> nicht wegschaltern!!!
	GLOinitCS(CS_TFT);	//CS TFT
	GLOinitCS(CS_TOUCH);	//CS Touch
  GLOinitCS(CS_SD);  //CS SD
	GLOinitCS(CSETHERNET);	//CS Ethernet
  // Konfiguration des CS-Pins
  pinMode(RESPINETHERNET, OUTPUT);
  digitalWrite(RESPINETHERNET, HIGH); // CS auf HIGH (deaktiviert)
  delay(1000);


  
const char* stdnamewennnichhstgesetztist = "/audiokaestel";
#if SPD_USE_SPIFFSSETTINGS == 1  
  const char* filepfad = "/MQTTName";
  
	mySpiffsSettingsloaderString Adressloader(filepfad, stdnamewennnichhstgesetztist);
#endif

//###########################################################################################################################
#if SPD_USE_MQTTVARIANT == 0
	SerialDummy mqtt(Adressloader.getSett());
#endif
#if SPD_USE_MQTTVARIANT == 1
  uint8_t MYMACADDR[6];
  GLOGetETHMacFromWifiMac(MYMACADDR);
	MqttCommunication mqtt(MYSERVERADDR, MYMACADDR,stdnamewennnichhstgesetztist, MYMQTTUSER, MYMQTTPASSW, RESPINETHERNET, CSETHERNET);
#endif
#if SPD_USE_MQTTVARIANT == 2
  ESP_Wifi_MQTT mqtt(MYSERVERADDR, stdnamewennnichhstgesetztist, MYMQTTUSER, MYMQTTPASSW, WIFISSID, WIFIPW);
#endif
#if SPD_USE_MQTTVARIANT == 3
  CANMQTT mqtt(CANaddrloader.getAddr(), &canHW); //nummer, rx und tx pin
#endif
	GlobInterfaces.addInt(&mqtt);

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
	//###########################################################################################################################
#if SPD_USE_INPUTS == 1
	uint8_t       MCUSETTINGS1eingangssadressenlen = 6;
	uint8_t       MCUSETTINGS1eingangsadressenpin[] = { 2,3,4,13,12,5 };  //5 Taster + Taster auf INC Geber
	uint8_t       MCUSETTINGS1eingangsadressenlog[] = { 0,1,2,3,4,5 };
	bool       MCUSETTINGS1eingangactivatelong[] = { true,true,true,true ,true ,true };

	bool       MCUSETTINGS1turnlogic[] = { true,true,true,true ,true ,true };

	myInputs IN(&mqtt, &MCP1, MCUSETTINGS1eingangssadressenlen, MCUSETTINGS1eingangsadressenpin, MCUSETTINGS1eingangsadressenlog, MCUSETTINGS1eingangactivatelong, MCUSETTINGS1turnlogic);
	GlobInterfaces.addInt(&IN);
#endif
	//###########################################################################################################################
#if SPD_USE_ENCODER == 1
	const uint8_t ENCODERNUMMER = 0;
	uint8_t ENCODERPINA = 36;
	uint8_t ENCODERPINB = 39;


	myEncoderESP ENC(&mqtt, ENCODERPINA, ENCODERPINB, ENCODERNUMMER);

	GlobInterfaces.addInt(&ENC);
#endif
	//###########################################################################################################################
#if SPD_USE_IR == 1
	const uint8_t IRREC_PIN = 34;
	const bool IR_ENABLE_SEND = false; //PIN is fix

	MyIr IRGeraet(&mqtt, IRREC_PIN, IR_ENABLE_SEND);
	GlobInterfaces.addInt(&IRGeraet);
#endif
	//###########################################################################################################################
#if SPD_USE_LEDINTERF == 1
 
	const uint8_t BANDNO1 = 0;
	const uint8_t ANZAHLLEDs1 = 30;
	const uint8_t MAXBRIGHTNESS1 = 50;
	const uint8_t LEDWS28PIN1 = 2;
	const bool SpeichereWertefuerHelligkeitsaenderung = true;

	LEDW2812Interface LEDs1(BANDNO1, ANZAHLLEDs1, LEDWS28PIN1, MAXBRIGHTNESS1, SpeichereWertefuerHelligkeitsaenderung);
	GlobInterfaces.addInt(&LEDs1);
#endif
	//###########################################################################################################################	
	
#if SPD_USE_DISP_UND_SND
	uint32_t zeitzone = 3600;

	myUhrESP Uhr(zeitzone);
	GlobInterfaces.addInt(&Uhr);
#endif
 
	//##########################################################################################################################
  

#if SPD_USE_WTD == 1
	bool onlyuseresetter = false;
	WTD GLOBWathdog(&mqtt, onlyuseresetter);
	GlobInterfaces.addInt(&GLOBWathdog);
#endif
  
	//###########################################################################################################################
#if SPD_USE_OTAUPDATER == 1
	const char* Firmwarename = "Audiokistel.elf";
	OTAUpdater OTA(&mqtt, Firmwarename); 
	GlobInterfaces.addInt(&OTA);
#endif
	//###########################################################################################################################
 
#if SPD_USE_DMXDIMMER == 1
  myDMXDimmerESP myDMX;
  GlobInterfaces.addInt(&myDMX);
#endif 
  //###########################################################################################################################


	HAUPTSCHLEIFE();

}
void loop() {}

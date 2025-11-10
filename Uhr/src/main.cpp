//Todo Watchdog sinnvoll, ohne Ethernetverbindung tut der nichts!

#define SPD_USE_CANUMSETZER     1


#define SPD_USE_HW_MCP          1 
#define SPD_USE_INPUTS          1
#define SPD_USE_OUTPUTS         1

#define SPD_USE_IR              1  		//getestet
#define SPD_USE_LEDMTRX         1 		//getestet
#define SPD_USE_DMXDIMMER		    1     //getestet
#define SPD_USE_LEDINTERF       1 
#define SPD_USE_ONEWIRE         1
 
#define SPD_USE_OTAUPDATER      1    
#define SPD_USE_WTD             1   	//getestet
#define SPD_USE_SPIFFSSETTINGS  1 		//getestet


 
 
#define SPD_USE_MQTTVARIANT     2  
            //0: Serial 				      //getestet
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

#if SPD_USE_OUTPUTS == 1
  #include "myOutputs.h"
#endif


#if SPD_USE_HW_MCP == 1
  #include "myBasisHW_MCP.h" //Hardware
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
  #include "myotaupdater.h"//über Wifi
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
#if SPD_USE_ONEWIRE == 1 
  #include "myonewire.h"
#endif
Interfacesammler GlobInterfaces(25);

void setup()
{

	globaleinitialisierung();

#if SPD_USE_HW_MCP == 1
const uint8_t resetmcppin = 4;//Achtung Hardwarefehler in der ersten Variante
	myBasisHW_MCP_Resetit(resetmcppin);
#endif




 
	char MYMQTTNAME[50];  
	strcpy(MYMQTTNAME, "/tester");
 

#if SPD_USE_SPIFFSSETTINGS == 1

  const char* filepfad = "/MQTTName";
  mySpiffsSettingsloaderString Adressloader(filepfad, MYMQTTNAME);
  strcpy(MYMQTTNAME, Adressloader.getSett());
	
#endif


 
#if SPD_USE_MQTTVARIANT == 0
  Serial.println("Starte Serial Interface");
  SerialDummy mqtt(MYMQTTNAME);
#endif
#if SPD_USE_MQTTVARIANT == 1
  Serial.println("Starte Ethernet Interface");
  byte MYMACADDR[6];
  GLOGetETHMacFromWifiMac(MYMACADDR);
  const uint8_t RESPINETHERNET = 26;
  const uint8_t CSETHERNET = 14;
  MqttCommunication mqtt(MYSERVERADDR, MYMACADDR, MYMQTTNAME, MYMQTTUSER, MYMQTTPASSW, RESPINETHERNET,CSETHERNET);
#endif
 #if SPD_USE_MQTTVARIANT == 2
  Serial.println("Starte Wifi Interface");
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
#if SPD_USE_HW_MCP == 1  
myBasisHW_MCP MCPs[8] = { myBasisHW_MCP(0), myBasisHW_MCP(1), myBasisHW_MCP(2), myBasisHW_MCP(3) , myBasisHW_MCP(4), myBasisHW_MCP(5) ,myBasisHW_MCP(6) ,myBasisHW_MCP(7) };

for (uint8_t i = 0; i < 8; i++) {
    if (MCPs[i].istesauchda()) {
        GlobInterfaces.addInt(&MCPs[i]);
    }
}
#endif
	//###########################################################################################################################

#if SPD_USE_INPUTS == 1
#if !defined(SPD_USE_HW_MCP) || (SPD_USE_HW_MCP != 1)
  #error "MCP muss an sein, sonst geht das nicht! (Oder Eingänge ausschalten)"
#endif

// ------------------- Konstanten -------------------
uint8_t MCUSETTINGS1eingangssadressenlen = 16;
uint8_t MCUSETTINGS1eingangsadressenpin[16] = 
  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
bool MCUSETTINGS1eingangactivatelong[16] = 
  { false, false, false, false, false, false, false, false,
    false, false, false, false, false, false, false, false };
bool MCUSETTINGS1turnlogic[16] =
  { true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true };

// ------------------- Logik-Adressen -------------------
uint8_t logs0[16], logs1[16], logs2[16], logs3[16];
for (int i = 0; i < 16; i++) {
  logs0[i] = i;
  logs1[i] = i + 16;
  logs2[i] = i + 32;
  logs3[i] = i + 48;
}
uint8_t* logs[4] = { logs0, logs1, logs2, logs3 };

// ------------------- Eingangs-Objekte (dynamisch) -------------------
myInputs* INs[4] = { nullptr, nullptr, nullptr, nullptr };

for (uint8_t i = 0; i < 4; i++) {
  if (MCPs[i].istesauchda()) {
    Serial.print("Eingänge Block eingehangen, Nr: ");
    Serial.println(i);

    char payload[8];                      // groß genug für "A255\0"
    int n = snprintf(payload, sizeof(payload), "E%u", (unsigned)i);
    if (n > 0 && n < (int)sizeof(payload)) {
      mqtt.sendmessage("INF", payload);   // erwartet const char*
    }

    INs[i] = new myInputs(&mqtt, &MCPs[i],
                          MCUSETTINGS1eingangssadressenlen,
                          MCUSETTINGS1eingangsadressenpin,
                          logs[i],
                          MCUSETTINGS1eingangactivatelong,
                          MCUSETTINGS1turnlogic);

    GlobInterfaces.addInt(INs[i]);
  }
}

#endif

  //###########################################################################################################################
#if SPD_USE_OUTPUTS == 1
#if !defined(SPD_USE_HW_MCP) || (SPD_USE_HW_MCP != 1)
  #error "MCP muss an sein, sonst geht das nicht! (Oder Ausgänge ausschalten)"
#endif
const uint8_t LAENGEAUSGAENGE = 16;
uint8_t ADRESSENHWAUSGAENGE[LAENGEAUSGAENGE] = 
  { 0, 1, 2, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8 };
bool AUSGAENGEDREHEN[LAENGEAUSGAENGE] =
  { true, true, true, true, true, true, true, true,
    true, true, true, true, true, true, true, true };

// Logikadressen für 4 MCP-Blöcke
uint8_t logsO0[16], logsO1[16], logsO2[16], logsO3[16];
for (int i = 0; i < 16; i++) {
  logsO0[i] = i;
  logsO1[i] = i + 16;
  logsO2[i] = i + 32;
  logsO3[i] = i + 48;
}
uint8_t* logsOut[4] = { logsO0, logsO1, logsO2, logsO3 };

// ------------------- Ausgangs-Objekte (dynamisch) -------------------
myOutputs* OUTs[4] = { nullptr, nullptr, nullptr, nullptr };

for (uint8_t i = 0; i < 4; i++) {
  if (MCPs[i+4].istesauchda()) {
    Serial.print("Ausgänge Block eingehangen, Nr: ");
    Serial.println(i);

    char payload[8];                      // groß genug für "A255\0"
    int n = snprintf(payload, sizeof(payload), "A%u", (unsigned)i);
    if (n > 0 && n < (int)sizeof(payload)) {
      mqtt.sendmessage("INF", payload);   // erwartet const char*
    }


    OUTs[i] = new myOutputs(&mqtt, &MCPs[i+4],
                            LAENGEAUSGAENGE,
                            ADRESSENHWAUSGAENGE,
                            logsOut[i],
                            AUSGAENGEDREHEN);

    GlobInterfaces.addInt(OUTs[i]);
  }
}

#endif





	//###########################################################################################################################
#if SPD_USE_IR == 1
	const uint8_t IRREC_PIN = 16;
	const bool IR_ENABLE_SEND = false; //PIN is fix

	MyIr IRGeraet(&mqtt, IRREC_PIN, IR_ENABLE_SEND);
	GlobInterfaces.addInt(&IRGeraet);
#endif
	//###########################################################################################################################
#if SPD_USE_LEDMTRX == 1
	uint32_t zeitzone = 3600;

	myUhrESP Uhr(zeitzone);
	GlobInterfaces.addInt(&Uhr);

  constexpr uint8_t LEDWS28PIN1 = 12;

  // Template-Instanz:
  myLEDMatrix<LEDWS28PIN1> LEDMTX(&Uhr);
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
  const char* Firmwarename = "GruenePlatine.bin";
	OTAUpdater OTA(&mqtt,Firmwarename);
	GlobInterfaces.addInt(&OTA);
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
	constexpr uint8_t LEDWS28PINBand = 33;
	const bool SpeichereWertefuerHelligkeitsaenderung = true;

	Ws2812streifen<LEDWS28PINBand> LEDs1(BANDNO1, ANZAHLLEDs1, MAXBRIGHTNESS1, SpeichereWertefuerHelligkeitsaenderung);
	GlobInterfaces.addInt(&LEDs1);
#endif
	//###########################################################################################################################

 #if SPD_USE_ONEWIRE == 1 
	//###########################################################################################################################
	const uint8_t ADDRESSE1WIREPIN = 27;

	XOnewire myonewire(ADDRESSE1WIREPIN, &mqtt);
	GlobInterfaces.addInt(&myonewire);
	//###########################################################################################################################
#endif

	HAUPTSCHLEIFE();

}
void loop() {}

#define MYDEBUG 0
#include "mydebug.h"


#include "w5100ethernetmqtt.h"
#include "settingsmqttsecret.h"
#include "serialdummymqtt.h"
#include "sperrausgang.h"
#include "arduinowatchdog.h"
#include "myonewire.h"
#include "ledinterfacews2812.h"
#include "myBasisHW_MCP.h"  //Hardware an einem MCP
#include "mytastermatrix.h"


#include "myBasisHW_GPIO.h"  //Hardware
#include "myOutputs.h"
#include "myInputs.h"

#include "myDMXDimmer.h"


#include "myhardwareserialrx.h"

#include "hilfe.h"
#include "basisinterface.h"


Interfacesammler GlobInterfaces(21);


void setup() {
  globaleinitialisierung();
  //###########################################################################################################################
  ArduinoHWWTD HWWTD;  //warum ohne Klammern???? CCC? Das verstehe ich nicht, achtung an der Stelle habe ich das nicht getestet, könnte auch mit Klammern gehen
  GlobInterfaces.addInt(&HWWTD);
  //###########################################################################################################################
  byte MYMACADDR[6] = { 0x74, 0x82, 0x33, 0x34, 0x10, 0x79 };
  const char* MYMQTTNAME = "/Zentrale";
  const uint8_t RESPINETHERNET = 9;

  MqttCommunication mqtt(MYSERVERADDR, MYMACADDR, MYMQTTNAME, MYMQTTUSER, MYMQTTPASSW, RESPINETHERNET);
  //SerialDummy mqtt(MYMQTTNAME);
  GlobInterfaces.addInt(&mqtt);
  //###########################################################################################################################

  const uint8_t ADDRMCP0 = 0;
  myBasisHW_MCP MCP0(ADDRMCP0);
  GlobInterfaces.addInt(&MCP0);
  //###########################################################################################################################
  const uint8_t ADDRMCP1 = 1;

  myBasisHW_MCP MCP1(ADDRMCP1);
  GlobInterfaces.addInt(&MCP1);
  //###########################################################################################################################
  const uint8_t ADDRMCP2 = 2;

  myBasisHW_MCP MCP2(ADDRMCP2);
  GlobInterfaces.addInt(&MCP2);
  //###########################################################################################################################
  const uint8_t ADDRMCP3 = 3;

  myBasisHW_MCP MCP3(ADDRMCP3);
  GlobInterfaces.addInt(&MCP3);
  //###########################################################################################################################
  const uint8_t ADDRMCP4 = 4;

  myBasisHW_MCP MCP4(ADDRMCP4);
  GlobInterfaces.addInt(&MCP4);

  //###########################################################################################################################
  myBasisHW_MCP_Resetit(A12);  //Resetpin ist an A12 Angeschlossen
  //###########################################################################################################################
  const int LAENGEAUSGAENGE = 16;
  uint8_t ADRESSENHWAUSGAENGE[] = { 0, 1, 2, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8 };
  uint8_t ADRESSENLOGISCHAUSGAENGE0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  bool AUSGAENGEDREHEN[] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };

  myOutputs OUT0(&mqtt, &MCP0, LAENGEAUSGAENGE, ADRESSENHWAUSGAENGE, ADRESSENLOGISCHAUSGAENGE0, AUSGAENGEDREHEN);
  GlobInterfaces.addInt(&OUT0);
  //###########################################################################################################################
  //const int   LAENGEAUSGAENGE1 = 16;
  //uint8_t     ADRESSENHWAUSGAENGE1[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
  uint8_t ADRESSENLOGISCHAUSGAENGE1[] = { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
  //bool        AUSGAENGEDREHEN[] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false };

  myOutputs OUT1(&mqtt, &MCP1, LAENGEAUSGAENGE, ADRESSENHWAUSGAENGE, ADRESSENLOGISCHAUSGAENGE1, AUSGAENGEDREHEN);
  GlobInterfaces.addInt(&OUT1);
  //###########################################################################################################################
  //const int   LAENGEAUSGAENGE2 = 16;
  //uint8_t     ADRESSENHWAUSGAENGE[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
  uint8_t ADRESSENLOGISCHAUSGAENGE2[] = { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };
  //bool        AUSGAENGEDREHEN[] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false };

  myOutputs OUT2(&mqtt, &MCP2, LAENGEAUSGAENGE, ADRESSENHWAUSGAENGE, ADRESSENLOGISCHAUSGAENGE2, AUSGAENGEDREHEN);
  GlobInterfaces.addInt(&OUT2);
  //###########################################################################################################################
  //const int   LAENGEAUSGAENGE3 = 16;
  //uint8_t     ADRESSENHWAUSGAENGE[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
  uint8_t ADRESSENLOGISCHAUSGAENGE3[] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 };
  //bool        AUSGAENGEDREHEN[] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false };

  myOutputs OUT3(&mqtt, &MCP3, LAENGEAUSGAENGE, ADRESSENHWAUSGAENGE, ADRESSENLOGISCHAUSGAENGE3, AUSGAENGEDREHEN);
  GlobInterfaces.addInt(&OUT3);
  //###########################################################################################################################

  WTD GLOBWathdog(&mqtt);
  GlobInterfaces.addInt(&GLOBWathdog);
  //###########################################################################################################################

  MyHsRx Hardwareserial(&mqtt);
  GlobInterfaces.addInt(&Hardwareserial);
  //###########################################################################################################################
  myBasisHW_GPIO HARDW;  //HARDW()         //warum ohne Klammern???? CCC? Das verstehe ich nicht
  GlobInterfaces.addInt(&HARDW);
  //###########################################################################################################################
  const uint8_t MTXzeil1 = 3;
  const uint8_t MTXspal1 = 3;
  uint8_t MTXZeilP1[] = { 35, 37, 39 };
  uint8_t MTXSpalP1[] = { 41, 43, 45 };
  uint8_t MTXkeys1[MTXzeil1 * MTXspal1] = { 21, 19, 20, 18, 16, 17, 24, 22, 23 };

  myTastermatrix MTX1(&mqtt, &HARDW, MTXspal1, MTXzeil1, MTXSpalP1, MTXZeilP1, MTXkeys1);
  GlobInterfaces.addInt(&MTX1);
  //###########################################################################################################################
  const uint8_t BANDNO1 = 0;
  const uint8_t ANZAHLLEDs1 = 9;
  const uint8_t MAXBRIGHTNESS1 = 60;
  const uint8_t LEDWS28PIN1 = 33;

  LEDW2812Interface LEDs1(BANDNO1, ANZAHLLEDs1, LEDWS28PIN1, MAXBRIGHTNESS1, true);
  GlobInterfaces.addInt(&LEDs1);

  //###########################################################################################################################
  uint8_t MCUSETTINGS1eingangssadressenlen = 16;
  uint8_t MCUSETTINGS1eingangsadressenpin[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  uint8_t MCUSETTINGS1eingangsadressenlog[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
  bool MCUSETTINGS1eingangactivatelong[] = { false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false };
  bool MCUSETTINGS1turnlogic[] = { true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true };

  myInputs IN(&mqtt, &MCP4, MCUSETTINGS1eingangssadressenlen, MCUSETTINGS1eingangsadressenpin, MCUSETTINGS1eingangsadressenlog, MCUSETTINGS1eingangactivatelong, MCUSETTINGS1turnlogic);
  GlobInterfaces.addInt(&IN);


  //###########################################################################################################################
  const uint8_t ADDRESSE1WIREPIN = 49;
  bool nur_Temperaturen = true;


  XOnewire myonewire(ADDRESSE1WIREPIN, &mqtt, nur_Temperaturen);
  GlobInterfaces.addInt(&myonewire);
  //###########################################################################################################################

  const uint8_t ADDRESSEDMX = 4;

  myDMXDimmer myDMX(ADDRESSEDMX);
  GlobInterfaces.addInt(&myDMX);




  HAUPTSCHLEIFE();
}
void loop() {}

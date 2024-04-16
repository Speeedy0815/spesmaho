#pragma once


#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#ifdef ARDUINO_ARCH_ESP32

#include "canbasis.h"
 
#include <ACAN_ESP32.h>
#include <ACAN_ESP32_CANRegisters.h>

 


#include "basisinterface.h" 
#include "arduinowatchdog.h"
#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true



class canhardESP : public canbasis
{
private:


public:
	bool hasgetmsg(mycan_frame* newframe);
	 


	canhardESP(uint8_t RXpin, uint8_t TXpin, const uint32_t geschwindigkeit);
	int8_t send(const long id, const uint8_t* buffer8, uint8_t size);
};



class canSettingloader : public BasisInterface 
{
private:
	void loadaddr();
	void saveaddr(uint8_t addr);
	const uint8_t startaddr = 30;
	uint8_t addr = startaddr; //30 ist dann quasi Start bzw ungültig, wie man es sieht
public:
	uint8_t getAddr();
	canSettingloader();
	void update() {}; // nichts zu tun, nur um Basisklasse zu beruhigen
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

#else

#endif




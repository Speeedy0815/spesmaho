#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif





#include <dmx.h>
#include "basisinterface.h"
#include "hilfewaveforms.h"

static const uint8_t maxChan = 64;
static const uint8_t maxGeschwindigkeit = 16;




class myDMXDimmerESP : public BasisInterface
{
private:
	//DMXESPSerial dmx;
	uint8_t modus[maxChan] = { 0 };
	uint8_t geschwindigkeitsmodus[maxChan] = { 0 };
	uint8_t offset[maxChan] = { 0 };
	uint8_t geschwindigkeitsvalues[maxGeschwindigkeit] = { 0 };
	
	uint8_t rampValue[maxChan]= { 0 };  // aktueller Ramp-Wert 0..255
    bool    rampDone[maxChan]= { false };   // true, wenn Kanal schon auf Max
  	uint8_t lastMiliValue[maxChan]= { 0 };   // letzter getmiliValue() für Delta-Berechnung


	void setmode(uint8_t mod, uint8_t chan, uint8_t mygeschwindigkeit, uint8_t myoffset, uint8_t value);


	void rechnemilivalue();
	uint8_t getmiliValue(uint8_t chanr);
	



public:
	myDMXDimmerESP();
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

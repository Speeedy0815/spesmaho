#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
 
 
#include "Adafruit_MCP23X17.h"
#include "myBasisHW.h"

void myBasisHW_MCP_Resetit(uint8_t pin);  // Use it if you have ResetPin

class myBasisHW_MCP : public myBasisHW
{
private:
	Adafruit_MCP23X17 mymcp;
	uint16_t abbild;
protected:

public:
	~myBasisHW_MCP() {};
	myBasisHW_MCP(uint8_t mcpaddr);
	void update();
	bool getBit(uint8_t nr);
	void setBit(uint8_t nr, uint8_t an_aus);

	void set_as_Output(uint8_t nr);
	void set_as_Input(uint8_t nr);
	void set_as_Input_Pullup(uint8_t nr);

	void activatePinForInterrupt(uint8_t nr, uint8_t mode);
 
};


#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
 
#include "basisinterface.h"

class myBasisHW : public BasisInterface
{
private: 

protected:

public:
	~myBasisHW(){};
	myBasisHW(){};
	virtual void update() = 0; 
	virtual bool getBit(uint8_t nr) = 0;
	virtual void setBit(uint8_t nr,uint8_t an_aus) = 0;

	virtual void set_as_Output(uint8_t nr) = 0;
	virtual void set_as_Input(uint8_t nr) = 0;
	virtual void set_as_Input_Pullup(uint8_t nr) = 0;

	virtual void activatePinForInterrupt(uint8_t nr, uint8_t mode) = 0;

	bool callbackismineanddo(char* topic, byte* payload, unsigned int length) { return (false); };	  //da Basisinterface abgeleitet
};


#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
 

class BasisInterface
{
private: 

protected:

public:
	~BasisInterface(){};
	BasisInterface(){};
	virtual void update() = 0;
	virtual bool callbackismineanddo(char* topic, byte* payload, unsigned int length) = 0;
 
};


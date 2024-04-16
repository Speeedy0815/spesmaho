
#define MYDEBUG 0
#include "mydebug.h"

#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
 

class myUhrBasis 
{
 
public: 
	virtual int getSecond() = 0;
	virtual int getDay() = 0;
	virtual void setUhrzeit(unsigned long timestamp) = 0;
	virtual uint32_t getUhrzeit() = 0;
	virtual bool istgestellt() = 0;
	virtual String getTime(String format) = 0;
};

#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



class mycan_frame {
public:
	uint8_t data[8] = { 0 };
	long id = 0;
	bool ready = false;
};
 
 
class canbasis
{
private:
 

public:
	virtual bool hasgetmsg(mycan_frame* newframe) = 0;
	virtual int8_t send(const long id, const uint8_t* buffer8, uint8_t size) = 0;
	 
};

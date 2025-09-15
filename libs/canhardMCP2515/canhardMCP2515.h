#pragma once

#define MYDEBUG 0
#include "mydebug.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "canbasis.h"
#include <Adafruit_MCP2515.h>
 
 

class CAN_RINGBUFF
{
private:
	static const uint8_t bufflen = 30;
	static mycan_frame internalbuffer[bufflen];
	static uint8_t buffctrwrite;
	static uint8_t buffctrread;
	static Adafruit_MCP2515 * _mcp;
public:
	CAN_RINGBUFF(Adafruit_MCP2515 * mcp);
	static void onReceive(int packetSize);
	bool hasmsg();
	mycan_frame* getactmsg();  // achtung kracht, wenn vorher nicht mit hasmsg abgeprüft!!!!
};




class canhardMCP2515 : public canbasis
{
private:
	CAN_RINGBUFF mybuf;
	Adafruit_MCP2515 mcp;
public:
 
	bool hasgetmsg(mycan_frame* newframe);
	 
	
	
	canhardMCP2515(uint8_t CS_RXpin, uint8_t IRQ_TXpin, long geschwindigkeit);
	int8_t send(const long id, const uint8_t * buffer8, uint8_t size);
};



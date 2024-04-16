//Debugzeugs
#define MYDEBUG 0
#include "mydebug.h"

#pragma once


#include "basisinterface.h"

class Interfacesammler
{
private:
	uint8_t _max;
	BasisInterface** interfaceliste;
	uint8_t count = 0;
public:
	Interfacesammler(uint8_t maxanzahl);
	void addInt(BasisInterface * Intf);
	void run();
	void runcallback(char* topic, byte* payload, unsigned int length);
 
};
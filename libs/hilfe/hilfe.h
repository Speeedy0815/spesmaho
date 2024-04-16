//Debugzeugs
#define MYDEBUG 0
#include "mydebug.h"


#pragma once
#ifndef HILFEHEADER
#define HILFEHEADER



#include "basisinterface.h"
#include "basiskommunikation.h"
//#include "arduinowatchdog.h"


#if defined(ESP32)
#include <WiFi.h>
#endif


#include "globalversion.h"
#include "interfacesammler.h"






const uint8_t MYPINUNUSED = 0xff;
const uint8_t GLOBmaxlenpayload = 35;
const uint8_t GLOBmaxlentopic = 20;

 


void  GLOBmysendmessageWithAddr(Basiskommunikation* mqtt, const char* inhalt, const  char* topic, uint8_t addr);

uint8_t plausibel(uint8_t zahl, uint8_t min, uint8_t max); 
void GLOBcallback(char* topic, byte* payload, unsigned int length);
 

extern Interfacesammler GlobInterfaces;



void  HAUPTSCHLEIFE(); 
void globaleinitialisierung();
uint32_t zahlaustextvonlen(const char* eingabe, uint8_t stellebeginn, uint8_t len);

void GLOinitCS(uint8_t pin);

class myBremser : public BasisInterface
{
private:
	uint32_t bremszeit;
public:
	myBremser(uint32_t bremszeit) { this->bremszeit = bremszeit; };
	void update() { delay(bremszeit); };
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length) { return false; };

};






#endif // !HILFEHEADER

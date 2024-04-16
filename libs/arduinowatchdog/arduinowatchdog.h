// CCC Todo: derzeit für (ESP8266) || defined(ESP32) noch keinen Hardwarewatchdog  drin,.---- > muss erstmal ausgiebig mit dem Watchdog auseinandersetzen
#define MYDEBUG 0
#include "mydebug.h"


#ifndef ARDUINOWTDHEADER
#define ARDUINOWTDHEADER

#include "hilfe.h"
#include "mytimer.h"
#include "basisinterface.h"
#include "basiskommunikation.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#if defined(__AVR__) 
#include <avr/wdt.h>
static void(*resetFunc) (void) = 0;
#endif // defined(__AVR__) 


void globalResetme();


 

class WTD : public BasisInterface
{
private:
	 
	bool _onlyuseresetter = false;
	uint32_t Reset_Timer = 60000;
	myTimer TIM;
	Basiskommunikation* _mqtt = nullptr;
	void init(Basiskommunikation* mqtt);
 
public:
	WTD(Basiskommunikation* mqtt, bool onlyuseresetter, uint32_t resetzeit);
	WTD(Basiskommunikation* mqtt, bool onlyuseresetter);
	WTD(Basiskommunikation* mqtt);
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};


class ArduinoHWWTD : public BasisInterface
{
private:
	
public:
	ArduinoHWWTD();
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length) {return false;};
};
#endif //ARDUINOWTDHEADER
#pragma once
// CCC Todo: derzeit für (ESP8266) || defined(ESP32) noch keinen Hardwarewatchdog  drin,.---- > muss erstmal ausgiebig mit dem Watchdog auseinandersetzen
#define MYDEBUG 0
#include "mydebug.h"

 
#include "hilfe.h"
 
#include "basisinterface.h"
 
 
#include <esp_task_wdt.h>
#define WDT_TIMEOUT_S 60
 
 
 

class EspHwWTD : public BasisInterface
{
private:
	 
 
 
public:
 
	EspHwWTD();
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

 
#pragma once


#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



#include "basisinterface.h" 
#include "arduinowatchdog.h"
#include "FS.h"
 
#if defined(ESP32)
#include "SPIFFS.h"
#endif
 

class mySpiffsSettingsloaderString : public BasisInterface
{
private:
	bool somethingsaved = false;
	char gespeicherterName[40];
	const char* _filepfad;
	const char* _stdname;
	void loadaddr();
	void saveaddr(char* sett);
	 
	 
 
public:
	const char* getSett();
	mySpiffsSettingsloaderString(const char* filepfad, const char* stdname);
	void update() {}; // nichts zu tun, nur um Basisklasse zu beruhigen
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};





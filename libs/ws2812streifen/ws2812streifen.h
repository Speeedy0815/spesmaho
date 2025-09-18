#define MYDEBUG 0
#include "mydebug.h"

#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basisinterface.h"
#include <Adafruit_NeoPixel.h>
#include "mytimer.h"

class Ws2812streifen : public BasisInterface
{
	public: 
	Ws2812streifen(uint8_t bandnummer ,uint32_t Pixelanzahl, uint8_t pin,uint8_t maxbrightness,bool store);
	~Ws2812streifen();
	
private:  
	uint8_t _bandnummer = 0;
	uint32_t _animationsnummer = 0;
	uint32_t _animationsfarbe = 0;
	uint32_t* data;
	bool _store = false;
	Adafruit_NeoPixel pixels;
	myTimer animationsupdatetimer;
	uint16_t animationsupdatetimerfreshzeit = 10; //alle 20 ms wird animation weitergemacht
		void setWRGB(uint8_t nr,uint32_t wrgb );
	void sethell(uint8_t helligkeit);  
 	void animiere();
	char _mode = 'A';
	void update() ; 
    bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
 



};

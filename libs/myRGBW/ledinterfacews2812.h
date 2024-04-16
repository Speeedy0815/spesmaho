#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basisinterface.h"
#include <Adafruit_NeoPixel.h>
#include "myRGBWbasis.h"

class LEDW2812Interface : public myRGBWbasis
{
private:  
	Adafruit_NeoPixel pixels;
	uint32_t* data;
	bool _store = false;
	
public: 
	LEDW2812Interface(uint8_t bandnummer ,uint8_t Pixelanzahl, uint8_t pin,uint8_t maxbrightness,bool store);
	void update(){}; 
	void setWRGB(uint8_t nr,uint32_t wrgb );
	void sethell(uint8_t helligkeit);  
	~LEDW2812Interface();
};

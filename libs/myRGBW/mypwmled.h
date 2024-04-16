#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basisinterface.h" 
#include "myRGBWbasis.h"
#include "hilfe.h"
#include "mytimer.h"


#if defined(ARDUINO_ARCH_ESP32)
#else
class mypwmled : public myRGBWbasis
{
private:  
	 uint8_t _pinR = MYPINUNUSED;
	 uint8_t _pinG = MYPINUNUSED;
	 uint8_t _pinB = MYPINUNUSED;
	 uint8_t _pinW = MYPINUNUSED;
	 myTimer TIM;



	 uint8_t rSoll = 0;
	 uint8_t gSoll = 0;
	 uint8_t bSoll = 0;
	 uint8_t wSoll = 0;

	 uint8_t rIst = 0;
	 uint8_t gIst = 0;
	 uint8_t bIst = 0;
	 uint8_t wIst = 0;

		  
public: 
	mypwmled(uint8_t bandnummer ,uint8_t pinR, uint8_t pinG,uint8_t pinB,uint8_t pinW);
	void update(); 
	void setWRGB(uint8_t nr,uint32_t wrgb );
	void sethell(uint8_t helligkeit) {}; //hier nicht implementiert --> ggf mal nachprogrammieren, wenn gewünscht
	 
};
#endif
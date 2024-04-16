#include "mypwmled.h"


#if defined(ARDUINO_ARCH_ESP32)
#else
mypwmled::mypwmled(uint8_t bandnummer, uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t pinW) :
	myRGBWbasis(bandnummer)
{
	if (pinR != MYPINUNUSED)
		_pinR = pinR;
	if (pinG != MYPINUNUSED)
		_pinG = pinG;
	if (pinB != MYPINUNUSED)
		_pinB = pinB;
	if (pinW != MYPINUNUSED)
		_pinW = pinW;

	if (_pinR != MYPINUNUSED)
		pinMode(_pinR, OUTPUT);
	if (_pinG != MYPINUNUSED)
		pinMode(_pinG, OUTPUT);
	if (_pinB != MYPINUNUSED)
		pinMode(_pinB, OUTPUT);
	if (_pinW != MYPINUNUSED)
		pinMode(_pinW, OUTPUT);

 
}

void mypwmled::update()
{
 
	 
		if (rSoll > rIst)rIst++;
		if (gSoll > gIst)gIst++;
		if (bSoll > bIst)bIst++;
		if (wSoll > wIst)wIst++;
		if (rSoll < rIst)rIst--;
		if (gSoll < gIst)gIst--;
		if (bSoll < bIst)bIst--;
		if (wSoll < wIst)wIst--;





		if (_pinR != MYPINUNUSED)
			analogWrite(_pinR, rIst);
		if (_pinG != MYPINUNUSED)
			analogWrite(_pinG, gIst);
		if (_pinB != MYPINUNUSED)
			analogWrite(_pinB, bIst);
		if (_pinW != MYPINUNUSED)
			analogWrite(_pinW, wIst);
	 
}


void mypwmled::setWRGB(uint8_t nr, uint32_t wrgb)
{
	rSoll = (wrgb >> 16);
	gSoll = (wrgb >> 8);
	bSoll = (wrgb >> 0);
	wSoll = (wrgb >> 24);
}
#endif
#include "ledinterfacews2812.h"


LEDW2812Interface::LEDW2812Interface(uint8_t bandnummer, uint8_t Pixelanzahl, uint8_t pin, uint8_t maxbrightness, bool store) :
	pixels(Pixelanzahl, pin, NEO_GRB + NEO_KHZ800),
	myRGBWbasis(bandnummer)
{
	_store = store;
	if (_store)
	{

		data = new uint32_t[Pixelanzahl];

		for (int i = 0; i < Pixelanzahl; i++)
		{

			data[i] = 0;
		}
	}

	pixels.begin();
	for (int i = 0; i < Pixelanzahl; i++) { pixels.setPixelColor(i, pixels.Color(0, 0, 0)); }
	sethell(maxbrightness);
	pixels.show();

}
LEDW2812Interface::~LEDW2812Interface()
{
	if (_store)
	{
		delete[] data;
	}
}
void LEDW2812Interface::sethell(uint8_t helligkeit)
{

	pixels.setBrightness(helligkeit);
	if (_store)
	{
		for (int i = 0; i < pixels.numPixels(); i++)
		{

			pixels.setPixelColor(i, data[i]);
		}

	}
	pixels.show();
}


void LEDW2812Interface::setWRGB(uint8_t nr, uint32_t wrgb)
{

	if (_store)
	{
		data[nr] = wrgb;
	}
	pixels.setPixelColor(nr, wrgb);
	pixels.show();
}

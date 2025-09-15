#pragma once
#define MYDEBUG 0
#include "mydebug.h"
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basisinterface.h" 
#include <Adafruit_NeoPixel.h>
#include "myUhrBasis.h"
#include "mytimer.h"


class myLEDMatrix : public BasisInterface
{
private:  
    Adafruit_NeoPixel pixels;
    uint16_t _currentLed = 0;
    uint8_t _anzahllauftext = 5;
    char _mode = 'T';
    static const uint16_t _width = 32;
    static const uint16_t _height = 8;
    static const uint16_t _Pixelanzahl = _width * _height;

    char textBuffer[100]; // 50-char buffer with example text
    uint32_t tickCounter = 0; // Tick counter for scrolling
public: 
    myLEDMatrix(uint8_t pin, myUhrBasis* Uhrzeit);
    void update(); 
    void drawText(const char* text); 

    

	void drawUhrzeit(); 
    bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
private:
    uint16_t xyToIndex(uint8_t x, uint8_t y); // Hilfsfunktion für Mäander	 
	myUhrBasis* _Uhrzeit;
	myTimer Uhrtimer;
	uint16_t Uhrrefreshzeit = 10000; //alle 10 Sekunden wird Uhrzeit neu dargestellt
    void drawTextDyn(); // New function for dynamic scrolling text
	
    myTimer DyntextTimer;
    uint16_t DynTextrefreshzeit = 50; //dyn Text aktualisieren --> damit Geschwindigkeit
     
};

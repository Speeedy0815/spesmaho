#pragma once
#define MYDEBUG 0
#include "mydebug.h"



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



#include "hilfe.h"
#include "mytimer.h"
#include "basisinterface.h"
#include <TFT_eSPI.h> // Hardware-specific library,

#include "mydisplay.h"


class DisplayAudioteil : public BasisInterface
{
private:
	int32_t Dauergesamt = 100;
	int32_t Dauerjetzt = 20;



	static const uint32_t resetkaestelfarbe = TFT_BLACK;

	void schreibeTitel(char* text);
	void ZeichneDauer();
	void schreibeArtist(char* text);

	myTimer Balkenaktualisierer;
	uint16_t BalkenaktualisiererZeit = 200; //ms

	MyDisp* _Display;
	TFT_eSPI* _tft;       // Invoke custom library
protected:

public:
	~DisplayAudioteil() {};
	DisplayAudioteil(MyDisp* Display);
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);

};

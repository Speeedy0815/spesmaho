#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif



#include "hilfe.h"
#include "mytimer.h"
#include "basisinterface.h"
#include <TFT_eSPI.h> // Hardware-specific library,
#include "myheiztemp.h"
#include "myhdc108.h"
#include "mydisplay.h"


class DisplayHeizungsteil : public BasisInterface
{
private:
	const char* _MYMQTTANTWORTADR = "Hz/D";
	static const uint32_t resetkaestelfarbe = TFT_BLACK;
	void ZeichneHeizungsButton();
	void aktualisiereInnentemperatur();
	void aktualisiereSolltemperatur();
	void aktualisiereheizungskreise();
	void handleButtonHeizung();
	void aktualisiereLuftfeuchte();
	int8_t  gibemepressedButton(TS_Point p);

	MyDisp* _Display;
	HeizTemp* _heizReglung;
	TFT_eSPI* _tft;       // Invoke custom library
	MyHDC108* _temperatursensor;
	XPT2046_Touchscreen* _ts;  // Param 2 - Touch IRQ Pin - interrupt enabled polling
	myTimer ButtonSendelimiter;
	uint16_t sendelimitButton = 200; //ms
protected:

public:
	~DisplayHeizungsteil() {};
	DisplayHeizungsteil(MyDisp* Display, MyHDC108* temperatursensor, HeizTemp* heizReglung);
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length) { return false; };

};

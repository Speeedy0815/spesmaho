#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include "myBasisHW.h"
#include "hilfe.h"
#include "mytimer.h"


class BasisBeleuchtungDisplay
{
private:

protected:

public:
	~BasisBeleuchtungDisplay() {};
	BasisBeleuchtungDisplay() {};
	virtual void update() = 0;
	virtual void schalteAn() = 0;
	virtual void schalteAus() = 0;
	virtual bool istDisplayAn() = 0;

};

class DigitalBeleuchtungDisplay :public BasisBeleuchtungDisplay
{
private:
	myBasisHW* _Hardware;
	uint8_t _ledpin = MYPINUNUSED;
	bool _state = false;
	void schaltestate();

protected:

public:
	~DigitalBeleuchtungDisplay() {};
	DigitalBeleuchtungDisplay(myBasisHW* Hardware, uint8_t ledpin);
	void update() {}; // brauche ich hier nicht
	void schalteAn();
	void schalteAus();
	bool istDisplayAn();

};

class PWMBeleuchtungDisplay :public BasisBeleuchtungDisplay
{
private:
	myTimer Hintergrundbeleuchtungstimer;
	uint16_t Hintergrundbeleuchtungstimerzeit = 10;
	int16_t dutyCycleHintergrundbeleuchtung = 0;

	uint8_t _ledpin = MYPINUNUSED;

	const int freqhintergrundbel = 5000;
	const int resolutionhintergrundbel = 8;

	bool sollstate = false;
	void setHintergrund(int8_t val);
protected:

public:
	~PWMBeleuchtungDisplay() {};
	 PWMBeleuchtungDisplay(uint8_t ledpin);
	void update();
	void schalteAn();
	void schalteAus();
	bool istDisplayAn();

};
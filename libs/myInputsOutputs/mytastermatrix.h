#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basiskommunikation.h" 
#include "myInput.h"
#include "myBasisHW.h"

class myTastermatrix : public BasisInterface
{
private:

	Basiskommunikation* _mqtt;
	myBasisHW* _Hardware;

	uint8_t _anzSpal;
	uint8_t _anzZeil;
	uint8_t* _Spaltlst;
	uint8_t* _Zeillist;

	myInput** ResTaster;

public:
	myTastermatrix(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t anzSpal, uint8_t anzZeil, uint8_t* Spaltlst, uint8_t* Zeillist, uint8_t* Keyliste);
	~myTastermatrix();

	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
	//void updateZeile(uint8_t zeilennummer, uint16_t zeilenergebnis);

};

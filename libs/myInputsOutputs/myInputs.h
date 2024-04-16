#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basiskommunikation.h" 
#include "myInput.h"
#include "myBasisHW.h"
#include "mytimer.h"

class myInputs : public BasisInterface
{
private:
	Basiskommunikation* _mqtt;
	myBasisHW* _Hardware;
	uint8_t _anzInp; 
	uint8_t* _Inplst;
	bool* _turnLogic;
	myInput** ResTaster;
	unsigned int _refreshzeit = 10000;   // ms how to refresh the outputs
	myTimer refreshtimer; 
	  
public: 
 	myInputs(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t anzInp, uint8_t* Inplst,  uint8_t* Loglist, bool* actLongsend, bool* actpullUp);
	~myInputs();   

	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
	 
};




 
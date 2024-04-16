#pragma once

#define MYDEBUG 0
#include "mydebug.h"


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basiskommunikation.h" 
#include "myOutput.h"
#include "myBasisHW.h"  //nur zum durchreichen der Hardware, eigentlich würde auch nen void pointer reichen
#include "mytimer.h" 

class myOutputs : public BasisInterface
{
private:
	uint8_t _anzOutp; 
	myOutput** Outputliste;
	unsigned int _refreshzeit = 60000;   // ms how to refresh the outputs
	myTimer refreshtimer; 
public: 
 	myOutputs(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t anzOutp, uint8_t* Outplst,  uint8_t* Loglist, bool* Drehliste);
	~myOutputs();   

	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
	 
};




 
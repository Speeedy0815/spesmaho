#include "myOutputs.h"


myOutputs::myOutputs(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t anzOutp, uint8_t* Outplst, uint8_t* Loglist, bool* Drehliste)
{
	 
	this->_anzOutp = anzOutp;
//	this->_Outplst = Outplst;  // brauche ich das?


	Outputliste =  new      (myOutput * [_anzOutp]);
	for (uint8_t i = 0; i < _anzOutp; i++)
	{
		 
		Outputliste[i] = new myOutput(mqtt, Hardware, Loglist[i], Outplst[i],Drehliste[i]);
	}
	refreshtimer.start(_refreshzeit,true);
 
}
myOutputs::~myOutputs()
{
	for (uint8_t i = 0; i < _anzOutp; i++)
	{
		delete Outputliste[i];
	}
	delete[] Outputliste;
}

void myOutputs::update()
{
	for (uint8_t i = 0; i < _anzOutp; i++)
	{
		
		if (refreshtimer.update_darfich())
		{
			Outputliste[i]->refresh();
			refreshtimer.start(_refreshzeit,false);
			debugln("Refresh");
		}			
		Outputliste[i]->update();
	}
}



bool  myOutputs::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	for (uint8_t i = 0; i < _anzOutp; i++)
	{
		if (Outputliste[i]->callbackismineanddo(topic, payload, length))
		{
			return true;
		};
	}
	return false;
}

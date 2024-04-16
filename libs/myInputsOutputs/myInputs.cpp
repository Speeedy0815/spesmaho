#include "myInputs.h"


myInputs::myInputs(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t anzInp, uint8_t* Inplst, uint8_t* Loglist, bool* actLongsend, bool* turnLogic)
{
	_mqtt = mqtt;
	this->_Hardware = Hardware;

	this->_anzInp = anzInp;
	this->_Inplst = Inplst;
	this->_turnLogic = turnLogic;

	ResTaster = new  (myInput * [_anzInp]);
	for (uint8_t i = 0; i < _anzInp; i++)
	{
		ResTaster[i] = new myInput(_mqtt, Loglist[i], actLongsend[i]);
		ResTaster[i]->begin(false);  //Todo hier auslesen!!! CCC ,... wenn dann auch ein hardware update nicht vergessen!, das ist am anfang noch nicht gelaufen
		_Hardware->set_as_Input_Pullup(_Inplst[i]);

	}
	refreshtimer.start(_refreshzeit,true);

}
myInputs::~myInputs()
{
	for (uint8_t i = 0; i < _anzInp; i++)
	{
		delete ResTaster[i];
	}
	delete[] ResTaster;
}

void myInputs::update()
{
	for (uint8_t i = 0; i < _anzInp; i++)
	{
		
		if (refreshtimer.update_darfich())
		{
			_Hardware->set_as_Input_Pullup(_Inplst[i]);
			refreshtimer.start(_refreshzeit,false);
			debugln("Refresh");
		}	
		
	 
		
		bool erg = (_Hardware->getBit(_Inplst[i]));
		
		if (this->_turnLogic[i])
		{
			erg = !erg;
		}


		ResTaster[i]->update(erg);
	}
}



bool  myInputs::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	for (uint8_t i = 0; i < _anzInp; i++)
	{
		if (ResTaster[i]->callbackismineanddo(topic, payload, length))
		{
			return true;
		};
	}
	return false;
}

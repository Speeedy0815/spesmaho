#include "mytastermatrix.h"


myTastermatrix::myTastermatrix(Basiskommunikation* mqtt, myBasisHW* Hardware,
	uint8_t anzSpal, uint8_t anzZeil, uint8_t* Spaltlst, uint8_t* Zeillist, uint8_t* Keyliste)
{
	_mqtt = mqtt;
	this->_Hardware = Hardware;

	_anzSpal = anzSpal;
	_anzZeil = anzZeil;
	_Spaltlst = Spaltlst;
	_Zeillist = Zeillist;

	ResTaster = (myInput**) new      (myInput * [(_anzSpal * _anzZeil)]);
	for (uint8_t i = 0; i < _anzSpal; i++)
	{
		for (uint8_t j = 0; j < _anzZeil; j++)
		{
			ResTaster[3 * i + j] = new myInput(_mqtt, Keyliste[j * 3 + i],true);
			ResTaster[3 * i + j]->begin(false);
		}
	}
	for (uint8_t i = 0; i < _anzZeil; i++)
	{
		_Hardware->set_as_Input_Pullup(_Zeillist[i]);
	}
	for (uint8_t i = 0; i < _anzSpal; i++)
	{
		_Hardware->set_as_Output(_Spaltlst[i]);
		_Hardware->setBit(_Spaltlst[i], HIGH);
	}
}
myTastermatrix::~myTastermatrix()
{
	for (uint8_t i = 0; i < _anzSpal; i++)
	{
		for (uint8_t j = 0; j < _anzZeil; j++)
		{
			delete ResTaster[3 * i + j]; 
		}
	}
	delete[] ResTaster;
}


void myTastermatrix::update()
{
	for (uint8_t b = 0; b < _anzZeil; b++)
	{
		_Hardware->set_as_Input_Pullup(_Zeillist[b]);
	}
	for (uint8_t i = 0; i < _anzSpal; i++)
	{
		_Hardware->set_as_Output(_Spaltlst[i]);
		_Hardware->setBit(_Spaltlst[i], LOW);
		_Hardware->update();
		//Serial.print("i: ");Serial.println(i);

		for (uint8_t j = 0; j < _anzZeil; j++)
		{
			//Serial.print("j: "); Serial.println(j);
			bool erg = !(_Hardware->getBit(_Zeillist[j]));
			//Serial.println(erg);
			uint8_t mtxnr = i * _anzSpal + j;  //J = Zeile
			//Serial.println(mtxnr);
			ResTaster[mtxnr]->update(erg);

		}
		_Hardware->setBit(_Spaltlst[i], HIGH);
	}


}



bool  myTastermatrix::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	for (uint8_t i = 0; i < _anzSpal; i++)
	{
		for (uint8_t j = 0; j < _anzZeil; j++)
		{
			if (ResTaster[3 * i + j]->callbackismineanddo(topic, payload, length))
			{
				return true;
			}
		}
	}

	return false;
}

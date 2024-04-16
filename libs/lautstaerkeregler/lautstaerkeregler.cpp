#include "lautstaerkeregler.h"
MyLSR::MyLSR(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t datapin, uint8_t clockpin, uint8_t nummer)
{
	TIM.start(180000, true);  
	_Hardware = Hardware;
	_mqtt = mqtt;
	_nummer = nummer;
	_datapin = datapin;
	_clockpin = clockpin;


	 

	_Hardware->set_as_Output(_datapin);
	_Hardware->set_as_Output(_clockpin);
 

	_Hardware->setBit(_datapin, 0);
	_Hardware->setBit(_clockpin, 0);
 
}

void MyLSR::setVolume(uint8_t volume)
{
	uint8_t bits;
	uint16_t data = 0; // control word is built by OR-ing in the bits
	if (volume >= maxi)
	{
		volume = maxi - 1;
	}
	volume = lot[maxi - 1 - volume];
	//Serial.println(volume);
	// convert attenuation to volume
	//volume = (volume > 100) ? 0 : (((volume * 83) / -100) + 83); // remember 0 is full volume!

		  // generate 10 bits of data
	data |= (0 << 0); // D0 (channel select: 0=ch1, 1=ch2)
	data |= (0 << 1); // D1 (individual/both select: 0=both, 1=individual)
	data |= ((21 - (volume / 4)) << 2); // D2...D6 (ATT1: coarse attenuator: 0,-4dB,-8dB, etc.. steps of 4dB)
	data |= ((3 - (volume % 4)) << 7); // D7...D8 (ATT2: fine attenuator: 0...-1dB... steps of 1dB)
	data |= (0b11 << 9); // D9...D10 // D9 & D10 must both be 1

	for (bits = 0; bits < 11; bits++) { // send out 11 control bits
		delayMicroseconds(2); // pg.4 - M62429P/FP datasheet
		_Hardware->setBit(_datapin, 0);
		delayMicroseconds(2);
		_Hardware->setBit(_clockpin, 0);
		delayMicroseconds(2);
		_Hardware->setBit(_datapin, (data >> bits) & 0x01);
		delayMicroseconds(2);
		_Hardware->setBit(_clockpin, 1);
	}
	delayMicroseconds(2);
	_Hardware->setBit(_datapin, 1);
	delayMicroseconds(2);
	_Hardware->setBit(_clockpin, 0);
	//return data; // return bit pattern in case you want it :)
}
void MyLSR::update()
{
	if (TIM.update_darfich())
	{
		sendvol();  // ist für Serverneustartda, dann werden ab und zu mal die Lautstärke aktualisiert
	}
}
bool  MyLSR::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	 
	if (length < 3)
		return false;
	if (length > 4)
		return false;
	if (payload[0] != 'V')
		return false;
	if (payload[1] != _nummer + '0')
		return false;


	 
	switch (payload[2])
	{
	case '+':
		Volumen++;
		break;
	case '-':
		Volumen--;
		break;
	default:  // gehe von Zahl aus
		payload[length] = 0;
		Volumen = atoi((char*)payload + 2);
		break;
	}



	if (Volumen >= maxi)
	{
		Volumen = maxi - 1;
	}
	if (Volumen <= 0)
	{
		Volumen = 0;
	}


 


	setVolume(Volumen);
	sendvol();


	return true;
}
void MyLSR::sendvol()
{
	char payloadbuf[8];
	itoa(Volumen, payloadbuf, 10);

	
	char topicbuf[30];
	uint8_t lentopic = strlen(_MYMQTTANTWORTADR);
	strcpy(topicbuf, _MYMQTTANTWORTADR);
	topicbuf[lentopic] = _nummer + '0';
	topicbuf[lentopic + 1] = 0;
	 

	_mqtt->sendmessage(topicbuf, payloadbuf);
}
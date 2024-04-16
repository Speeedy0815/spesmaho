#include "myEncoderESP.h"
myEncoderESP::myEncoderESP(Basiskommunikation* mqtt, uint8_t encoderPinA, uint8_t encoderPinB, uint8_t encodernummer)
{
 
	_encoderPinA = encoderPinA;
	_encoderPinB = encoderPinB;

	GLOBINTPINA = encoderPinA;
	GLOBINTPINB = encoderPinB;

	_mqtt = mqtt;
	_encnr = encodernummer;

 
	pinMode(_encoderPinA, INPUT_PULLUP);
	pinMode(_encoderPinB, INPUT_PULLUP);

	aktiviereInterrupts();

}
void myEncoderESP::aktiviereInterrupts()
{
	attachInterrupt(_encoderPinA, doInter, CHANGE); // encoder pin on interrupt 0 (pin 2)
	attachInterrupt(_encoderPinB, doInter, CHANGE); // encoder pin on interrupt 1 (pin 3)

}
void myEncoderESP::deaktiviereInterrupts()
{
	detachInterrupt(_encoderPinA); // encoder pin on interrupt 0 (pin 2)
	detachInterrupt(_encoderPinB); // encoder pin on interrupt 1 (pin 3)
}

void IRAM_ATTR doInter() {
 
	GLOBVALUEA = digitalRead(GLOBINTPINA);
	GLOBVALUEB = digitalRead(GLOBINTPINB);
	uint8_t revent = GLOBROT.process(GLOBVALUEB, GLOBVALUEA);
	if (revent == DIR_CW)
	{
		GLOBWert++;
	}
	if (revent == DIR_CCW)
	{
		GLOBWert--;
	}
	
}


void myEncoderESP::update()
{
	if (GLOBWert > 0)
	{
		GLOBmysendmessageWithAddr(_mqtt, "+", _topic, _encnr);
		GLOBWert--;
	}
	if (GLOBWert < 0)
	{
		GLOBmysendmessageWithAddr(_mqtt, "-", _topic, _encnr);
		GLOBWert++;
	}
 
}
bool  myEncoderESP::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	return false;
}

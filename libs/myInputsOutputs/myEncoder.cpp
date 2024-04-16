#include "myEncoder.h"
myEncoder::myEncoder(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t encoderPinA, uint8_t encoderPinB, uint8_t encodernummer)
{
	_Hardware = Hardware;
	_encoderPinA = encoderPinA;
	_encoderPinB = encoderPinB;

	_mqtt = mqtt;
	_encnr = encodernummer;

	_Hardware->set_as_Input_Pullup(_encoderPinA);
	_Hardware->set_as_Input_Pullup(_encoderPinB);


}





void myEncoder::update()
{
	_Hardware->update();
	uint8_t revent = rot.process(_Hardware->getBit(_encoderPinB), _Hardware->getBit(_encoderPinA));
	if (revent == DIR_CW)
		GLOBmysendmessageWithAddr(_mqtt, "+", _topic, _encnr);
	if (revent == DIR_CCW)
		GLOBmysendmessageWithAddr(_mqtt, "-", _topic, _encnr);
}
bool  myEncoder::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	return false;
}

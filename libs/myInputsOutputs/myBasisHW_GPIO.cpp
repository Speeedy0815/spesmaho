 



#include "myBasisHW_GPIO.h"


 
bool myBasisHW_GPIO::getBit(uint8_t nr)
{
	return  (digitalRead(nr));
}
void myBasisHW_GPIO::setBit(uint8_t nr, uint8_t an_aus)
{
	digitalWrite(nr, an_aus);
}

void myBasisHW_GPIO::set_as_Output(uint8_t nr)
{ 
	pinMode(nr, OUTPUT);
}
void myBasisHW_GPIO::set_as_Input(uint8_t nr)
{
	pinMode(nr, INPUT);
}
void myBasisHW_GPIO::set_as_Input_Pullup(uint8_t nr)
{
	pinMode(nr, INPUT_PULLUP);
}



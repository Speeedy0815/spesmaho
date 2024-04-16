 



#include "myBasisHW_MCP.h"

void myBasisHW_MCP_Resetit(uint8_t pin)
{
	Serial.println("Reset MCPs");
	digitalWrite(pin, LOW);
	delay(200);
	pinMode(pin, OUTPUT);  
	digitalWrite(pin, HIGH);
	delay(200);
}

myBasisHW_MCP::myBasisHW_MCP(uint8_t mcpaddr)
{

	mymcp.begin_I2C(mcpaddr);
	update();
}


void myBasisHW_MCP::update()
{
	abbild = mymcp.readGPIOAB();
	debugln(abbild);
}
bool myBasisHW_MCP::getBit(uint8_t nr)
{
	return  ((abbild & (1 << nr)) > 0);
}
void myBasisHW_MCP::setBit(uint8_t nr, uint8_t an_aus)
{
	mymcp.digitalWrite(nr, an_aus);
}

void myBasisHW_MCP::set_as_Output(uint8_t nr)
{
	//Serial.print("set_as_Output"); Serial.println(nr);
	mymcp.pinMode(nr, OUTPUT);
}
void myBasisHW_MCP::set_as_Input(uint8_t nr)
{
	//Serial.print("set as input"); Serial.println(nr);
	mymcp.pinMode(nr, INPUT);
}
void myBasisHW_MCP::set_as_Input_Pullup(uint8_t nr)
{
	//Serial.print("set as input pullup"); Serial.println(nr);
	mymcp.pinMode(nr, INPUT_PULLUP);
	//pullUp(nr, HIGH);
	//set_as_Input(nr);
}
void myBasisHW_MCP::activatePinForInterrupt(uint8_t nr,uint8_t mode)
{
	//Kommentar ist aus einem Beispiel �bernommen
	// We mirror INTA and INTB, so that only one line is required between MCP and Arduino for int reporting
	// The INTA/B will not be Floating 
	// INTs will be signaled with a LOW
	mymcp.setupInterrupts(true, false, LOW);
	mymcp.setupInterruptPin(nr, mode);
}


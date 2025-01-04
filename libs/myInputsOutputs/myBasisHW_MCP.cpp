 



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

	if(!mymcp.begin_I2C(mcpaddr + MCP23XXX_ADDR))//ggf mal noch ne 20 draufrechnen
	{
		Serial.println("MCP23X17 Fehler bei der Initialisierung!");
		while(1);
	}
	update();
}


void myBasisHW_MCP::update()
{
	
	
 
	abbild = mymcp.readGPIOAB(); // Aktuellen Wert lesen
 
	//debugln(abbild);
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
	debug("set_as_Output"); debugln(nr);
	
	 
	mymcp.pinMode(nr, OUTPUT);
}
void myBasisHW_MCP::set_as_Input(uint8_t nr)
{
	debug("set as input, achtung ohne pullup"); debugln(nr);
	mymcp.pinMode(nr, INPUT);
}
void myBasisHW_MCP::set_as_Input_Pullup(uint8_t nr)
{
	debug("set as input pullup"); debugln(nr);
	mymcp.pinMode(nr, INPUT_PULLUP);

	//mymcp.pullUp(nr, HIGH);
	//set_as_Input(nr);
}
void myBasisHW_MCP::activatePinForInterrupt(uint8_t nr,uint8_t mode)
{
	//Kommentar ist aus einem Beispiel übernommen
	// We mirror INTA and INTB, so that only one line is required between MCP and Arduino for int reporting
	// The INTA/B will not be Floating 
	// INTs will be signaled with a LOW
	mymcp.setupInterrupts(true, false, LOW);
	mymcp.setupInterruptPin(nr, mode);
}


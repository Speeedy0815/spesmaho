#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include "myBasisHW.h"




class myBasisHW_GPIO: public myBasisHW
{
private: 

protected:

public:
 
	void update() {};
	bool getBit(uint8_t nr) ;
	void setBit(uint8_t nr,uint8_t an_aus) ;	
	
	void set_as_Output(uint8_t nr) ;
	void set_as_Input(uint8_t nr) ;
	void set_as_Input_Pullup(uint8_t nr) ;
	
	void activatePinForInterrupt(uint8_t nr, uint8_t mode){Serial.println("XXX Das macht hier keinen Sinn!!!");};
	
};
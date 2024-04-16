#include "arduinowatchdog.h"



void globalResetme()
{
	#if defined(__AVR__) 
		resetFunc();  //call reset
	#endif // defined(__AVR__) 
	#if defined(ESP32) ||  defined(ESP8266)
		ESP.restart();
	#endif // defined(ESP32) 	

}


WTD::WTD(Basiskommunikation* mqtt)
{
	init(mqtt);
}
WTD::WTD(Basiskommunikation* mqtt, bool onlyuseresetter)
{
	_onlyuseresetter = onlyuseresetter;
	init(mqtt);
}
 
WTD::WTD(Basiskommunikation* mqtt, bool onlyuseresetter, uint32_t resetzeit)
{
	Reset_Timer = resetzeit;
	_onlyuseresetter = onlyuseresetter;
	init(mqtt);
}

void WTD::init(Basiskommunikation* mqtt)
{


	_mqtt = mqtt;
	TIM.start(Reset_Timer, false);
}



 

void WTD::update() {
	 
	if (_mqtt)
	{
		if (_onlyuseresetter == false)
		{
			if (TIM.update_darfich())
			{
				Serial.println(F("RES, no WTD triggered"));
				_mqtt->sendmessage("ALLERT", "R");
				delay(1000);
				globalResetme();
			}
		}
	}
 
 


}
bool WTD::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length < 3)
		return false;
	if ((payload[0] != 'W') && (payload[0] != 'R'))
		return false;
	if ((payload[1] != 'T') && (payload[1] != 'E'))
		return false;
	if ((payload[2] != 'D') && (payload[2] != 'S'))
		return false;


	if (payload[0] == 'W')
	{
		//ResetTimer
		debugln(F("ResetSWWatchdog"));
		TIM.start(Reset_Timer, false);
	}
	if (payload[0] == 'R')
	{
		debugln(F("Sestart from Server"));
		_mqtt->sendmessage("ALLERT", "S");
		delay(1000);

		globalResetme();
	}

	return true;
}






 
ArduinoHWWTD::ArduinoHWWTD()
{
	#if defined(__AVR__) 	
		wdt_enable(WDTO_8S);
	#endif // defined(__AVR__) 
}
 
void ArduinoHWWTD::update()
{
	#if defined(__AVR__) 	
	wdt_reset();
	 
	#endif // defined(__AVR__) 		
}
	
 
#include "mybeeper.h"



MyBeep::MyBeep(Basiskommunikation* mqtt)
{

	_mqtt = mqtt;

	ledcSetup(ledChannel, freq, resolution);

	// attach the channel to the GPIO to be controlled
	ledcAttachPin(ledPin, ledChannel);
	ledcWrite(ledChannel, 0);
	TIMER.start(TIMERZeit, false);
}


void MyBeep::update()
{
 
	if (TIMER.update_darfich())
	{
		if (dauer > 0)
		{
			dauer --;
		}
	}

	if (dauer <= 0)
	{
		ledcWrite(ledChannel, 0);  //ausschalten
	}

}
bool  MyBeep::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length < 3)
		return false;
	if (length > 10)
		return false;
	if (payload[0] != 'P')
		return false;
	//P für Piel
	//0-9 Tonhöhe
	//0-9 Dauer

	freq = 1000 - (payload[1] - '0' - 5) * 100;
	debug("Frequenz: ");
	debugln(freq);

	dauer = 2 + ((payload[2] - '0') * 10);

	ledcSetup(ledChannel, freq, resolution);
	ledcWrite(ledChannel, 127);  
	TIMER.start(TIMERZeit, false);  //Timerreset, dass das piepsen nicht sofort wieder ausgeschaltet wird

	return true;
}

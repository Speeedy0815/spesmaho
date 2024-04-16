#include "myhelligkeit.h"
MyHell::MyHell(Basiskommunikation* mqtt)
{

	_mqtt = mqtt;
	TIM.start(sendezeit, true);
}

void MyHell::send(uint32_t val)
{
	char buffer[8];
	itoa(val, buffer, 10);
	_mqtt->sendmessage(_MYMQTTANTWORTADR, buffer);
	lastsent = val;

}
void MyHell::update()
{
	uint32_t act_val = analogRead(potPin);
	if (TIM.update_darfich())
	{
		send(act_val);
	}
	int32_t dif = (int32_t)lastsent - (int32_t)act_val;
	if (abs(dif) > dif_whentosend)
	{
		send(act_val);
	}
}
bool  MyHell::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	return false;
}

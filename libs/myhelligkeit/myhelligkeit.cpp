#include "myhelligkeit.h"


const int FILTER_SIZE = 10;
uint32_t filterBuffer[FILTER_SIZE];
int filterIndex = 0;
bool bufferFilled = false;

uint32_t filterAnalog(uint32_t newVal) {
    filterBuffer[filterIndex++] = newVal;
    if (filterIndex >= FILTER_SIZE) {
        filterIndex = 0;
        bufferFilled = true;
    }

    uint64_t sum = 0;
    int count = bufferFilled ? FILTER_SIZE : filterIndex;
    for (int i = 0; i < count; i++) {
        sum += filterBuffer[i];
    }
    return sum / count;
}



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
	uint32_t act_val = filterAnalog(analogRead(potPin));
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

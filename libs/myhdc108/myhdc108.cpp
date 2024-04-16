// 
// 
// 

#include "myhdc108.h"



MyHDC108::MyHDC108(Basiskommunikation* mqtt) :hdc1080()
{
	_mqtt = mqtt;
	hdc1080.begin(0x40);
	Sendetimer.start(sendebegrenzunginms_Temperature, true);
	Updatetimer.start(updatetimerzeit, true);
	
}
void MyHDC108::update()
{
	if (Updatetimer.update_darfich())
	{
		updateSensoren();
	}
	if (Sendetimer.update_darfich())
	{
		updateSensoren();
		sendeSensoren();
	}

}
void MyHDC108::updateSensoren()
{
	double tmptemperature = hdc1080.readTemperature();
	if (tmptemperature > 60) // unplausibel, írgendwas ging schief, I2C wird neu initialisiert
	{
		Serial.println("Temperatur unplausibel, Lade I2C neu");
		hdc1080.begin(0x40);
		tmptemperature = hdc1080.readTemperature(); //neu einlesen
	}
	lasttemp = tmptemperature;//wenn jetzt immer noch mehr Temperatur drauf war, war es vielleicht wirklich mehr als 60°C
	lasthum = hdc1080.readHumidity();
}
void MyHDC108::sendeSensoren()
{
	_mqtt->sendmessage(_MYMQTTANTTEMP, lasttemp);
	_mqtt->sendmessage(_MYMQTTANTHUMI, lasthum);
}
bool MyHDC108::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	return false; //nur senden, nichts empfangen
}
double MyHDC108::getTemperature()
{
	return lasttemp;
}
double MyHDC108::getHuminity()
{
	return lasthum;
}

 
#pragma once
 
 
#include "basiskommunikation.h" 
#include <PubSubClient.h> 
 

class SerialDummy : public Basiskommunikation
{
public:
	~SerialDummy(){};
	SerialDummy(const char * MYMQTTNAME);
 // die Updatefunktion aus der Basiskommunikation wird genutzt
	void subscribeextra(const char* topic) {};  //macht keinen Sinn am Serial dummy
	//void setCallback(MQTT_CALLBACK_SIGNATURE);  in der Basiskommunikation
	void dopublish(const char* topic, const byte* msgbuffer,uint8_t len); //nichts tun, da es schon in der Basisklasse rausposaunt wird
private:
	//settings
	

	 
	

};

 

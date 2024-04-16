
#include "canmqttclient.h"

void CANMQTT::dopublish(const char* topic, const byte* msgbuffer, uint8_t len)
{
	_CANInterface.publish(topic, msgbuffer, len, CANFROMDEVICEOFFSET + _nr);
}




CANMQTT::CANMQTT(uint8_t nr, canbasis* hardware) :

	_CANInterface(hardware),
	Basiskommunikation()
{
	_nr = nr;
	sprintf(namenbuffer, "%d", _nr);
	Serial.print("CAN Client Addr: ");
	Serial.println(namenbuffer);
	Basiskommunikation::init(namenbuffer);
	
}

void CANMQTT::setCallback(MQTT_CALLBACK_SIGNATURE)
{
	this->callback = callback;
}


void CANMQTT::update() {
	static bool once = true; if (once) {
		setCallback(GLOBcallback); 
		dopublish(get_allTopic(), (byte*)"con", strlen("con"));
		once = false; 
	}
	

	Basiskommunikation::update();  // Serielle Schnittstelle
	mycan_frame canMsg;
	if (_CANInterface.updateandismessage(&canMsg))
	{
		//Serial.println("incoming Message, decode now");
		if ((canMsg.id == CANSENDTOALL) || (canMsg.id == (CANTODEVICEOFFSET + _nr)))
		{

			if (NACHR.wertenachrichtaus(&canMsg))
			{
				//Serial.print(F("payload: ")); Serial.println((char*)NACHR.payload);
				//Serial.print(F("topic: ")); Serial.println(NACHR.topic);
				uint8_t len = strlen((const char*)NACHR.payload);
				if  (callback){ callback(NACHR.topic, (unsigned char*)NACHR.payload, len); }
					
				
			}
		}
		else
		{
			//Serial.print(F("CAN wrong ID: ")); Serial.println(canMsg.id);
		}
	}



}



















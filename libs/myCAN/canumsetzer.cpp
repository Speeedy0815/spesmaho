#include "canumsetzer.h"

 
CanUmsetzer::CanUmsetzer(Basiskommunikation* mqtt, canbasis* hardware) :
	_CANInterface(hardware)
{
	
	_mqtt = mqtt;



	cantopic = new char[strlen(_mqtt->get_MQTTName()) + strlen(_topic) + 9];
	strcpy(cantopic, _mqtt->get_MQTTName());
	strcpy(cantopic + strlen(_mqtt->get_MQTTName()), "/");
	strcpy(cantopic + strlen(_mqtt->get_MQTTName()) + 1, _topic);
	strcpy(cantopic + strlen(_mqtt->get_MQTTName()) + 1 + strlen(_topic), "/todo/#");
	Serial.print(F("CAN Subscribe: ")); Serial.println(cantopic);
	_mqtt->subscribeextra(cantopic);
	delete[] cantopic;
	cantopic = new char[strlen(_mqtt->get_MQTTName()) + strlen(_topic) + 8];
	strcpy(cantopic, _mqtt->get_MQTTName());
	strcpy(cantopic + strlen(_mqtt->get_MQTTName()), "/");
	strcpy(cantopic + strlen(_mqtt->get_MQTTName()) + 1, _topic);
	strcpy(cantopic + strlen(_mqtt->get_MQTTName()) + 1 + strlen(_topic), "/todo/");

	_mqtt->set_Sub((BasisInterface*)this);
}


CanUmsetzer::~CanUmsetzer()
{
	delete[] cantopic;
}


uint8_t CanUmsetzer::getPufferNummerornextfree(long CANid)
{
	debug("suche Puffer fuer:"); debugln(CANid);
	for (uint8_t i = 0; i < MAXPUFFERNACHR; i++)  // erst benutzte Puffer nach ID suchen
	{
		if (NACHR[i].isinUse())
		{
			 
			if (NACHR[i].forWichCAN_IDUsed() == CANid)
			{
				 
				return i;
			}
		}
	}
	for (uint8_t i = 0; i < MAXPUFFERNACHR; i++)  // ersten UNbenutzten Puffer suchen
	{
		if (!(NACHR[i].isinUse()))
		{
			 
			return i;

		}
	}


	// Puffer voll, schwerwiegender Fehler!!! Puffer muss vergrößert werden!!!! Nachricht geht verloren!
	return CANNACHRPUFFVOLL;

}

 
void CanUmsetzer::update()
{
	mycan_frame canMsg;
	while (_CANInterface.updateandismessage(&canMsg))
	{
		uint8_t puffnummer = getPufferNummerornextfree(canMsg.id);
		if (CANNACHRPUFFVOLL == puffnummer)
		{// Puffer voll, schwerwiegender Fehler!!! Puffer muss vergrößert werden!!!! Nachricht geht verloren!
			_mqtt->sendmessage("ALLERT", "CANPuffer voll!");
		
			Serial.println(F("SCHWERER FEHLER, CAN PUFFER VOLL!!!!!"));
			return;
		}
		debugln("werte Nachricht aus");
		if (NACHR[puffnummer].wertenachrichtaus(&canMsg))
		{

			debugln("decoding done");


			if ((NACHR[puffnummer].topic[0] == '!') && (strlen(NACHR[puffnummer].topic) == 1))  //das hier ist ein Ident Frame --> ohne extra Name senden
			{
				NACHR[puffnummer].payload[NACHR[puffnummer].lenpayload] = 0;

				char* puffer = new char[strlen(_mqtt->get_MQTTName())
					+ strlen(_topic)
					+ strlen((char*)NACHR[puffnummer].payload)
					+ 3]; //+3 = 2x/+Stringende
				strcpy(puffer, _mqtt->get_MQTTName());
				strcpy(puffer + strlen(_mqtt->get_MQTTName()), "/");
				strcpy(puffer + strlen(_mqtt->get_MQTTName()) + 1, _topic);
				strcpy(puffer + strlen(_mqtt->get_MQTTName()) + 1 + strlen(_topic), "/");
				strcpy(puffer + strlen(_mqtt->get_MQTTName()) + 1 + strlen(_topic) + 1, (char*)NACHR[puffnummer].payload);

				_mqtt->sendmessageWithoutName("!", (byte*)puffer, (strlen(puffer)));

				delete[] puffer;

			}
			else   //--> stinknormales Frame --> weiterleiten mit Name
			{
				char* puffer = new char[strlen(NACHR[puffnummer].topic) + strlen(_topic) + 2];
				strcpy(puffer, _topic);
				strcpy(puffer + strlen(_topic), "/");
				strcpy(puffer + strlen(_topic) + 1, NACHR[puffnummer].topic);
	 
				_mqtt->sendmessage(puffer, NACHR[puffnummer].payload, NACHR[puffnummer].lenpayload);
				delete[] puffer;
			}
		}
	}

}
bool  CanUmsetzer::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if ((length == 1) && (topic[0] == '?')) // Ident an CAN weiterschicken
	{
		_CANInterface.publish(topic, payload, length, CANSENDTOALL);
		return true;
	}




	uint8_t cantopiclen = strlen(cantopic);  // ist es ein todo für den CAN
	uint8_t mqtttopiclen = strlen(topic);  // ist es ein todo für den CAN
	for (uint8_t i = 0; i < cantopiclen; i++)
	{
		if (cantopic[i] != topic[i])
		{
			return false;
		}
	}
	if (mqtttopiclen <= cantopiclen)
	{
		return false; //Topic muss länger sein, da noch eine Zahl dran sein muss!!!
	}
	for (uint8_t i = cantopiclen; i < mqtttopiclen; i++)  //Erkennung, sind nur noch Zahlen am Topicende
	{
		if (topic[i] > '9')
		{
			return false;
		}
		if (topic[i] < '0')
		{
			return false;
		}
	}



	int nummer;

	nummer = atoi(topic + cantopiclen);
	//Serial.println(nummer);
	_CANInterface.publish("X", payload, length, CANTODEVICEOFFSET + nummer); //leeren Sting, um Bytes auf dem Can zu sparen
	return true;


}



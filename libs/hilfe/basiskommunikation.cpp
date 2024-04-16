


#include "basiskommunikation.h"



//##############################################################################
// Basiskommunikationsklasse
void  Basiskommunikation::sendmessage(const char* topic, const byte* msgbuffer, uint8_t len)  //Binärzeugs verschicken
{
	uint8_t lenT = strlen(topic);
	uint8_t lenN = strlen(_MQTTNAME);

	char* newtopic = new  char[lenT + lenN + 2];

	strcpy(newtopic, _MQTTNAME);
	strcpy(newtopic + lenN, "/");
	strcpy(newtopic + lenN + 1, topic);



	debug(" in Topic: ");
	debugln(newtopic);

	dopublish(newtopic, msgbuffer, len);  //hier die Implementierung in den Hardwareabhängigen Klassen

	delete[] newtopic;
}
void Basiskommunikation::sendmessageWithoutName(const char* topic, const byte* msgbuffer, uint8_t len)
{
	dopublish(topic, msgbuffer, len);
}
void Basiskommunikation::sendmessage(const char* topic, const char* msgbuffer)
{
	debug("SenMes: ");
	debug(msgbuffer);
	sendmessage(topic, (byte*)msgbuffer, strlen(msgbuffer));
}
void Basiskommunikation::sendmessage(const char* topic, double  wert)
{
	char buffer[30];
	dtostrf(wert, 0, 2, buffer);
	sendmessage(topic, buffer);
}
void Basiskommunikation::sendmessage(const char* topic, int32_t  wert)
{
	char buffer[30];
	ltoa(wert, buffer, 10);
	sendmessage(topic, buffer);
}

void Basiskommunikation::update() {
	static bool once = true; if (once) { setCallback(GLOBcallback); once = false; }

	while (Serial.available()) {
		char inByte = Serial.read();


		if (ziffernzaehler > 28) // irgendwas ist schief gelaufen, Puffer darf nicht überschritten werden! Speicherüberschreiber
		{
			Serial.println(F("Puffer überschritten, beginne neu! Bishergesendetes wird geloescht!"));
			ziffernzaehler = 0;
			zahl[0] = 0; // leerer String
		}
		if (inByte == 10 || inByte == 13) // Zeilenende --> Sende raus
		{

			zahl[ziffernzaehler] = 0;
			if (callback) {
				callback((char*)"egal", (uint8_t*)zahl, ziffernzaehler);
			}
			//Hier sollte Callback rein

			ziffernzaehler = 0;
			zahl[0] = 0; // leerer String
		}
		else
		{
			zahl[ziffernzaehler] = inByte;
			ziffernzaehler++;
		}
	}
}
void Basiskommunikation::setCallback(MQTT_CALLBACK_SIGNATURE)
{
	debugln("Basistcallback");
	this->callback = callback;

}

Basiskommunikation::Basiskommunikation(const char* MYMQTTNAME)
{
	Serial.print("MQTTName: ");
	Serial.println(MYMQTTNAME);
	init(MYMQTTNAME);
}
Basiskommunikation::Basiskommunikation()
{
	// hier muss man die Initfunktion selbst aufrufen!!!
}
void Basiskommunikation::init(const char* MYMQTTNAME)
{
	_MQTTNAME = MYMQTTNAME;
	createTopic(_TeilAllgemein, Topicallgemein);
	createTopic(_TeilSubscribe, Topicsubscribe);
}
const char* Basiskommunikation::get_MQTTName()
{
	return _MQTTNAME;
}


Basiskommunikation::~Basiskommunikation()
{
	deleteTopic(Topicallgemein);
	deleteTopic(Topicsubscribe);
}
void Basiskommunikation::createTopic(const char* topic, char*& destination)
{

	uint8_t lenT = strlen(topic);
	uint8_t lenN = strlen(_MQTTNAME);

	Serial.print(F("Cr top: ")); 	Serial.println(topic);


	destination = new  char[lenT + lenN + 1];
	strcpy(destination, _MQTTNAME);
	strcpy(destination + lenN, topic);

	debug("ready: "); 	debugln(destination);



}
void Basiskommunikation::deleteTopic(char*& topic)
{
	delete[] topic;
}

bool Basiskommunikation::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length != 1)
		return false;

	debug("Payload");
	debugln((char*)payload);

	debug("Topic");
	debugln((char*)topic);


	if (topic[0] == '?' || payload[0] == '?')
	{
		dopublish("!", (byte*)_MQTTNAME, strlen(_MQTTNAME));  //ohne den MQTT Name vorn dran  --> sonst hätte es sendmessage sein müssen
		if (_hassub){_sub->callbackismineanddo(topic, payload, length);} // dann MUSS das nächste das Sub sein!!!!!
		return true;
	}
	else if (payload[0] == 'Z')
	{

		if (strchr(topic, 'CAN') == NULL)  // sonst für CAN gedacht
		{
			sendmessage("V", GLOBALVERSION);
			return true;
		}

	}
	return false;
}
void Basiskommunikation::set_Sub(BasisInterface* sub)
{
	_hassub = true;
	_sub = sub;
}

char* Basiskommunikation::get_allTopic()
{
	return Topicallgemein;
}
char* Basiskommunikation::get_SubsTopic()
{
	return Topicsubscribe;
}
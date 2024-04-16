#define MYDEBUG 0
#include "mydebug.h"

#pragma once



#include "globalversion.h"
#include "basisinterface.h" 
#include "PubSubClient.h" 
//#include "hilfe.h" 
extern void GLOBcallback(char* topic, byte* payload, unsigned int length);


class Basiskommunikation : public BasisInterface
{
public:
	void sendmessageWithoutName(const char* topic, const byte* msgbuffer, uint8_t len);
	Basiskommunikation(const char* MYMQTTNAME);
	Basiskommunikation();
	void init(const char* MYMQTTNAME);
	virtual void subscribeextra(const char* topic) = 0;   // f�r den CAN geacht hier leer, in EthernetMQTT �berladen mit Inhalt. Alle anderen Kommuunikationsarten brauchen das nicht
	~Basiskommunikation();
	const char* get_MQTTName();
 

	void sendmessage(const char* topic, const char* msgbuffer);  //Strings verschicken  -- das ist zu 99 % im code vervbaut
	void sendmessage(const char* topic, const byte* msgbuffer, uint8_t len);  //Bin�rzeugs verschicken
	void sendmessage(const char* topic, double  wert); //Kommazahlen verschicken
	void sendmessage(const char* topic, int32_t  wert);

	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);

 void setCallback(MQTT_CALLBACK_SIGNATURE) ;
	void update() ;

	char* get_allTopic();
	char* get_SubsTopic();
	void set_Sub(BasisInterface* sub);
protected:

	virtual void dopublish(const char* topic, const byte* msgbuffer, uint8_t len) = 0;
	const char* _MQTTNAME;

private:
	char zahl[30] = "";
	uint8_t ziffernzaehler = 0;

	MQTT_CALLBACK_SIGNATURE;  // Callbacksignatur

	bool _hassub = false;
	BasisInterface* _sub;
	const char* _TeilAllgemein = "/allg";  //vorn wird noch der MQTT Name angehangen
	const char* _TeilSubscribe = "/todo/#";  //vorn wird noch der MQTT Name angehangen
	char* Topicallgemein;
	char* Topicsubscribe;

	void createTopic(const char* topic, char*& destination);
	void deleteTopic(char*& topic);



};






#include "ethernetmqtt.h"


//##############################################################################
// ESP_Wifi_MQTT  für ESP mit WIFI
 

void EthernetMQTT::dopublish(const char* topic, const byte* msgbuffer, uint8_t len)
{
	debugln(F("doP Eth"));
	debugln(topic);
	debugln((char*)msgbuffer);
	MqttClient.publish(topic, msgbuffer,len);
}

void EthernetMQTT::subscribeextra(const char* topic)
{
	Serial.print(F("E SubsFTopic "));
	Serial.println(topic);
	MqttClient.subscribe(topic); //Identify Controller --> Y muss gesendet werden // Antwort in !
}
EthernetMQTT::EthernetMQTT(const char* server, const char* MYMQTTNAME, const char* MYMQTTUS, const char* MYMQTTPW, Client& client, uint16_t brokerport)
	:
	Basiskommunikation(MYMQTTNAME),
	MqttClient(server, brokerport, client)
{
	this->_client = &client;
	_MQTTPW = MYMQTTPW;
	_MQTTUS = MYMQTTUS;

}




void EthernetMQTT::setCallback(MQTT_CALLBACK_SIGNATURE)
{
	Serial.println("Ethernetcallback");
	MqttClient.setCallback(callback);
}

void EthernetMQTT::reconnect() {
	while ((!MqttClient.connected()) || (!_client->connected()))
	{
		Serial.println(F("Att MQTT con..."));
		if (MqttClient.connect(_MQTTNAME, _MQTTUS, _MQTTPW, get_allTopic(), 1, 0, "dead")) {                    // Check the MQTT broker connectivity
			dopublish(get_allTopic(), (byte*)"con",strlen("con"));
			debug("SUBSCRIBE:");
			debugln(get_SubsTopic());
			MqttClient.subscribe(get_SubsTopic()); 
			MqttClient.subscribe("?"); //Identify Controller --> Y muss gesendet werden // Antwort in !
		}
		else {

			 

			if (!_client->connected()) {
				debugln("client nicht connected.");
			}


			Serial.print(F("failed, rc="));
			Serial.print(MqttClient.state());
			Serial.println(F(" again in 1 s"));
			delay(1000);
			// nach x Sekunden schlägt wathdog an (oder sollte zumindestens)
		}
	}
}

void EthernetMQTT::update() {
	Basiskommunikation::update();  // Serielle Schnittstelle
	static bool once = true; if (once) { setCallback(GLOBcallback); once = false; }

	if (!MqttClient.connected()) {
		Serial.println(F("MQTTC not conn"));
		reconnect();
	}

	if (!_client->connected()) {
		Serial.println(F("Netzwerk unterbrochen"));
		reconnect();
	}

	MqttClient.loop();
}



















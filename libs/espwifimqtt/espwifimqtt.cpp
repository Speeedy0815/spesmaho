
#include "espwifimqtt.h"


//##############################################################################
// ESP_Wifi_MQTT  für ESP mit WIFI
 
 
ESP_Wifi_MQTT::ESP_Wifi_MQTT(const char* server,const char * MYMQTTNAME,const char * MYMQTTUS,const char * MYMQTTPW,const char* SSID,const char* WIFIPW ,uint16_t brokerport)
:
EthernetMQTT( server,MYMQTTNAME,MYMQTTUS, MYMQTTPW,ethClient,brokerport)
{
	init(SSID,WIFIPW);
}
ESP_Wifi_MQTT::ESP_Wifi_MQTT(const char* server,const char * MYMQTTNAME,const char * MYMQTTUS,const char * MYMQTTPW,const char* SSID,const char* WIFIPW )
:
EthernetMQTT( server,MYMQTTNAME,MYMQTTUS, MYMQTTPW,ethClient,1883)
{
	init(SSID,WIFIPW);
}


void ESP_Wifi_MQTT::init(const char* SSID,const char* WIFIPW )
{
	WiFi.mode(WIFI_STA);
	Serial.println("Enable Wifi!");
	WiFi.begin(SSID, WIFIPW);
	Serial.println("Searching for Wifi!");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.print("My IP address: ");
	Serial.println(WiFi.localIP());
	
	this -> update(); //einmalig aufrufen zum verbindenS
}

 







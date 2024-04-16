#include "myotaupdater.h"



void OTAUpdater::benachrichtigealles(char* inhalt)
{
	//if (this->_display) { this->_display->setupdatestring(inhalt); }  //wenn Display angeschlossen ist  --> umschalten
	//if (this->_display) { this->_display->aktualisiereupdateinhalt(); }//wenn Display angeschlossen ist  --> umschalten + einmalig Bild aufbauen
	_newcontent = true;
	strcpy(updatestring, inhalt);
	_mqtt->sendmessage(_MYMQTTANTWORTADR, inhalt);
}


void OTAUpdater::setupmyOTA(const char* nameprefix, const char* ssid, const char* password) {
	// Configure the hostname
	uint16_t maxlen = strlen(nameprefix) + 7;
	char* fullhostname = new char[maxlen];

	snprintf(fullhostname, maxlen, "OTA_%s", nameprefix);
	ArduinoOTA.setHostname(fullhostname);
	delete[] fullhostname;

	// Configure and start the WiFi station
	if (!WiFi.isConnected())
	{
		benachrichtigealles("Connecting Wifi");
		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid, password);

		// Wait for connection
		while (WiFi.waitForConnectResult() != WL_CONNECTED) {

			benachrichtigealles("Wifi Problem");
			delay(5000);
			ESP.restart();
		}
	}


	// Port defaults to 3232
	// ArduinoOTA.setPort(3232); // Use 8266 port if you are working in Sloeber IDE, it is fixed there and not adjustable

	// No authentication by default
	// ArduinoOTA.setPassword("admin");

	// Password can be set with it's md5 value as well
	// MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
	// ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

	ArduinoOTA.onStart([this]() {
		//NOTE: make .detach() here for all functions called by Ticker.h library - not to interrupt transfer process in any way.

		if (ArduinoOTA.getCommand() == U_FLASH)
			benachrichtigealles("Start updating sketch");
		else // U_SPIFFS
			benachrichtigealles("Start updating filesystem");


		// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()

		});

	ArduinoOTA.onEnd([this]() {
		benachrichtigealles("End");
		delay(1000);
		ESP.restart();

		});

	ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
		//Serial.printf();
		static uint8_t progressvorher = 0; //0-100
		uint8_t prozent = (progress / (total / 100));


		if ((0 == (prozent % 2)) && (progressvorher != prozent))
		{
			char str[30];
			sprintf(str, "Progress: %u%%", prozent);
			benachrichtigealles(str);
		}
		progressvorher = prozent;
		});

	ArduinoOTA.onError([this](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) benachrichtigealles("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) benachrichtigealles("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) benachrichtigealles("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) benachrichtigealles("Receive Failed");
		else if (error == OTA_END_ERROR) benachrichtigealles("End Failed");
		});

	ArduinoOTA.begin();

	benachrichtigealles("OTA Init");

	sendeIPuName();
	


}


void OTAUpdater::sendeIPuName()
{
	char buf[50];
	IPAddress ip = WiFi.localIP();
	sprintf(buf, "IP: %u.%u.%u.%u|%s", ip[0], ip[1], ip[2], ip[3], _firmwarename);
	benachrichtigealles(buf);
}

OTAUpdater::OTAUpdater(Basiskommunikation* mqtt, const char* firmwarename)
{
	_mqtt = mqtt;
	_firmwarename = firmwarename;
}



void OTAUpdater::update()
{


	if (otaisstarted)
	{
		if (TIM.update_darfich())
		{
			_mqtt->sendmessage(_MYMQTTANTWORTADR, "ena");
		}
		ArduinoOTA.handle();
	}
	else
	{
		if (this->_updatew)
		{
			if (this->_updatew->isupdatewunsch())
			{
				starteupdate();
			}
		}
	}
}
bool  OTAUpdater::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{

	if (length != 3)
		return false;
	if (payload[0] != 'U')
		return false;
	if (payload[1] != 'P')
		return false;
	if (payload[2] != 'D')
		return false;
	if (otaisstarted)
	{
		benachrichtigealles("Runs");
		sendeIPuName();
		return false;
	}

	starteupdate();



	return true;
}
void OTAUpdater::starteupdate()
{
	_updatemode = true;
	benachrichtigealles("Start");
	setupmyOTA(_mqtt->get_MQTTName(), myWIFISSID, myWIFIPW);
	TIM.start(sendezeit, false);
	otaisstarted = true;
}


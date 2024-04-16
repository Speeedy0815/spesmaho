#pragma once


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "basiskommunikation.h"

#include "settingsmqttsecret.h"

#include "basisinterface.h"

#include "hilfe.h"



#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif

#include <WiFiUdp.h>
#include <ArduinoOTA.h>
 
#include "myupdatewuenscher.h"
#include "mytimer.h"




class OTAUpdater : public BasisInterface
{
private:
	const char* _firmwarename;
	bool _newcontent = false;
	char updatestring[50] = "not startet";
	bool _updatemode = false;

	myTimer TIM;
	uint32_t sendezeit = 30000;
	updatewuenscher* _updatew;
	const char* _MYMQTTANTWORTADR = "Upd";
	Basiskommunikation* _mqtt;
	bool otaisstarted = false;
	void setupmyOTA(const char* nameprefix, const char* ssid, const char* password);

	void benachrichtigealles(char* inhalt);
	void sendeIPuName();
public:
	void starteupdate();
	void setupdateW(updatewuenscher* updatew) { _updatew = updatew; };

	OTAUpdater(Basiskommunikation* mqtt,const char* firmwarename);
	void update();
	bool isnewcontent() { return _newcontent; };
	char* getInhalt() { _newcontent = false;  return updatestring; };
	bool isupdatemode() { return _updatemode; };
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

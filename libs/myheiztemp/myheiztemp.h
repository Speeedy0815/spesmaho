// myonewire.h
#pragma once


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

	 // Achtung!!! Hardware und Speicher sind nicht abstrahiert. Das Modul läuft nur auf dem ESP32!!!!!


#include "basiskommunikation.h"
#include "basisinterface.h"
#include "myhdc108.h" 
#include "myBasisHW.h"

#include "FS.h"
#include "SPIFFS.h"
 




class HeizTemp : public BasisInterface
{
public:
	HeizTemp(Basiskommunikation* mqtt, myBasisHW* Hardware, MyHDC108* temperatursensor, uint8_t fussbodenpin, uint8_t heizkoerperpin);
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
	double getSolltemperatur() { return solltemperatur; }
	double getIsttemperatur() { return isttemperatur; }
	double getisWindowopen() { return windowwopen; }
	int32_t getisNachtabsenkung();
	bool getHeizFB() { return heizefussboden; };

	bool getHeizHK() { return heizeHeizk; };
	void setRelativeSollTemp(double value, bool save);
	void saveSettings();
	void loadSettings();
private:
	myTimer Regeltimertimer;
	myTimer Sendetimer;
	Basiskommunikation* _mqtt;
	uint8_t _fussbodenpin;
	uint8_t _heizkoerperpin;
	MyHDC108* _temperatursensor;

	double solltemperatur = 19.0;
	double isttemperatur = 19.0;

	double diffussboden = 1.5;
	double difHeizk = 3;

	double nachtabsenkungstemperatur = 0;


	bool heizefussboden = false;
	bool heizeHeizk = false;
	bool windowwopen = false;


	const uint32_t Regelzeit = 1000;
	const uint32_t Sendezeit = 120000;

	void sendSolltemp();
	void sendHeizungStatus(bool force);
	void setAbsSollTemp(double value, bool save);
	void sendFenster();
	void sendnachtabsenk();
	void sendH1();
	void sendH2();
	void sendnachtabsenkTemp();




	myBasisHW* _hardware;

	const char* _MYMQTTANTSoll = "Hz/S";
	const char* _MYMQTTANTFB = "Hz/FB";//FB Temperatur
	const char* _MYMQTTANTHK = "Hz/HK";//HK Temperatur

	const char* _MYMQTTANTH1 = "Hz/H1";//Hysterese FB
	const char* _MYMQTTANTH2 = "Hz/H2";//Hysterese HK
	const char* _MYMQTTANTN = "Hz/N";//Nachtabsenkung generell (-1/0-1)
	const char* _MYMQTTANTn = "Hz/n";//Nachtabsenkungstemperatur
	const char* _MYMQTTANTF = "Hz/F";//Fenster
};

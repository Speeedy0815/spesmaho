#pragma once

#define MYDEBUG 0
#include "mydebug.h"



#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "basiskommunikation.h"

#include "myhdc108.h"
#include <XPT2046_Touchscreen.h> 
#include "basisinterface.h"

#include "myheiztemp.h"

#include <SPI.h>



#include "FS.h"
#include "SPIFFS.h"
#include <TFT_eSPI.h> // Hardware-specific library,
#include "mytimer.h"
#include "FS.h"
#include "SPIFFS.h"
#include "myEncoderESP.h"
#include "arduinowatchdog.h"
#include "myUhrBasis.h"

#include "myotaupdater.h"
#include "myupdatewuenscher.h"
#include "globalversion.h"
#include "mydisplaybeleuchter.h"


#define TFT_GREY 0x5AEB
#define _sclk     25  //GPIO25
//#define _mosi     32  //GPIO32 
//#define _miso     39  //GPIO39
#define TOUCH_CS  33  //GPIO33







class MyDisp : public BasisInterface, public updatewuenscher
{
private:

	static const uint32_t resetkaestelfarbe = TFT_BLACK;

	
	OTAUpdater* _OTAupdater = NULL;
	uint8_t updatewunschzaehler = 0;
	uint8_t resetwunschzaehler = 0;
	bool updatemode = false;
	myEncoderESP* _Encoder;

	HeizTemp* _heizReglung;
	myTimer Hintergrundbeleuchtungstimer;
	uint16_t Hintergrundbeleuchtungstimerzeit = 10;


	 
	

	XPT2046_Touchscreen ts;  // Param 2 - Touch IRQ Pin - interrupt enabled polling
	TFT_eSPI tft;       // Invoke custom library


	bool eingeschalteteButtons[8] = { false };

	double  aussentemperatur = 999;
	byte warntext[50];
	uint8_t mywarntextfarbe = 0;  //0 schwarz (unsichtbar) 1:wei� 2:rot 3:gr�n 4:blau 5:grau

	uint8_t lautstaerke = 0; //0-100
	int8_t mondph = -1; //0 Neumond,1-4 zunehmend 5 Vollmond,6-9 abnehmend   -1 disabled
	uint8_t wetterbildnummer = 0; //0-100
	const  uint8_t lautstaerkeMAX = 100;
	void aktualisiereDisplay();
	void aktualisiereMond();//  
	void aktualisiereDatumundUhr();

	void aktualisiereVolumeDreieck();

	void aktualisiereAussentemperatur();




	void aktualisiereWetter();
	 
	void ZeichneButton(uint8_t nr, bool enabled, uint8_t Farbe, const char* bildname, bool bildname_ist_text);
	void aktualisiereWarnung(); //das wird nicht mit im aktualisiere Display aufgerufen  --> nur wenn vom Server da was kommt (da das �berpr�fen, ob sich was �ndert schwierig ist)

	void schalteDisplayan();
	bool sollDislayan() { return ((zeitbisausgeschaltetwird > 0) || (true == updatemode)); };  // wenn Update --> Bildschirm nicht mehr ausschalten



	//myTimer Hintergrundbeleuchtungstimer;
	//uint16_t Hintergrundbeleuchtungstimerzeit = 10; //ms  --> achtung nicht �ndern, der Timer wird doppelt verwendet!!!!!

	uint32_t zeitbisausgeschaltetwird = 0; //in 10msTaktung  -->800 sind 88s  siehe auch:stdzeidausschalten
	const uint32_t stdzeidausschalten = 800;  //8Sekunden

	myTimer ButtonSendelimiter;
	uint16_t sendelimitButton = 200; //ms
	myTimer Bildaktualisierungstimertimer;
	uint16_t limitaktualisierung = 200; //ms

	uint16_t _breite = 240;
	uint16_t _hoehe = 320;

	uint32_t getfarbe(uint8_t farbcode);
	int8_t  gibemepressedButton(TS_Point p);
	myUhrBasis* _Uhrzeit;

	void ladeButtoneinstellung(uint8_t nr, const char* inhalt);
	void saveButtoneinstellung(uint8_t nr, const char* inhalt);



	uint32_t read32(fs::File& f);
	uint16_t read16(fs::File& f);
	void drawBmp(const char* filename, int16_t x, int16_t y);


public:
	Basiskommunikation* _mqtt;
	const char* _MYMQTTANTWORTADR = "Disp";
	BasisBeleuchtungDisplay* _Beleuchtung;


	XPT2046_Touchscreen* getTouch() { return &ts; };
	TFT_eSPI* getTFT() { return &tft; };
	void setupdatemode(bool mode);
	bool isUpdatemode() { return updatemode; }
	MyDisp(Basiskommunikation* mqtt, BasisBeleuchtungDisplay* Beleucht, myEncoderESP* Encoder, myUhrBasis* Uhrzeit, uint8_t respin, uint8_t cspin);
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
	void setOTAUpdater(OTAUpdater* ota) { Serial.println("Bind OTA on Display"); _OTAupdater = ota; };
};









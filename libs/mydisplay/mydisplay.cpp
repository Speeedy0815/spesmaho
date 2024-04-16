#include "mydisplay.h"

void MyDisp::setupdatemode(bool mode)
{
	if (true == mode)
	{
		updatewunschzaehler = 0;
		tft.fillScreen(TFT_BLACK);
	}
	updatemode = mode;
}


MyDisp::MyDisp(Basiskommunikation* mqtt, BasisBeleuchtungDisplay* Beleucht, myEncoderESP* Encoder, myUhrBasis* Uhrzeit, uint8_t respin, uint8_t cspin) :ts(TOUCH_CS)
{
	debugln("start Display");
	_Uhrzeit = Uhrzeit;
	_Beleuchtung = Beleucht;
	_Encoder = Encoder;
	_mqtt = mqtt;




	if (!SPIFFS.begin(true)) {
		Serial.println("SPIFFS Mount Failed");
		return;
	}

	debugln("start Touch");
	ts.begin();
	ts.setRotation(1);
	debugln("start TFT");



	tft.init(respin, cspin);
	tft.setRotation(0);



	setup_t user; // The library defines the type "setup_t" as a struct
	tft.getSetup(user); //
	debug("CS TFT: ");
	debugln(user.pin_tft_cs);
	debug("RES TFT: ");
	debugln(user.pin_tft_rst);
	//Startstrings mal leeren nullen


	warntext[0] = 0;

	tft.fillScreen(TFT_BLACK);

	debugln("zeichne Display");
	aktualisiereDisplay();

	ButtonSendelimiter.start(sendelimitButton, true);
	Bildaktualisierungstimertimer.start(limitaktualisierung, true);



	Hintergrundbeleuchtungstimer.start(Hintergrundbeleuchtungstimerzeit, true);


	debugln("Display an");
	schalteDisplayan();


	Serial.println(F("Loading settings Display Buttons"));
	for (uint8_t i = 0; i < 8; i++)
	{

		char filename[] = "/but/x";
		filename[strlen(filename) - 1] = i + '0';
		Serial.println(filename);
		File file;
		char buffer[30] = { 0 };

		uint8_t len = 0;

		//##################################################
		file = SPIFFS.open(filename);
		if (!file) {
			Serial.println(F("File Open crashed"));
		}
		while (file.available())
		{
			buffer[len] = file.read();
			//Serial.print(buffer[len]);
			len++;
		}
		buffer[len] = 0;
		file.close();

		if (len > 0)
		{
			ladeButtoneinstellung(i, buffer);
		}
		else
		{
			Serial.println(F("kein Settings für den Button"));
		}

	}



}
void MyDisp::aktualisiereDatumundUhr()
{


	static int sekundevorher = 99;
	static int tagvorher = 99;


	if (_Uhrzeit) // Nullpointer
	{

		if (_Uhrzeit->istgestellt())
		{

			int tmptag = _Uhrzeit->getDay();
			if (tmptag != tagvorher)
			{
				tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
				tft.drawCentreString(_Uhrzeit->getTime("%d.%m.%Y"), 120, 300, 2);
			}
			tagvorher = tmptag;

			int tmpsekunde = _Uhrzeit->getSecond();
			if (tmpsekunde != sekundevorher)
			{
				tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
				tft.drawString(_Uhrzeit->getTime("%T"), 10, 10, 2);
			}
			sekundevorher = tmpsekunde;

		}
		else
		{
			debugln("Uhrnichtgestellt");
		}

	}


}


void MyDisp::ladeButtoneinstellung(uint8_t nr, const char* inhalt)
{
	uint8_t tmp = (inhalt[0] - '0');
	bool enabled = tmp % 2 == 1;
	bool bildname_is_text = (tmp / 2) % 2 == 1;

	ZeichneButton(nr, enabled, inhalt[1] - '0', inhalt + 2, bildname_is_text);
}
void MyDisp::saveButtoneinstellung(uint8_t nr, const char* inhalt)
{
	debugln("Saving settings Button");
	char filename[] = "/but/x";
	filename[strlen(filename) - 1] = nr + '0';
	debugln(filename);
	File file;


	//##################################################

	file = SPIFFS.open(filename, FILE_WRITE);
	if (file.print(inhalt)) {
		debugln("- file written");
	}
	else {
		Serial.println("- write failed");
	}
	file.close();

}
void MyDisp::aktualisiereAussentemperatur()
{
	if (this->isUpdatemode())
		return;
	static double aussentempvorher = 0;
	if (this->aussentemperatur == aussentempvorher)
		return;
	aussentempvorher = this->aussentemperatur;
	char buffer[30];
	uint8_t len = 0;
	//aussentemperatur
	if (this->aussentemperatur < 998)
	{
		tft.fillRect(180, 0, 60, 27, resetkaestelfarbe);  //vorherigen Mist löschen
		tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
		dtostrf(this->aussentemperatur, 0, 1, buffer);
		len = strlen(buffer);
		buffer[len] = '`';
		buffer[len + 1] = 'C';
		buffer[len + 2] = 0;


		tft.drawRightString(buffer, 230, 10, 2);

	}
}



void MyDisp::aktualisiereWarnung()
{


	//Warnung
	tft.fillRect(0, 263, 240, 15, resetkaestelfarbe);  //vorherigen Mist löschen
	tft.setTextColor(getfarbe(mywarntextfarbe), TFT_BLACK);  // Adding a background colour erases previous text automatically
	tft.drawString((char*)warntext, 28, 262, 2);
	tft.fillCircle(15, 271, 7, getfarbe(mywarntextfarbe));

}

void MyDisp::aktualisiereDisplay()
{


	aktualisiereDatumundUhr();
}

void MyDisp::ZeichneButton(uint8_t nr, bool enabled, uint8_t Farbe, const char* bildname, bool bildname_ist_text)
{

	uint16_t hoehe1 = 180;
	uint16_t hoehe2 = 235;

	uint16_t hoehe;
	eingeschalteteButtons[nr] = enabled;  // ein und ausschalten des Touches
	if (nr >= 4) { hoehe = hoehe2; }
	else { hoehe = hoehe1; }

	//Buttons

	uint8_t anzahl = 4;
	uint16_t _breiteButton = (_breite / anzahl) - 10;
	uint16_t _hoeheButton = 50;

	uint8_t i = nr % anzahl;


	uint16_t mittelpunkt_button = ((_breite / anzahl) / 2) + i * (_breite / anzahl);
	if (enabled)
	{

		tft.fillRoundRect(mittelpunkt_button - (_breiteButton / 2), hoehe - (_hoeheButton / 2), _breiteButton, _hoeheButton, 10, getfarbe(Farbe));
		_breiteButton = 40;
		_hoeheButton = 40;
		tft.fillRoundRect(mittelpunkt_button - (_breiteButton / 2), hoehe - (_hoeheButton / 2), _breiteButton, _hoeheButton, 10, TFT_WHITE);


		if (!bildname_ist_text)
		{
			char buf[50] = "/pic/";
			strcpy(buf + strlen(buf), bildname);
			strcpy(buf + strlen(buf), ".bmp");
			debugln(buf);
			drawBmp(buf, mittelpunkt_button - 15, hoehe - 15);
		}
		else  // Textvariante
		{
			debugln("zeichne");
			debugln(bildname);
			tft.setTextColor(TFT_BLACK, TFT_WHITE);  // Adding a background colour erases previous text automatically
			tft.drawCentreString(bildname, mittelpunkt_button, hoehe - 8, 2);
		}

	}
	else  //einfach abschalten
	{
		tft.fillRoundRect(mittelpunkt_button - (_breiteButton / 2), hoehe - (_hoeheButton / 2), _breiteButton, _hoeheButton, 10, TFT_BLACK);
	}




}
void MyDisp::aktualisiereWetter()
{

	static uint8_t wetterbildnummervorher = 99;
	if (this->wetterbildnummer == 0)  //init --> leer
		return;
	if (this->wetterbildnummer == wetterbildnummervorher)
		return;
	wetterbildnummervorher = this->wetterbildnummer;




	tft.fillRect(200, 280, 40, 40, resetkaestelfarbe);  //vorherigen Mist löschen

	char buf[] = "/wet/xx.bmp";
	debugln(buf);
	buf[6] = this->wetterbildnummer % 10 + '0';
	buf[5] = this->wetterbildnummer / 10 + '0';
	debugln(buf);
	drawBmp(buf, 200, 280);
}
void MyDisp::aktualisiereMond()
{
	static int8_t mondphvorher = 0; //0-100
	if (this->mondph == mondphvorher)
		return;
	mondphvorher = this->mondph;


	//Mond
	if (mondph >= 0)
	{
		tft.fillRect(0, 290, 70, 30, resetkaestelfarbe);  //vorherigen Mist löschen
		uint16_t mondradius = 10;
		uint16_t mondY = 30;
		uint16_t mondX = 305;
		const uint8_t mondStatianz = 10;

		tft.drawCircle(mondY, mondX, mondradius, TFT_WHITE);
		if (mondph != 5)
		{
			tft.fillCircle(mondY, mondX, mondradius, TFT_WHITE);
		}
		if ((mondph != 0) && (mondph != 5))
		{
			tft.fillCircle(mondY - mondradius * 2 + ((mondph * mondradius * 4) / mondStatianz), mondX, 10, TFT_BLACK);
		}
	}
}
void MyDisp::aktualisiereVolumeDreieck()
{
	static uint8_t lautstaerkevorher = 0; //0-100
	if (this->lautstaerke == lautstaerkevorher)
		return;


	//Dreieck Lautstärke
	tft.fillRect(0, 110, 50, 30, resetkaestelfarbe);  //vorherigen Mist löschen


	uint16_t Dreieckunten = 150;
	uint16_t Dreieckoben = 120;
	uint16_t Dreiecklinks = 10;
	uint16_t Dreieckrechts = 230;

	char buffer[30];
	uint8_t len;
	itoa(lautstaerke, buffer, 10);
	len = strlen(buffer);
	buffer[len] = '%';
	buffer[len + 1] = 0;
	tft.setTextColor(TFT_WHITE, TFT_BLACK);

	tft.drawRightString(buffer, 35, Dreieckoben, 2);
	tft.fillTriangle(Dreiecklinks, Dreieckunten, Dreieckrechts, Dreieckunten, Dreieckrechts, Dreieckoben, TFT_BLACK);
	tft.drawTriangle(Dreiecklinks, Dreieckunten, Dreieckrechts, Dreieckunten, Dreieckrechts, Dreieckoben, TFT_WHITE);

	uint16_t klDreieckrechts = (lautstaerke * (Dreieckrechts - Dreiecklinks) / lautstaerkeMAX) + Dreiecklinks;
	uint16_t klDreieckoben = Dreieckunten - (lautstaerke * (Dreieckunten - Dreieckoben) / lautstaerkeMAX);

	tft.fillTriangle(Dreiecklinks, Dreieckunten, klDreieckrechts, Dreieckunten, klDreieckrechts, klDreieckoben, TFT_WHITE);
	lautstaerkevorher = this->lautstaerke;
}
void MyDisp::schalteDisplayan()
{
	zeitbisausgeschaltetwird = stdzeidausschalten;
}


void MyDisp::update()
{
	if (sollDislayan())
	{
		_Beleuchtung->schalteAn();
	}
	else
	{
		_Beleuchtung->schalteAus();
	}
	_Beleuchtung->update();

	if (Hintergrundbeleuchtungstimer.update_darfich())
	{
		if (zeitbisausgeschaltetwird > 0) { zeitbisausgeschaltetwird--; }
	}
	if (Bildaktualisierungstimertimer.update_darfich())
	{
		if (false == updatemode)
		{
			aktualisiereDisplay();  //aktualisier den INHALT des Displays
		}
		else
		{
			if (_OTAupdater)
			{
				if (_OTAupdater->isnewcontent())
				{
					debugln("Update New Content: ");
					debugln(_OTAupdater->getInhalt());

					tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
					tft.drawCentreString("Update", 120, 10, 4);
					tft.fillRect(0, 45, 240, 25, resetkaestelfarbe);  //vorherigen Mist löschen
					tft.drawString(_OTAupdater->getInhalt(), 10, 50, 2);

					tft.drawString("Version:", 10, 90, 2);
					tft.drawFloat(GLOBALVERSION, 2, 80, 90, 2);
					// hier könnten theoretisch noch mehr Informationen kommen --> falls gebraucht werden
				}
			}
		}
	}


	if (ts.touched())
	{
		TS_Point p = ts.getPoint();
		if (p.x != 0 && p.y != 0)//nicht angeschlossen
		{
			schalteDisplayan();
			debug("Punkte: ");debug(p.y);debug(" | ");debugln(p.x);
			if ((ButtonSendelimiter.update_darfich()) && (_Beleuchtung->istDisplayAn()))
			{

				if (p.y < 700 && p.x > 3500 && false == updatemode)
				{
					if (updatewunschzaehler < 255) { updatewunschzaehler++; if (updatewunschzaehler > 15) { updatewunschzaehler = 0; updatewuenscher::wuenscheupdate(); } }
				}
				if (p.y > 3200 && p.x < 700)
				{
					if (resetwunschzaehler < 255) { resetwunschzaehler++; if (resetwunschzaehler > 15) { resetwunschzaehler = 0; globalResetme(); } }
				}


				if (false == updatemode)
				{
					int8_t buttonergebnis = gibemepressedButton(p);
					if (buttonergebnis >= 0)
					{
						_mqtt->sendmessage(_MYMQTTANTWORTADR, buttonergebnis);
					}

					ButtonSendelimiter.start(sendelimitButton, false);//ResetTime

				}

			}
		}
	}
	else
	{
		if (updatewunschzaehler > 0) { updatewunschzaehler--; }
		if (resetwunschzaehler > 0) { resetwunschzaehler--; }
	}


	if (_OTAupdater)
	{
		if (!updatemode)
		{
			if (_OTAupdater->isupdatemode())
			{
				setupdatemode(true);
			}
		}
	}
}

int8_t  MyDisp::gibemepressedButton(TS_Point p)
{
	int8_t fertig = -1;
	int8_t reihe = -1;
	int8_t spalte = -1;


	if ((p.x > 1000) && (p.x < 1600))
	{
		reihe = 1;

	}
	if ((p.x > 1700) && (p.x < 2200))
	{
		reihe = 0;

	}
	if ((reihe == 1) || (reihe == 0))
	{

		if ((p.y > 200) && (p.y < 1000))
		{
			spalte = 0;

		}
		if ((p.y > 1200) && (p.y < 2000))
		{
			spalte = 1;

		}
		if ((p.y > 2200) && (p.y < 3000))
		{
			spalte = 2;

		}
		if ((p.y > 3200) && (p.y < 4000))
		{
			spalte = 3;

		}
	}


	if ((reihe >= 0) && (spalte >= 0))
	{
		fertig = reihe * 4 + spalte;
		if (eingeschalteteButtons[fertig] == false) { return(-1); }  // disableter Button
	}


	return (fertig);
}
bool  MyDisp::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length < 2)
		return false;

	if (payload[0] != 'D')
		return false;
	if (this->isUpdatemode())
		return true;  //keine weitere Befehle für das Display entgegennehmen




	if (payload[1] == 'T') //Aussentemperatur
	{
		this->aussentemperatur = atof((char*)payload + 2);
		aktualisiereAussentemperatur();
		return true;
	}
	if (payload[1] == 'L') //Lautstärke
	{
		lautstaerke = atoi((char*)payload + 2);
		aktualisiereVolumeDreieck();
		schalteDisplayan();

		return true;
	}
	if (payload[1] == 'M') // Mond
	{
		mondph = atoi((char*)payload + 2);
		aktualisiereMond();
		return true;
	}
	if (payload[1] == 'w' || payload[1] == 'W') //Warnung
	{
		memcpy(warntext, payload + 3, sizeof(warntext));

		debugln((char)payload[2]);
		mywarntextfarbe = ((char)(payload[2])) - '0';
		debugln(mywarntextfarbe);

		warntext[sizeof(warntext) - 1] = 0;// Sting abschließen, falls keiner gekommen sein sollte
		aktualisiereWarnung();//auf dem Display
		if (payload[1] == 'W') //Warnung mit Display an
		{
			schalteDisplayan();
		}
		return true;
	}



	if (payload[1] == 'E') //wEtter
	{
		wetterbildnummer = atoi((char*)payload + 2);
		aktualisiereWetter();
		return true;
	}
	if (payload[1] == 'B')
	{
		uint8_t nr = payload[2] - '0';
		bool remanent = payload[3] - '0';
		//bool enabled = payload[4] - '0';
		//uint8_t farbe = payload[5] - '0';

		ladeButtoneinstellung(nr, (char*)payload + 4);

		if (remanent)
		{
			saveButtoneinstellung(nr, (char*)payload + 4);
		}
		return true;
	}
	if (payload[1] == 'A')
	{
		schalteDisplayan();
		return true;
	}

	return false;

	
}
uint32_t MyDisp::getfarbe(uint8_t farbcode)
{
	switch (farbcode)
	{
	case 0:
	{
		return (TFT_BLACK);
		break;
	}
	case 1:
	{
		return (TFT_WHITE);
		break;
	}
	case 2:
	{
		return (TFT_RED);
		break;
	}
	case 3:
	{
		return (TFT_GREEN);
		break;
	}
	case 4:
	{
		return (TFT_BLUE);
		break;
	}
	case 5:
	{
		return (TFT_PINK);
		break;
	}
	case 6:
	{
		return (TFT_DARKGREEN);
		break;
	}
	}

	//
	//#define TFT_BLACK       0x0000      /*   0,   0,   0 */
	//#define TFT_NAVY        0x000F      /*   0,   0, 128 */
	//#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
	//#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
	//#define TFT_MAROON      0x7800      /* 128,   0,   0 */
	//#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
	//#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
	//#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
	//#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
	//#define TFT_BLUE        0x001F      /*   0,   0, 255 */
	//#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
	//#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
	//#define TFT_RED         0xF800      /* 255,   0,   0 */
	//#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
	//#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
	//#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
	//#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
	//#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
	//#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
	//#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
	//#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
	//#define TFT_SILVER      0xC618      /* 192, 192, 192 */
	//#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
	//#define TFT_VIOLET      0x915C      /* 180,  46, 226 */
	return (TFT_WHITE);
}
// Bodmer's BMP image rendering function

void MyDisp::drawBmp(const char* filename, int16_t x, int16_t y) {


	_Encoder->deaktiviereInterrupts();



	if ((x >= tft.width()) || (y >= tft.height())) return;

	fs::File bmpFS;

	// Open requested file on SD card
	bmpFS = SPIFFS.open(filename, "r");

	if (!bmpFS)
	{
		Serial.print("File not found");
		return;
	}

	uint32_t seekOffset;
	uint16_t w, h, row;
	uint8_t  r, g, b;

	//	uint32_t startTime = millis();

	if (read16(bmpFS) == 0x4D42)
	{
		read32(bmpFS);
		read32(bmpFS);
		seekOffset = read32(bmpFS);
		read32(bmpFS);
		w = read32(bmpFS);
		h = read32(bmpFS);

		if ((read16(bmpFS) == 1) && (read16(bmpFS) == 24) && (read32(bmpFS) == 0))
		{
			y += h - 1;

			bool oldSwapBytes = tft.getSwapBytes();
			tft.setSwapBytes(true);
			bmpFS.seek(seekOffset);

			uint16_t padding = (4 - ((w * 3) & 3)) & 3;
			uint8_t lineBuffer[w * 3 + padding];

			for (row = 0; row < h; row++) {

				bmpFS.read(lineBuffer, sizeof(lineBuffer));
				uint8_t* bptr = lineBuffer;
				uint16_t* tptr = (uint16_t*)lineBuffer;
				// Convert 24 to 16 bit colours
				for (uint16_t col = 0; col < w; col++)
				{
					b = *bptr++;
					g = *bptr++;
					r = *bptr++;
					*tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
				}

				// Push the pixel row to screen, pushImage will crop the line if needed
				// y is decremented as the BMP image is drawn bottom up
				tft.pushImage(x, y--, w, 1, (uint16_t*)lineBuffer);
			}
			tft.setSwapBytes(oldSwapBytes);

		}
		else Serial.println("BMP format not recognized.");
	}
	bmpFS.close();



	_Encoder->aktiviereInterrupts();
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t MyDisp::read16(fs::File& f) {
	uint16_t result;
	((uint8_t*)&result)[0] = f.read(); // LSB
	((uint8_t*)&result)[1] = f.read(); // MSB
	return result;
}

uint32_t MyDisp::read32(fs::File& f) {
	uint32_t result;
	((uint8_t*)&result)[0] = f.read(); // LSB
	((uint8_t*)&result)[1] = f.read();
	((uint8_t*)&result)[2] = f.read();
	((uint8_t*)&result)[3] = f.read(); // MSB
	return result;
}

#include "mydisplayHeizungsteil.h"

void DisplayHeizungsteil::ZeichneHeizungsButton()
{

	//+- Buttons Temperatur
	uint16_t breiteHoeheTempBu = 40;
	uint16_t XTempButton = 50 + breiteHoeheTempBu / 2;
	//	uint16_t YTempButton = 40;
	uint16_t PlatzRand = 10;
	uint16_t YButton1 = 240 - breiteHoeheTempBu / 2 - PlatzRand;
	uint16_t YButton2 = PlatzRand + breiteHoeheTempBu / 2;
	uint32_t buttonTempfarbe = TFT_DARKGREY;
	_tft->fillSmoothRoundRect(YButton1 - breiteHoeheTempBu / 2, XTempButton - breiteHoeheTempBu / 2, breiteHoeheTempBu, breiteHoeheTempBu, 8, buttonTempfarbe, TFT_WHITE);
	_tft->fillSmoothRoundRect(YButton2 - breiteHoeheTempBu / 2, XTempButton - breiteHoeheTempBu / 2, breiteHoeheTempBu, breiteHoeheTempBu, 8, buttonTempfarbe, TFT_WHITE);
	_tft->setTextColor(TFT_WHITE, buttonTempfarbe);  // Adding a background colour erases previous text automatically
	_tft->drawCentreString("+", YButton1, XTempButton - 12, 4);
	_tft->drawCentreString("-", YButton2, XTempButton - 12, 4);



}

DisplayHeizungsteil::DisplayHeizungsteil(MyDisp* Display, MyHDC108* temperatursensor, HeizTemp* heizReglung)
{
	_temperatursensor = temperatursensor;
	_heizReglung = heizReglung;
	_tft = Display->getTFT();
	_ts = Display->getTouch();
	_Display = Display;
	ZeichneHeizungsButton();
	ButtonSendelimiter.start(sendelimitButton, true);
}




void DisplayHeizungsteil::aktualisiereInnentemperatur()
{
	static double Innentempvorher = 0;
	if (_heizReglung->getIsttemperatur() == Innentempvorher)
		return;
	Innentempvorher = _heizReglung->getIsttemperatur();

	char buffer[30];
	uint8_t len = 0;
	//isttemperatur
	_tft->setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
	_tft->fillRect(80, 45, 80, 35, resetkaestelfarbe);  //vorherigen Mist löschen
	dtostrf(_heizReglung->getIsttemperatur(), 0, 1, buffer);
	len = strlen(buffer);
	buffer[len] = '`';
	buffer[len + 1] = 'C';
	buffer[len + 2] = 0;

	_tft->drawCentreString(buffer, 120, 50, 4);
}


void DisplayHeizungsteil::update()
{
	if (!_Display->isUpdatemode())
	{
		if (_temperatursensor) { _temperatursensor->update(); }

		aktualisiereLuftfeuchte();
		//Todo: nur alle paar Sekunden
		aktualisiereInnentemperatur();
		aktualisiereSolltemperatur();
		aktualisiereheizungskreise();
		handleButtonHeizung();
	}
}

void DisplayHeizungsteil::aktualisiereSolltemperatur()
{

	static double solltempvorher = 0;
	if (_heizReglung->getSolltemperatur() == solltempvorher)
		return;

	solltempvorher = _heizReglung->getSolltemperatur();
	char buffer[30];
	uint8_t len = 0;
	// Soll  temperatur
	_tft->setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
	_tft->fillRect(90, 80, 60, 20, resetkaestelfarbe);  //vorherigen Mist löschen
	dtostrf(_heizReglung->getSolltemperatur(), 0, 1, buffer);
	len = strlen(buffer);
	buffer[len] = '`';
	buffer[len + 1] = 'C';
	buffer[len + 2] = 0;
	_tft->drawCentreString(buffer, 120, 80, 2);

}

void DisplayHeizungsteil::handleButtonHeizung()
{
	if (_ts->touched())
	{
		if ((ButtonSendelimiter.update_darfich()) && (_Display->_Beleuchtung->istDisplayAn()))
		{
			TS_Point p = _ts->getPoint();
			if (p.x != 0 && p.y != 0)//nicht angeschlossen
			{
				int8_t buttonergebnis = gibemepressedButton(p);
				if (buttonergebnis >= 0)
				{

					if (buttonergebnis == 1)
					{
						_Display->_mqtt->sendmessage(_MYMQTTANTWORTADR, "+");
						_heizReglung->setRelativeSollTemp(0.5, false);

					}
					if (buttonergebnis == 0)
					{
						_Display->_mqtt->sendmessage(_MYMQTTANTWORTADR, "-");
						_heizReglung->setRelativeSollTemp(-0.5, false);

					}
					ButtonSendelimiter.start(sendelimitButton, false);//ResetTime
				}
			}
		}
	}
}
void DisplayHeizungsteil::aktualisiereheizungskreise()
{

	static bool getHeizFBvorher = false;
	static bool getHeizHKvorher = false;
	static bool getisWindowopenvorher = false;
	static int8_t getisNachtabsenkungvorher = 0;

	uint8_t radius_heizkreis = 5;

	if (_heizReglung->getHeizFB() != getHeizFBvorher) {
		getHeizFBvorher = _heizReglung->getHeizFB();
		if (_heizReglung->getHeizFB())
		{
			_tft->fillCircle(175, 60, radius_heizkreis, TFT_ORANGE);

		}
		else
		{
			_tft->fillCircle(175, 60, radius_heizkreis, TFT_BLACK);
		}
	}
	if (_heizReglung->getHeizHK() != getHeizHKvorher)
	{
		getHeizHKvorher = _heizReglung->getHeizHK();
		if (_heizReglung->getHeizHK())
		{
			_tft->fillCircle(175, 80, radius_heizkreis, TFT_ORANGE);
		}
		else
		{
			_tft->fillCircle(175, 80, radius_heizkreis, TFT_BLACK);
		}
	}
	if (_heizReglung->getisWindowopen() != getisWindowopenvorher)
	{
		getisWindowopenvorher = _heizReglung->getisWindowopen();
		if (_heizReglung->getisWindowopen())
		{
			_tft->fillCircle(65, 60, radius_heizkreis, TFT_BLUE);
		}
		else
		{
			_tft->fillCircle(65, 60, radius_heizkreis, TFT_BLACK);
		}
	}
	if (_heizReglung->getisNachtabsenkung() != getisNachtabsenkungvorher)
	{
		getisNachtabsenkungvorher = _heizReglung->getisNachtabsenkung();
		if (_heizReglung->getisNachtabsenkung() > 0)
		{
			_tft->fillCircle(65, 80, radius_heizkreis, TFT_BLUE);
		}
		else if (_heizReglung->getisNachtabsenkung() < 0)
		{
			_tft->fillCircle(65, 80, radius_heizkreis, TFT_ORANGE);
		}
		else
		{
			_tft->fillCircle(65, 80, radius_heizkreis, TFT_BLACK);
		}
	}

}

int8_t  DisplayHeizungsteil::gibemepressedButton(TS_Point p)
{
	int8_t fertig = -1;

	if ((p.x > 2800) && (p.x < 3400))
	{

		if ((p.y > 200) && (p.y < 1000))
		{
			fertig = 0;

		}
		if ((p.y > 3200) && (p.y < 4000))
		{
			fertig = 1;

		}
	}


	return (fertig);
}
void DisplayHeizungsteil::aktualisiereLuftfeuchte()
{

	static double feuchtevorher = 0;
	if (_temperatursensor->getHuminity() == feuchtevorher)
		return;
	feuchtevorher = _temperatursensor->getHuminity();
	char buffer[30];
	uint8_t len = 0;
	//Feuchte
	_tft->setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
	_tft->fillRect(80, 0, 80, 27, resetkaestelfarbe);  //vorherigen Mist löschen
	dtostrf(_temperatursensor->getHuminity(), 0, 0, buffer);
	len = strlen(buffer);
	buffer[len] = '%';
	buffer[len + 1] = 0;
	_tft->drawCentreString(buffer, 120, 10, 2);

}
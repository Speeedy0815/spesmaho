#include "mydisplayAudioteil.h"




void DisplayAudioteil::update()
{
	/*if (!_Display->isUpdatemode())
	{
		if (Balkenaktualisierer.update_darfich())
		{

		}
	}*/
}


void DisplayAudioteil::schreibeTitel(char* text)
{
	_tft->fillRect(0, 35, 240, 20, resetkaestelfarbe);  //vorherigen Mist löschen
	_tft->setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
	_tft->drawCentreString(text, 120, 37, 2);

}
void DisplayAudioteil::schreibeArtist(char* text)
{
	_tft->fillRect(0, 58, 240, 20, resetkaestelfarbe);  //vorherigen Mist löschen
	_tft->setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
	_tft->drawCentreString(text, 120, 60, 2);
}



void DisplayAudioteil::ZeichneDauer()
{
	char zeit1[10];
	char zeit2[10];

	const uint32_t breiteBalken = 136;
	uint32_t breiterel = 0;
	if (Dauergesamt != 0)
	{
		breiterel = (Dauerjetzt * breiteBalken) / Dauergesamt;
	}



	uint32_t tmpsek = Dauerjetzt % 60;
	uint32_t tmpmin = Dauerjetzt / 60;



	sprintf(zeit1, "%02d:%02d", tmpmin, tmpsek);
	debugln(zeit1);

	tmpsek = Dauergesamt % 60;
	tmpmin = Dauergesamt / 60;

	sprintf(zeit2, "%02d:%02d", tmpmin, tmpsek);
	debugln(zeit2);







	_tft->fillRect(0, 81, 50, 20, resetkaestelfarbe);  //vorherigen Mist löschen
	_tft->setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
	_tft->drawCentreString(zeit1, 25, 83, 2);


	_tft->fillRect(190, 81, 50, 20, resetkaestelfarbe);  //vorherigen Mist löschen
	_tft->setTextColor(TFT_WHITE, TFT_BLACK);  // Adding a background colour erases previous text automatically
	_tft->drawCentreString(zeit2, 215, 83, 2);


	_tft->fillRect(52, 81 + 5, breiteBalken, 10, TFT_BLACK); //altes weglöschen
	_tft->drawRect(52, 81 + 5, breiteBalken, 10, TFT_WHITE);
	_tft->fillRect(52, 81 + 5, breiterel, 10, TFT_WHITE);

}



DisplayAudioteil::DisplayAudioteil(MyDisp* Display)
{
	_tft = Display->getTFT();
	_Display = Display;
	Balkenaktualisierer.start(BalkenaktualisiererZeit, true);
}
bool DisplayAudioteil::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	debugln("Display AudioTeil erreicht");
	debugln(topic);
	debugln((char*)payload);

	if (length < 2)
		return false;
	if (payload[0] != 'D')
		return false;
	if (payload[1] != 'a') //audio (großes A vergeben)
		return false;


	debugln("duch mist durch");
	if (_Display->isUpdatemode())
		return false;//könnte auch sein dass ich mir das hier mal anders überlege


	if (payload[2] == 'T') //Titel
	{
		debugln("Titel");
		schreibeTitel((char*)payload + 3);
		return true;
	}
	if (payload[2] == 'A') //Artist
	{
		debugln("Autor");
		schreibeArtist((char*)payload + 3);
		return true;
	}
	if (payload[2] == 'D') //Dauer
	{
		debugln("Dauer");
		int32_t tmp_akt = -1;
		int32_t tmp_ges = -1;

		debugln((char*)payload + 7);
		//Reihenfolge wichtig --> Hack
		tmp_ges = atoi((char*)payload + 7);
		payload[7] = 0; //String zwischendrin beenden	
		debugln((char*)payload + 3);
		tmp_akt = atoi((char*)payload + 3);


		debugln(tmp_ges);
		debugln(tmp_akt);
		Dauergesamt = tmp_ges;
		Dauerjetzt = tmp_akt;


		ZeichneDauer();
		return true;
	}
	if (payload[2] == 'd') //Dauer ausschaltern
	{
		Dauergesamt = -1;
		Dauerjetzt = -1;
		debugln("Daueraus");
		ZeichneDauer();
		return true;
	}

	return false;
};
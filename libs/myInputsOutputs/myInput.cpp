//Hinweis hier hat chatgpt noch weitere hinweise gegeben:
//BUTT.attachClick([this](){ this->mysendmessage("C"); });





#include "myInput.h"


myInput::myInput(Basiskommunikation* mqtt, uint8_t pinnummer, bool activateLongPress)
{
	this->_mqtt = mqtt;
	this->pinnummer = pinnummer;

	DB20.interval(20);

	//mal alles aktivieren
	BUTT.attachClick();
	BUTT.attachDoubleClick();
	BUTT.attachMultiClick();
	BUTT.attachLongPressStart();
	BUTT.attachLongPressStop();
	if (activateLongPress)
	{
		BUTT.attachDuringLongPress();
	}
}



void myInput::begin(bool currentState)
{
	DB20.begin(currentState);
}

void myInput::mysendmessage(const char* inhalt)
{
	debug("sendmymessage ");
	debugln(inhalt);
	
	
	GLOBmysendmessageWithAddr(_mqtt, inhalt, _topic, this->pinnummer);
}


void myInput::sendestate()
{
	bool state = DB20.read();
	if (state)
	{
		mysendmessage("H");
	}
	else
	{
		mysendmessage("L");
	}
}

bool  myInput::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (payload[0] != 'E')
		return false;
	if ((length != 1) && (length != 3)) // laenge muss eintweder genau 1 sein oder genau 3
		return false;

	if (length == 1) // alle abfragen
	{
		sendestate();
		return false; // Achtung, auch wenns passt falsch zurückliefern, denn das nächste Modul kann auch noch passen (mehrere IO Module)
	}
	else // nur genau den einen abfragen
	{
		if (length >= 3) {
			uint8_t Eingangssnummer = ((payload[1] - '0') * 10) + (payload[2] - '0');
			if (this->pinnummer == Eingangssnummer) 
			{
				sendestate();
				return true;
			}
		}
		return false;
	}
}






void myInput::update(bool currentState)
{
	DB20.update(currentState);
	BUTT.tick(currentState);
	if (DB20.rose()) { mysendmessage("R"); }
	if (DB20.fell()) { mysendmessage("F"); }
	if (BUTT.nowIsclick()) { mysendmessage("C"); }
	if (BUTT.nowIsdoubleClick()) { mysendmessage("D"); }
	if (BUTT.nowIsmultiClick()) { mysendmessage("M"); }
	if (BUTT.nowIslongPressStart()) { mysendmessage("p"); }
	if (BUTT.nowIslongPressStop()) { mysendmessage("l"); }
	if (BUTT.nowIsduringLongPress()) { mysendmessage("P"); }
}










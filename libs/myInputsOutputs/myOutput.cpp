
#include "myOutput.h"


myOutput::myOutput(Basiskommunikation* mqtt, myBasisHW* Hardware, uint8_t logischenummer, uint8_t hardwarepinnummer, bool logiktauschen)
{
	this->_mqtt = mqtt;
	this->_Hardware = Hardware;
	this->_logischenummer = logischenummer;
	this->_hardwarepinnummer = hardwarepinnummer;
	this->_logiktauschen = logiktauschen;



	_Hardware->setBit(_hardwarepinnummer, false != _logiktauschen);
	_Hardware->set_as_Output(_hardwarepinnummer);

}


void myOutput::toggle()
{
	 
	onoff(!_lastZustand);
}


void myOutput::setState(bool State)
{
	Anzahltodo = 0;
	onoff(State);
}
void myOutput::setTimer(uint32_t dauer)
{
	setBlink(dauer, 1);
}
void myOutput::setBlink(uint32_t HalfPeriod_ms, uint32_t periodes)
{
	naechster_zykl = millis();
	Anzahltodo = periodes * 2;
	halbePeriode = HalfPeriod_ms;

}

void myOutput::refresh()
{
	debugln("refresh1");
	bool neu = _lastZustand != _logiktauschen;
	_Hardware->set_as_Output(_hardwarepinnummer);
	_Hardware->setBit(_hardwarepinnummer, neu);
	
}


void myOutput::onoff(bool anaus)
{
	_lastZustand = anaus;
	_sende_nachricht(anaus);
	refresh();

}


void myOutput::_sende_nachricht(bool an)
{
	
	char neuespayload[2];
	char puffer[50] = {}; 
	strncpy(puffer, _topic, sizeof(puffer) - 3); 
	puffer[sizeof(puffer)-3] = '\0'; // Sicherheitshalber Nullterminierung
	puffer[sizeof(puffer)-1] = '\0'; // Sicherheitshalber Nullterminierung


	if (_logischenummer != 0xFF)
	{
		size_t ende = strlen(puffer);
		puffer[ende] = (_logischenummer / 10) + '0';
		puffer[ende + 1] = (_logischenummer % 10) + '0';
		puffer[ende + 2] = 0x00; //Stringende

		neuespayload[0] = an ? '1' : '0'; //0 oder1 an, oder aus
		neuespayload[1] = 0x00;

		_mqtt->sendmessage(puffer, neuespayload);
	}
	
}






bool  myOutput::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length < 3)
		return false;
	if (length > 12)
		return false;
	if ((payload[0] != 'A') && (payload[0] != 'T') && (payload[0] != 'B') && (payload[0] != 'G')) {
		return false;
	}

	uint8_t binichs = (payload[1] - '0') * 10 + (payload[2] - '0');
	if (binichs != _logischenummer) {
		return false;
	}

	switch(payload[0]) {
		case 'A':
			setState(payload[3] == '1');
			return true;
		case 'G':
			toggle();
			return true;
		case 'T':
			setTimer(zahlaustextvonlen((char*)payload, 3, 6) * 100);
			return true;
		case 'B':
			uint32_t anzahl = zahlaustextvonlen((char*)payload, 9, 3);
			uint32_t Zeit_zehntelsekunden = zahlaustextvonlen((char*)payload, 3, 6);
			setBlink(Zeit_zehntelsekunden * 100, anzahl);
			return true;
	}
	return false;
}




void myOutput::update()
{
	
	if (Anzahltodo > 0)
	{
		uint32_t jetzt = millis();
		if (jetzt >= naechster_zykl)
		{
			onoff((Anzahltodo + 1) % 2);
			naechster_zykl = jetzt + halbePeriode;
			Anzahltodo--;
		}
	}
}










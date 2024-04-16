// 
// 
// 

#include "phone.h"


bool  Phone::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
  if (length < 4)
    return false;
  if (length > 29)
    return false;
  if (payload[0] != 'P')
    return false;
  
   
    
        char myNumber[30];//Speicher für Telefonnummer
        uint32_t dauer = 0;
        //Todo 2  Ziffern Dauer in sekunden
        if (plausibel(payload[1], 48, 58) && plausibel(payload[2], 48, 58))
        {
          dauer = ((payload[1] - 48) * 10) + (payload[2] - 48);
        }
        else
        {
          Serial.println(F("Dauer not correct"));
          return true; // aber Fehler aufgetreten --> sollte vll melden irgendwie
        }

        for (int i = 3; i < length; i++)
        {

          if (plausibel(payload[i], 48, 58)|| payload[i] == '*' || payload[i] == 'P' || payload[i] == '#')
          {
            myNumber[i-3] = payload[i];
          }
          else
          {
            Serial.println(F("Number not possible"));
            return; 
          }
          myNumber[i - 2] = 0;//Endezeichen 
        }
        Serial.print(F("send Number: "));
        Serial.println(myNumber);
        dial_number(myNumber, dauer);

  return true;
}






 
       


















Phone::Phone( uint8_t RELAIS,uint8_t CS,uint8_t DATA,uint8_t CLK)
{
	_RELAIS = RELAIS;
	_CS = CS;
	_DATA = DATA;
	_CLK = CLK;
	
	
	
	
	
	
	
	digitalWrite(_RELAIS, 1);
	pinMode(_RELAIS, OUTPUT); // Output for Tone 2
	pinMode(_CS, OUTPUT); // Output for Tone 2
	pinMode(_DATA, OUTPUT); // Output for Tone 2
	pinMode(_CLK, OUTPUT); // Output for Tone 2
	digitalWrite(_CLK, 1);
	digitalWrite(_CS, 1);

}

void Phone::_playDigit(uint8_t number)
{
	if (number != 31)
	{
		Serial.println(number);
	}
	for (int cnt = 0; cnt < 5; cnt++)
	{
		digitalWrite(_DATA, bitRead(number, cnt));
		delayMicroseconds(d);
		digitalWrite(_CLK, 0);
		delayMicroseconds(d);
		digitalWrite(_CLK, 1);
	}
}
void Phone::dial_number(char mynumber[30], uint32_t duration)
{
	//Todo Nummer und Dauer �bertragen
	if (mode == IDLE)
	{
		for (uint8_t i; i < 30; i++)
		{
			Number[i] = mynumber[i];
		}

		Anrufdauer = duration;
		mode = PREP_DIALING;
	}
	else
	{
		//es wurde gerade noch gew�hlt, neue Nummer wird abgeblockt
		//Todo  vll Fehlermeldung
	}
}
void Phone::update()
{
	uint32_t now = millis();
	static uint32_t last_number_done = 0;
	if (mode != IDLE)
	{
		//Serial.print("Phone: "); Serial.println(mode);
	}

	switch (mode) {
	case IDLE:
		//nichts tun, warten bis Nummer kommt
		break;
	case PREP_DIALING:
		digitalWrite(_RELAIS, 0);
		digitalWrite(_CLK, 1);
		digitalWrite(_CS, 0);
		mode = WAIT_RELAIS_READY;
		nonBlockDelay_start(duration_rel); //warte bis Relais gezogen hat ist 
		break;

	case WAIT_RELAIS_READY:

		if (nonBlockDelay_update())
		{
			mode = DIALING;;
			NP = NUMBER;
			ziffernzaehler = 0;
		}
		break;
	case DIALING:

		if (dialing())
		{
			nonBlockDelay_start(Anrufdauer * 1000); 
			mode = CALL;
		}


		break;
	case CALL:
		digitalWrite(_CS, 1);

		if (nonBlockDelay_update())
		{
			mode = CALL_END;;
			nonBlockDelay_start(1000); //warte bis Relais abgefallen ist 
		}
		break;
	case CALL_END:
		digitalWrite(_RELAIS, 1);

		if (nonBlockDelay_update())
		{
			mode = IDLE;
		}

		break;
	}
}

void Phone::nonBlockDelay_start(uint32_t dauer)
{
	biswann = millis() + dauer;
	
}
bool Phone::nonBlockDelay_update()
{
	uint32_t jetzt = millis();


	return (jetzt > biswann);

}

bool Phone::dialing()
{
	if (NP == NUMBER)
	{
		if ((Number[ziffernzaehler] == 0) || (ziffernzaehler >= 30))
		{
			return true;
		}
	
		int nr = Number[ziffernzaehler] - 48;

		if (nr == 0) { _playDigit(10); }        //0
		else if (nr == -6) { _playDigit(11); }  //*
		else if (nr == -13) { _playDigit(12); } //#
		else if (nr == 32) { _playDigit(31); }  //P      (Pause) (f�r TK Anlage gedacht)
		else { _playDigit(nr); }

		nonBlockDelay_start(duration_number);
		NP = NUMW;
	}
	if (NP == NUMW)
	{
		
		if (nonBlockDelay_update())
		{
			NP = PAUSE;
		}
	}
		
	if (NP == PAUSE)
	{
		_playDigit(31);
		nonBlockDelay_start(duration_pause);
		NP = PAUW;
	}
	if (NP == PAUW)
	{
		if (nonBlockDelay_update())
		{
			NP = NUMBER;
			ziffernzaehler++;
		}
	}
	

	return false;
}


 

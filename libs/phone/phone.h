// phone.h

#ifndef _PHONE_h
#define _PHONE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
 
#include "basisinterface.h"
#include "hilfe.h"
class Phone : public BasisInterface
{
private:
	enum Phonemode
	{
		IDLE,
		PREP_DIALING,
		WAIT_RELAIS_READY,
		DIALING,
		CALL,
		CALL_END

	};
	enum NumberPause
	{
		NUMBER,
		NUMW,
		PAUSE,
		PAUW

	};
	
	 uint8_t _RELAIS;
	 uint8_t _CS;
	 uint8_t _DATA;
	 uint8_t _CLK;


	char Number[30];//Speicher f�r Telefonnummer
	uint8_t ziffernzaehler = 0;

	Phonemode mode = IDLE;
	NumberPause NP = NUMBER;
	

	void _playDigit(uint8_t number);


	const  int d = 1; //dauer nach ziffer (IC) TODO: so kurz wie m�glich (�s)
	uint32_t duration_number = 200; //dauer nach gew�hlter Ziffer (ms)
	uint32_t duration_pause = 100; //dauer nach gew�hlter Ziffer (ms)
	uint32_t duration_rel = 1800; // bis Relais gezogen hat ist

	uint32_t Anrufdauer = 0;


	uint32_t biswann = 0;
	void nonBlockDelay_start(uint32_t dauer);
	bool nonBlockDelay_update();
	bool dialing();



 public:
	
	Phone( uint8_t RELAIS,uint8_t CS,uint8_t DATA,uint8_t CLK);
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
	void dial_number(char mynumber[30], uint32_t duration);
	void update();

};






#endif

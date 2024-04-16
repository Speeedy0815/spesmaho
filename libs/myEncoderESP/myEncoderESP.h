#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "basiskommunikation.h"
#include "basisinterface.h"
#include "myBasisHW.h"
#include "hilfe.h"
#include <Rotary.h>

// ACHTUNG!!!!!! CCC als totale Krücke programmiert
// Encoder MUSS mit Interrupts abgefragt werden, um es einfach zu handeln wurde mit mehrerern globalen Variablen gearbeitet
// Bedeutet es kann nur EIN Modul geben,... oder muss umprogrammiert werden!


void IRAM_ATTR doInter();

volatile static uint8_t GLOBINTPINA;
volatile static uint8_t GLOBINTPINB;

volatile static bool GLOBVALUEA;
volatile static bool GLOBVALUEB;
static Rotary GLOBROT;                         /* the rotary object which will be created*/
volatile static int8_t GLOBWert = 0;

class myEncoderESP : public BasisInterface
{
private:
	
	bool _encoderPinALast = LOW;
	bool _encoderPinANow = LOW;
	uint8_t _encoderPinA;
	uint8_t _encoderPinB; 
	uint8_t _encnr;
 
	const char* _topic = "Enc/";
	Basiskommunikation* _mqtt;
 
public:
	 
	myEncoderESP(Basiskommunikation* mqtt, uint8_t encoderPinA, uint8_t encoderPinB, uint8_t encodernummer);
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);

	void deaktiviereInterrupts();
	void aktiviereInterrupts();
};

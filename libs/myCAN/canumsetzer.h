#pragma once

#define MYDEBUG 0
#include "mydebug.h"

#include "basisinterface.h"

#include "basiskommunikation.h"  //für Datentyp: Basiskommunikation
#include "cangemeinsam.h"
#include "canbasis.h"

class CanUmsetzer : public BasisInterface
{
private:
	uint8_t getPufferNummerornextfree(long CANid);


	const static uint8_t MAXPUFFERNACHR = 12;// wie viele Nachrichten können GLEICHZEITIG bearbeitet werden?  --> Entscheidend z.B. bei Abfrage aller Controller
	Basiskommunikation* _mqtt;

	CANInterface _CANInterface;
	CANMYNachricht NACHR[MAXPUFFERNACHR];
	 
	const char* _topic = "CAN";
	char* cantopic;
public:


	CanUmsetzer(Basiskommunikation* mqtt, canbasis* hardware);
	~CanUmsetzer();
	void update();
	bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};

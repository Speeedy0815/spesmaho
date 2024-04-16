#include "cangemeinsam.h"

//uint8_t CAN_RINGBUFF::buffctrwrite;
//uint8_t CAN_RINGBUFF::buffctrread;
//mycan_frame CAN_RINGBUFF::internalbuffer[CAN_RINGBUFF::bufflen];



//#########################################################################################################
//CANInterface
//#########################################################################################################

CANInterface::CANInterface(canbasis* canhardware)
{

	_canhardware = canhardware;

}
bool CANInterface::updateandismessage(mycan_frame* canMsg)
{
		return _canhardware->hasgetmsg(canMsg);
}
void CANInterface::publish(const char* topic, const byte* msgbuffer, uint8_t lenbuffer, long id)
{
	debug(F("CAN sende topic:"));
	debugln(topic);
	const uint8_t MAXLEN = 56; // BYTES --> max 8 Nachrichten
	uint8_t lentop = strlen(topic);

	uint8_t puffer[MAXLEN];

	uint8_t summe = lentop + lenbuffer + 3; //+3 wg 2xAngaben der Längen + anzahldernachrichten
	if (summe > MAXLEN)
	{
		Serial.print("Nachricht zu lang. breche ab!");
		return;
	}
	uint8_t lennachrichten = (summe) / 7; //+1 für das Längebyte 
	if ((summe) % 7)
	{
		lennachrichten++; // letzte Nachricht Rest rein
	}
	puffer[0] = lennachrichten;
	puffer[1] = lentop;
	puffer[2] = lenbuffer;


	memcpy(puffer + 3, topic, strlen(topic));
	memcpy(puffer + 3 + lentop, msgbuffer, lenbuffer);

	//Serial.println("Länge Nachrichten senden");
	//Serial.println(lennachrichten);

	for (uint8_t i = 0; i < lennachrichten; i++)
	{
		uint8_t sendebuf[8];

		sendebuf[0] = i;

		for (uint8_t k = 0; k < 7; k++)
		{

			sendebuf[k + 1] = puffer[k + 7 * i];

		}
		for (uint8_t h = 0; h < 10; h++)   //öfter wiederholenlassen  --> vll geht das besser
		{

			uint8_t buflen = 8;

			int8_t ERROR_CODE = _canhardware->send(id, sendebuf, buflen);


			if (ERROR_CODE == 1) // IO
			{
				debugln("fertig");
				break;
			}
			else
			{
				debug(ERROR_CODE); debug('\t');
				if (ERROR_CODE == 1) { debug("Buf sent!\r\n"); }
				else if (ERROR_CODE == -1) { debug("Get tx buff time out!\r\n"); }
				else if (ERROR_CODE == -2) {

					debug("Send msg timeout!\r\n");

				}
				else { debug("Buf send failed!\r\n"); }

			}

		}
		debug("Msg "); debug(i + 1); debugln("sent");
	}
}














//#########################################################################################################
//CANMYNachricht
//#########################################################################################################

bool CANMYNachricht::isinUse()
{
	return (nachricherhaltenbitweisesoll > 0);
}
long CANMYNachricht::forWichCAN_IDUsed()
{
	return lastCAnID;
}
bool CANMYNachricht::wertenachrichtaus(mycan_frame* canMsg)
{
	if ((canMsg->data[0] != 0) && (nachricherhaltenbitweisesoll == 0))
	{
		Serial.println("Kritischer Fehler Nachricht weg!!! breche ab");
		debugln(nachricherhaltenbitweisesoll);

 
		// alles leeren
		for (uint8_t r = 0; r < MAXLEN; ++r) { puffer[r] = 0; }
		nachricherhaltenbitweisesoll = 0;
	
		return false;
	}


	lastCAnID = canMsg->id;
	if (canMsg->data[0] == 0)
	{
		//	Serial.print("data1: "); Serial.println(canMsg->data[1]);


		lentopic = canMsg->data[2];
		lenpayload = canMsg->data[3];


		for (uint8_t j = 0; j < canMsg->data[1]; j++)
		{
			nachricherhaltenbitweisesoll |= (1 << j);
		}
	}





	nachricherhaltenbitweise |= (1 << canMsg->data[0]);

	memcpy(puffer + canMsg->data[0] * 7, canMsg->data + 1, 7);


	debug("Soll: "); debugln(nachricherhaltenbitweisesoll);
	debug("Ist: "); debugln(nachricherhaltenbitweise);
	debug("LängeTopic: "); debugln(lentopic);
	debug("LängePayload: "); debugln(lenpayload);



	//Das passiert erst, wenn fertig übertragen
	if ((nachricherhaltenbitweisesoll == nachricherhaltenbitweise) && (nachricherhaltenbitweisesoll != 0))
	{
		nachricherhaltenbitweisesoll = 0;
		nachricherhaltenbitweise = 0;

		memset(topic, 0, sizeof(topic));
		memset(payload, 0, sizeof(payload));


		memcpy(topic, puffer + 3, lentopic);
		topic[lentopic] = 0; // string abschließen



		memcpy(payload, puffer + 3 + lentopic, lenpayload);

		memset(puffer, 0, sizeof(puffer));  //puffer leeren


	 
		debugln("debugxxxxxxx");
		debug("payload:"); debugln((char*)payload);
		debug("topic:");debugln(topic);
		payload[lenpayload] = 0;
		debug("payloadlen:"); debugln(strlen((char*)payload));
		debugln("xxxxxxx");
		 

		return true;
	}
	return false;

}

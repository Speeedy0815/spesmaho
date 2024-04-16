#include "ir.h"



#define IR_SEND_PIN         3 // das muss da sein, sonst kommt die Bibliothek nicht klar. scheint ein Fehler in der Biblipothek zu sein

#if defined(__AVR__)   // Arduino Nanos haben nicht genügend Speicher, deswegen nur Auswahl --> ESPs sollen alles können

// nur ein paar Protokolle aktiviert um Programmspeicher zu sparen  --> für die Arduino Nanos, ggf anpassen
#define DECODE_JVC
#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
//#define DECODE_LG
#define DECODE_NEC          // Includes Apple and Onkyo
//#define DECODE_SAMSUNG
//#define DECODE_SONY
//#define DECODE_LEGO_PF
//#define DECODE_KASEIKYO
//#define DECODE_DENON        // Includes Sharp
// 
//
#define DECODE_RC5
//#define DECODE_RC6

//#define DECODE_BOSEWAVE
//#define DECODE_LEGO_PF
//#define DECODE_MAGIQUEST
//#define DECODE_WHYNTER
//#define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width protocols
//#define DECODE_HASH         // special decoder for all protocols
//
//#define DECODE_BEO          // This protocol must always be enabled manually, i.e. it is NOT enabled if no protocol is defined. It prevents decoding of SONY!
//#define RAW_BUFFER_LENGTH  180 
#endif

#include <IRremote.hpp>



MyIr::MyIr(Basiskommunikation* mqtt, uint8_t myREC_PIN, bool enable_send)
{
	_mqtt = mqtt;
	IrReceiver.begin(myREC_PIN, DISABLE_LED_FEEDBACK);

	_enable_send = enable_send;

	if (_enable_send)
	{
		debugln("Senden einschalten");
		IrSender.begin(IR_SEND_PIN, DISABLE_LED_FEEDBACK); // Sendepin, sollte ich mal nach außen führen
		IrSender.enableIROut(38); // Call it with 38 kHz to initialize the values printed below
		IrSender.begin(DISABLE_LED_FEEDBACK);
	}

}


void MyIr::send(uint8_t type, uint16_t aAddress, uint16_t aData_aCommand)
{

	IrReceiver.disableIRIn();

	debug("Type");
	debugln(type);

	debug("aAddress");
	debugln(aAddress);

	debug("aData_aCommand");
	debugln(aData_aCommand);


	switch (type) {
	case 8: debugln("Panasonic");
		IrSender.sendPanasonic(aAddress, aData_aCommand, 0); // LSB first
		//sendPanasonic(uint16_t aAddress, uint8_t aData, uint_fast8_t aNumberOfRepeats); // LSB first
		break;
	case 14: debugln("RC5");
		IrSender.sendRC5(aAddress, aData_aCommand, 0);
		//sendRC5(uint8_t aAddress, uint8_t aCommand, uint_fast8_t aNumberOfRepeats, bool aEnableAutomaticToggle = true);
		break;
	case 7: debugln("NEC");
		IrSender.sendNEC(aAddress, aData_aCommand, 0);
		//sendNEC(uint16_t aAddress, uint8_t aCommand, uint_fast8_t aNumberOfRepeats, bool aSendOnlySpecialNECRepeat = false);
		break;

	default: Serial.println(F("UNKNOWN")); break;
	}



	IrReceiver.enableIRIn();
}
void MyIr::update()
{
	if (IrReceiver.decode()) {      //Wenn Daten empfangen wurden,

		if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_WAS_OVERFLOW) {
			debugln("Overflow detected");
			debugln("Try to increase the RAW_BUFFER_LENGTH value of in " __FILE__);
		}
		else {
			if (IrReceiver.decodedIRData.decodedRawData < 0xFFFFFFFF)
			{
				if (IrReceiver.decodedIRData.decodedRawData != 0)
				{

					debug("RawData:");
					debugln(IrReceiver.decodedIRData.decodedRawData);
					debug("Protokoll:");
					debugln(IrReceiver.decodedIRData.protocol);
					debug("command:");
					debugln(IrReceiver.decodedIRData.command);
					debug("address:");
					debugln(IrReceiver.decodedIRData.address);

					char dataString[14] = { 0 };  // mit Nullen füllen
					sprintf(dataString, "C%c%04X%04X",
						IrReceiver.decodedIRData.protocol + 'a',
						IrReceiver.decodedIRData.address,
						IrReceiver.decodedIRData.command);
					debugln("datastring");
					debugln(dataString);
					_mqtt->sendmessage(_MYMQTTANTWORTADR, dataString);
				}
				else
				{
					debugln("0 ignored");
				}
			}
			else
			{
				debugln("To Long");
			}
		}
		IrReceiver.resume();  //Der nächste Wert soll vom IR-Empfänger eingelesen werden
	}
}
bool  MyIr::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (false == _enable_send)
		return false;
	if (length != 10)
		return false;
	if (payload[0] != 'C')
		return false;

	payload[length] = 0x00; //String abschließen!
	debugln(length);
	debugln((char*)payload);
	debugln((char*)payload + 6);
	uint8_t type = payload[1] - 'a';


	int16_t aData_aCommand = strtol((char*)payload + 6, NULL, 16);
	payload[6] = 0;

	debugln((char*)payload);
	debugln((char*)payload + 2);

	uint16_t aAddress = strtol((char*)payload + 2, NULL, 16);
	debug("adresse");
	debugln(aAddress);
	debug("aData_aCommand");
	debugln(aData_aCommand);


	send(type, aAddress, aData_aCommand);

	return true;
}

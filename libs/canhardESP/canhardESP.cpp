#include "canhardESP.h"




int8_t canhardESP::send(const long id, const uint8_t* buffer8, uint8_t size)
{
 
	CANMessage frame;
	frame.len = size;
	frame.id = id;
	frame.ext = false;//stdframe


	for (uint8_t i = 0; i < size; i++)
	{
		frame.data[i] = buffer8[i];
		debug("i: "); debug(i); debug("\t"); debug(buffer8[i]); debug("\t"); debugln((char)buffer8[i]);
	}



	const bool ok = ACAN_ESP32::can.tryToSend(frame);
	if (ok) {
		debugln("sendok");
		return 1;
	}
	else
	{
		debugln("senderr");
		return -1;
	}

}


canhardESP::canhardESP(uint8_t RXpin, uint8_t TXpin, const uint32_t geschwindigkeit)
{



	ACAN_ESP32_Settings settings(geschwindigkeit); // CAN bit rate
	//settings.mRequestedCANMode = ACAN_ESP32_Settings::LoopBackMode; // Select loopback mode
	const ACAN_ESP32_Filter filter = ACAN_ESP32_Filter::acceptStandardFrames();

	settings.mRxPin = (gpio_num_t)RXpin;
	settings.mTxPin = (gpio_num_t)TXpin;

	settings.mDriverTransmitBufferSize = 70;
	settings.mDriverReceiveBufferSize = 70;
	const uint32_t errorCode = ACAN_ESP32::can.begin(settings, filter);
	if (errorCode == 0)
	{
		debug("Bit Rate prescaler: ");
		debugln(settings.mBitRatePrescaler);
		debug("Time Segment 1: ");
		debugln(settings.mTimeSegment1);
		debug("Time Segment 2: ");
		debugln(settings.mTimeSegment2);
		debug("SJW: ");
		debugln(settings.mRJW);
		debug("Triple Sampling: ");
		debugln(settings.mTripleSampling ? "yes" : "no");
		debug("Actual bit rate: ");
		debug(settings.actualBitRate());
		debugln(" bit/s");
		debug("Exact bit rate ? ");
		debugln(settings.exactBitRate() ? "yes" : "no");
		debug("Sample point: ");
		debug(settings.samplePointFromBitStart());
		debugln("%");
		debugln("Configuration OK!");

	}
	else
	{
		Serial.print("Configuration error 0x");
		Serial.println(errorCode, HEX);

	}

}




bool canhardESP::hasgetmsg(mycan_frame* newframe)
{
	CANMessage message;
	if (ACAN_ESP32::can.receive(message)) {
		if (message.ext == false)
		{

			debugln("hasmsg");
			newframe->id = message.id;
			for (int i = 0; i < message.len; i++) {
				newframe->data[i] = message.data[i];
				debug("i: "); debug(i); debug("\t"); debug(newframe->data[i]); debug("\t"); debugln((char)newframe->data[i]);
			}
			return true;
		}
	}



	return false;

}











void canSettingloader::loadaddr()
{
	if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
		Serial.println("SPIFFS Mount Failed");
		return;
	}


	Serial.println("Loading settings CAN");
	File file;
	char buffer[30] = { 0 };
	uint8_t len = 0;

	//##################################################
	file = SPIFFS.open("/can.txt");
	if (!file) {
		Serial.println("keine Cansettings");
		return;
	}
	while (file.available())
	{
		buffer[len] = file.read();
		//Serial.print(buffer[len]);
		len++;
	}



	buffer[len] = 0;
	file.close();
	if (len > 0)
	{
		this->addr = atoi(buffer);
	}
}
void canSettingloader::saveaddr(uint8_t addr)
{

	Serial.println("Saving CAN");
	File file;


	//##################################################
	file = SPIFFS.open("/can.txt", FILE_WRITE);
	if (file.print(addr)) {
		Serial.println("- file written");
	}
	else {
		Serial.println("- write failed");
	}
	file.close();
	delay(1000);
	globalResetme();
}
uint8_t canSettingloader::getAddr()
{
	return this->addr;
}
canSettingloader::canSettingloader()
{
	loadaddr();
}
bool canSettingloader::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length < 2)
		return false;
	if (payload[0] != 'c')
		return false;



	if (payload[1] == 'S')
	{
		if (startaddr != addr)
		{
			Serial.println("Canaddr schon gesetzt");
			return false;//Änderung der Adresse nur einmal zulassen. Wenn es nochmal geändert werden soll, dann muss neu Daten geflasht werden
		}
		uint8_t adde = atoi((char*)payload + 2);
		Serial.println("speichere:");
		Serial.println(adde);
		saveaddr(adde);
		return true;
	}
	if (payload[1] == 'D')
	{
		Serial.println("deleteSetting");
		SPIFFS.remove("/can.txt");
		return true;
	}
	return false;

}
// 
// 
// 

#include "myonewire.h"


XOnewire::XOnewire(byte onewirepin, Basiskommunikation* mqtt,bool useonytemperature) :_onew(onewirepin)
{
	_mqtt = mqtt;
	TimerTemp.start(sendebegrenzunginms_Temperature, true);
	if (useonytemperature)
	{
		usekeys = false;
	}
}

XOnewire::XOnewire(byte onewirepin, Basiskommunikation* mqtt) :_onew(onewirepin)
{

	_mqtt = mqtt;
	TimerTemp.start(sendebegrenzunginms_Temperature, true);

}
void XOnewire::updateTempsensor()
{
	while (_onew.search(addr))
	{
		if (addr[0] != 0x01 && addr[0] != 0x10 && addr[0] != 0x28) {
			Serial.println(F("Device is unknown"));
		}
		else
		{
			if (_onew.crc8(addr, 7) != addr[7]) {
				Serial.println(F("CRC not valid!"));
			}
			else
			{
				if (addr[0] == 0x10 || addr[0] == 0x28) // Temperature-Sensor
				{
					//sSerial.println("hier");
					byte data[2];
					_onew.reset();
					_onew.select(addr);
					_onew.write(0x44, 1); // start conversion, with parasite power on at the end

					_onew.reset();
					_onew.select(addr);
					_onew.write(0xBE); // Read Scratchpad


					for (int i = 0; i < 2; i++) { // we need 2 bytes
						data[i] = _onew.read();
					}
					 
					int16_t tempRead = (((int16_t)data[1] << 8) | (uint16_t)data[0]); //using two's compliment  --> ist bestimmt uint16,... da stimmt was nicht
					float TemperatureSum = (float)tempRead / 16.0;

					char addressestr[50];
					uint8_t start = strlen(_MYMQTTANTTEMP);
					memcpy(addressestr, _MYMQTTANTTEMP, start);
					 

					uint8_t z = 0;
					for (z = 0; z < 8; z++) {
				 
						char tmp[3];
						sprintf(tmp, "%02X", addr[z]);
						addressestr[(2 * z) + start] = tmp[0];
						addressestr[2 * z + 1 + start] = tmp[1];

					}
					addressestr[z + start] = 0x00;

					//Serial.print("Temperatur:");
					//Serial.println(TemperatureSum);
				 
					strcpy(sendebuffer, "");
					//dtostrf(TemperatureSum, 20, 3, &sendebuffer[strlen(sendebuffer)]);
					dtostrf(TemperatureSum, 0, 2, sendebuffer);
					
					if (TemperatureSum == 85) // lt Datenblatt noch nicht fertig mit rechnen --> auf nächsten Wert warten
					{
						Serial.println(F("Temp not ready"));
					}
					else
					{
						_mqtt->sendmessage(addressestr, sendebuffer);
					}
					
				}
			}
		}
	}
	 
	_onew.reset_search();
}
void XOnewire::updSchluesselandSend()
{
	 
	if (TimerSchl.update_darfich())
	{
		 
		button_darfsenden = true;
	}
	if (button_darfsenden)
	{
		 
		while (_onew.search(addr))
		{


			if (addr[0] != 0x01 && addr[0] != 0x10 && addr[0] != 0x28) {
				Serial.println(F("Device is unknown"));
			}
			else
			{

				if (_onew.crc8(addr, 7) != addr[7]) {
					Serial.println(F("CRC not valid!"));
				}
				else
				{
					if (addr[0] == 0x01) // I-Button
					{

						uint8_t i = 0;
						for (i = 0; i < 8; i++) {
							char tmp[2];
							sprintf(tmp, "%02X", addr[i]);
							sendebuffer[2 * i] = tmp[0];
							sendebuffer[2 * i + 1] = tmp[1];
						}
						sendebuffer[i] = 0x00;
						_mqtt->sendmessage(_MYMQTTANTSCH, sendebuffer);
						TimerSchl.start(sendebegrenzunginms_Ibutton, false);
						button_darfsenden = false;

					}

				}
			}
		}

		_onew.reset_search();
	}
}

void XOnewire::update()
{
	if (TimerTemp.update_darfich())
	{
		//Serial.println(F("Temp"));
		updateTempsensor();
	}
	if (usekeys)
	{	
		updSchluesselandSend();
	}
	
}
bool XOnewire::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	return false; //nur senden, nichts empfangen
}


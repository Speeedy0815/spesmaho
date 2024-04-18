#include "mySpiffsSettingsloader.h"




mySpiffsSettingsloaderString::mySpiffsSettingsloaderString(const char* filepfad, const char* stdname)
{
	_stdname = stdname;
	_filepfad = filepfad;

	loadaddr();
}

void mySpiffsSettingsloaderString::loadaddr()
{
	if (!SPIFFS.begin()) {
		SPIFFS.format();
		Serial.println("SMF"); //SPIFFS Mount Failed
		return;
	}
	debug("Pfad: ");
	debugln(_filepfad);
	debugln("LoadSet");
	File file;

	uint8_t len = 0;

	//##################################################
	file = SPIFFS.open(_filepfad, "r");
	if (!file) {
		Serial.println("NoSetFount");

	}
	while (file.available())
	{
		gespeicherterName[len] = file.read();
		//Serial.print(buffer[len]);
		len++;
	}
	debug("Filelaenge");

	gespeicherterName[len] = 0;
	file.close();
	if (len > 0) { somethingsaved = true; }
	else { somethingsaved = false; }
}
void mySpiffsSettingsloaderString::saveaddr(char* sett)
{
	Serial.println("Saving");
	File file;
	//##################################################
	file = SPIFFS.open(_filepfad, "w");
	if (file.print(sett)) {
		Serial.println("- f w");//file written
	}
	else {
		Serial.println("- w f");//write failed
	}
	file.close();
	delay(1000);
	globalResetme();
}
const char* mySpiffsSettingsloaderString::getSett()
{
	if (somethingsaved)
	{
		return ((const char*)gespeicherterName);
	}
	else
	{
		return _stdname;
	}
}


bool mySpiffsSettingsloaderString::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	debugln("mySpiffsSettingsloaderString");
	debugln(topic);
	debugln((char*)payload);


	if (length < 2)
		return false;
	if (payload[0] != 'c')
		return false;

	debugln("mySpiffsSettingsloaderString drin");

	if (payload[1] == 'S')
	{
		if (somethingsaved)
		{
			Serial.println("Addr schon gesetzt");
			return false;//Änderung der Adresse nur einmal zulassen. Wenn es nochmal geändert werden soll, dann muss neu Daten geflasht werden
		}

		Serial.println("save:");
		Serial.println((char*)payload + 2);
		saveaddr((char*)payload + 2);
		return true;
	}
	if (payload[1] == 'D')
	{
		Serial.println("deleteSetting");
		SPIFFS.remove(_filepfad);
		return true;
	}
	return false;

}
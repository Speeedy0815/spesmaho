// 
// 
// 

#include "myheiztemp.h"

void HeizTemp::saveSettings()
{
	Serial.println("Saving settings");
	File file;


	//##################################################
	file = SPIFFS.open("/SollTemp", FILE_WRITE);
	if (file.print(this->solltemperatur)) {
		Serial.println("- file written");
	}
	else {
		Serial.println("- write failed");
	}
	file.close();
	//##################################################
	file = SPIFFS.open("/Hys1", FILE_WRITE);
	if (file.print(this->diffussboden)) {
		Serial.println("- file written");
	}
	else {
		Serial.println("- write failed");
	}
	file.close();
	//##################################################
	file = SPIFFS.open("/Hys2", FILE_WRITE);
	if (file.print(this->difHeizk)) {
		Serial.println("- file written");
	}
	else {
		Serial.println("- write failed");
	}
	file.close();


}
void HeizTemp::loadSettings()
{
	Serial.println("Loading settings Heizung");
	File file;
	char buffer[30] = { 0 };
	uint8_t len = 0;

	//##################################################
	file = SPIFFS.open("/SollTemp");
	if (!file) {
		Serial.println("kein Settings für Solltemperatur");
	}
	while (file.available())
	{
		buffer[len] = file.read();
		//Serial.print(buffer[len]);
		len++;
	}
	buffer[len] = 0;
	file.close();
	this->solltemperatur = atof(buffer);
	//##################################################


	len = 0;
	file = SPIFFS.open("/Hys1");
	if (!file) {
		Serial.println("kein Settings für Hys1");
	}
	while (file.available())
	{
		buffer[len] = file.read();
		//Serial.print(buffer[len]);
		len++;
	}
	buffer[len] = 0;
	file.close();
	this->diffussboden = atof(buffer);
	//##################################################

	len = 0;
	file = SPIFFS.open("/Hys2");
	if (!file) {
		Serial.println("kein Settings für Hys2");
	}
	while (file.available())
	{
		buffer[len] = file.read();
		//Serial.print(buffer[len]);
		len++;
	}
	buffer[len] = 0;
	file.close();
	this->difHeizk = atof(buffer);


}


HeizTemp::HeizTemp(Basiskommunikation* mqtt, myBasisHW* Hardware, MyHDC108* temperatursensor, uint8_t fussbodenpin, uint8_t heizkoerperpin)
{
	if (!SPIFFS.begin(true)) {
		Serial.println("SPIFFS Mount Failed");
		return;
	}
	_temperatursensor = temperatursensor;
	_hardware = Hardware;
	_mqtt = mqtt;
	_fussbodenpin = fussbodenpin;
	_heizkoerperpin = heizkoerperpin;
	loadSettings();

	Serial.print(F("Hys1: ")); Serial.println(diffussboden);
	Serial.print(F("Hys2: ")); Serial.println(difHeizk);

	//Achtung Relaisplatine dreht 
	_hardware->setBit(_fussbodenpin, true);
	_hardware->setBit(_heizkoerperpin, true);
	_hardware->set_as_Output(_fussbodenpin);
	_hardware->set_as_Output(_heizkoerperpin);
	Regeltimertimer.start(Regelzeit, true);
	Sendetimer.start(Sendezeit, true);
}
int32_t HeizTemp::getisNachtabsenkung() {

	if (nachtabsenkungstemperatur > 0)
		return 1;
	if (nachtabsenkungstemperatur < 0)
		return ((int32_t)-1);

	return 0;


}
void HeizTemp::update()
{
	if (Regeltimertimer.update_darfich())
	{
		_temperatursensor->update();
		isttemperatur = _temperatursensor->getTemperature();

		if (windowwopen)
		{
			heizefussboden = false;
			heizeHeizk = false;
		}
		else
		{
			double ausschalttemperatur = solltemperatur - nachtabsenkungstemperatur;
			double einschalttemperaturfussb = solltemperatur - diffussboden - nachtabsenkungstemperatur;
			double einschalttemperaturheiz = solltemperatur - difHeizk - nachtabsenkungstemperatur;
			if (isttemperatur > ausschalttemperatur)
			{
				 
				heizefussboden = false;
				heizeHeizk = false;
			}
			if (isttemperatur < einschalttemperaturfussb)
			{
				heizefussboden = true;
			}
			if (isttemperatur < einschalttemperaturheiz)
			{
				heizeHeizk = true;
			}
		}
		_hardware->set_as_Output(_fussbodenpin);    //nur zur Sicherheit, falls was gesponnen hat zwischendrin
		_hardware->set_as_Output(_heizkoerperpin);  //nur zur Sicherheit, falls was gesponnen hat zwischendrin
		_hardware->setBit(_heizkoerperpin, !heizeHeizk);
		_hardware->setBit(_fussbodenpin, !heizefussboden);
		sendHeizungStatus(false);
	}

	if (Sendetimer.update_darfich())
	{
		sendHeizungStatus(true);
		sendSolltemp();
	}


}
void HeizTemp::setRelativeSollTemp(double value, bool save)
{
	setAbsSollTemp(solltemperatur += value, save);

}
void HeizTemp::setAbsSollTemp(double value, bool save)
{
	solltemperatur = value;
	sendSolltemp();
	if (save)
	{
		saveSettings();
	}
}
void HeizTemp::sendSolltemp()
{
	_mqtt->sendmessage(_MYMQTTANTSoll, solltemperatur);

}
void HeizTemp::sendHeizungStatus(bool force)
{
	static bool heizefussbodenvorher = 0;
	if ((heizefussboden != heizefussbodenvorher) || force)
	{
		_mqtt->sendmessage(_MYMQTTANTFB, heizefussboden);
		heizefussbodenvorher = heizefussboden;
	}
	static bool heizeheizkbodenvorher = 0;
	if ((heizeHeizk != heizeheizkbodenvorher) || force)
	{
		_mqtt->sendmessage(_MYMQTTANTHK, heizeHeizk);
		heizeheizkbodenvorher = heizeHeizk;
	}
}
bool HeizTemp::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length < 2)
		return false;


	if (payload[0] != 'H')
		return false;

	if (payload[1] == 'T') // sTatus
	{
		//sendHeizungStatus(true);
		//sendSolltemp();
		sendnachtabsenk();
		sendFenster();
		sendH1();
		sendH2();
		sendnachtabsenkTemp();
		//_temperatursensor->sendeSensoren();
	}

 



	if (payload[1] == 'S') // solltemperatur
	{
		if (payload[2] == '+')
		{
			setRelativeSollTemp(0.5, false);
		}
		else if (payload[2] == '-')
		{
			setRelativeSollTemp(-0.5, false);
		}
		else
		{
			setAbsSollTemp(atof((char*)payload + 2), false);
		}


	}
	if (payload[1] == 's') // solltemperatur + Speichern
	{
		if (payload[2] == '+')
		{
			setRelativeSollTemp(0.5, true);
		}
		else if (payload[2] == '-')
		{
			setRelativeSollTemp(-0.5, true);
		}
		else
		{
			setAbsSollTemp(atof((char*)payload + 2), true);
		}
	}
	if (payload[1] == 'H') // Hysterese Fußbodenheizung
	{
		this->diffussboden = atof((char*)payload + 2);
		saveSettings();
		sendH1();
	}
	if (payload[1] == 'h') //  Hysterese Heizkörper
	{
		this->difHeizk = atof((char*)payload + 2);
		saveSettings();
		sendH2();
	}
	if (payload[1] == 'F') // Fenster ist offen
	{
		Serial.print("Fenster:");
		Serial.println((char)payload[2]);
		this->windowwopen = (char)payload[2] - '0';
		sendFenster();
	}
	if (payload[1] == 'N') // Nachtabsenkung
	{
		this->nachtabsenkungstemperatur = atof((char*)payload + 2);
		sendnachtabsenk();
		sendnachtabsenkTemp();
	}




	return false;
}
void HeizTemp::sendFenster()
{
	if (windowwopen)
	{
		_mqtt->sendmessage(_MYMQTTANTF, "1");
	}
	else
	{
		_mqtt->sendmessage(_MYMQTTANTF, "0");
	}
	
}
void HeizTemp::sendnachtabsenk()
{
	_mqtt->sendmessage(_MYMQTTANTN, (int32_t) this->getisNachtabsenkung());
}
void HeizTemp::sendnachtabsenkTemp()
{
	_mqtt->sendmessage(_MYMQTTANTn, nachtabsenkungstemperatur);
}
void HeizTemp::sendH1()
{
	_mqtt->sendmessage(_MYMQTTANTH1, diffussboden);
}
void HeizTemp::sendH2()
{
	_mqtt->sendmessage(_MYMQTTANTH2, difHeizk);
}
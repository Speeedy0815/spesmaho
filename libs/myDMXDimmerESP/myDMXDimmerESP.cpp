#include "myDMXDimmerESP.h"




myDMXDimmerESP::myDMXDimmerESP()
{
	DMX::Initialize(DMXDirection::output);

	//dmx.init(maxChan);


	//DMX::Write(1, 255);
	//setmode(3, 4, 3, 0,255);  //B
	//setmode(4, 5, 3, 127,255);  //W

}

void myDMXDimmerESP::rechnemilivalue()
{
	unsigned long _millis = millis();
	unsigned long _mycros = micros();



	geschwindigkeitsvalues[0] = ((_millis / 32) % 255);
	geschwindigkeitsvalues[1] = ((_millis / 16) % 255);
	geschwindigkeitsvalues[2] = ((_millis / 8) % 255);
	geschwindigkeitsvalues[3] = ((_millis / 4) % 255);
	geschwindigkeitsvalues[4] = ((_millis / 3) % 255);
	geschwindigkeitsvalues[5] = ((_millis / 2) % 255);
	geschwindigkeitsvalues[6] = ((_millis) % 255);
	geschwindigkeitsvalues[7] = ((_mycros / 500) % 255);
	geschwindigkeitsvalues[8] = ((_mycros / 250) % 255);
	geschwindigkeitsvalues[9] = ((_mycros / 125) % 255);





}
uint8_t myDMXDimmerESP::getmiliValue(uint8_t chanr)
{

	return (
		geschwindigkeitsvalues[geschwindigkeitsmodus[chanr]] + offset[chanr]  //Überlauf  --> wird toleriert


		);
}


void myDMXDimmerESP::update()
{
	rechnemilivalue();
	for (uint8_t i = 0; i < maxChan; i++)
	{
		switch (modus[i])
		{
		case 1: //viereck
			if (getmiliValue(i) > 127) { DMX::Write(i, 255); }
			else {  DMX::Write(i, 0); }
			break;
		case 2://dreieck
		 
			DMX::Write(i, uint8_t(getmiliValue(i)));
			break;
		case 3://sinus
		 
			DMX::Write(i, sinTable[(getmiliValue(i))]);
			break;
		default://0 oder anderes stabil
			break;
		}

	}




	//dmx.update(); ccc hier müsste theoretisch aktualisieren kommen
}


bool  myDMXDimmerESP::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	//fixeLänge
	//McccMvvvVOOO
	debugln("DimmerESP reached");
	if (payload[0] != 'M')
		return false;
	if (length != 12)
	{
		debug("LF: "); //Längefalsch
		debugln(length);
		return false;
	}



	uint8_t mychan = 0;
	uint8_t myvalue = 0;
	uint8_t mymode = 0;
	uint8_t myoffset = 0;
	uint8_t mygeschw = 0;

	//Reihenfolge wichtig --> Hack
	myoffset = atoi((char*)payload + 9);
	payload[9] = 0; //String zwischendrin beenden
	mygeschw = atoi((char*)payload + 8);
	payload[8] = 0; //String zwischendrin beenden
	myvalue = atoi((char*)payload + 5);
	payload[5] = 0; //String zwischendrin beenden
	mymode = atoi((char*)payload + 4);
	payload[4] = 0; //String zwischendrin beenden
	mychan = atoi((char*)payload + 1);
	payload[1] = 0; //String zwischendrin beenden

	debug("myoffset:"); debugln(myoffset);
	debug("mygeschw:"); debugln(mygeschw);
	debug("myvalue:"); debugln(myvalue);
	debug("mymode:"); debugln(mymode);
	debug("mychan:"); debugln(mychan);

	if (mychan < maxChan)
	{
		setmode(mymode, mychan, mygeschw, myoffset, myvalue);
	}
	else
	{
		Serial.print("ERR! MaxChan: "); Serial.println(maxChan);
	}



	return true;
}
void myDMXDimmerESP::setmode(uint8_t mod, uint8_t chan, uint8_t mygeschwindigkeit, uint8_t myoffset, uint8_t value)
{
	modus[chan] = mod;
	geschwindigkeitsmodus[chan] = mygeschwindigkeit;
	offset[chan] = myoffset;
	if (mod == 0)
	{
		
		DMX::Write(chan, value);
	}
}
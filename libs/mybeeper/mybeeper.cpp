#include "mybeeper.h"



MyBeep::MyBeep(Basiskommunikation* mqtt,uint8_t pwmchannel)
{

	_mqtt = mqtt;


	_pwmchannel = pwmchannel;
	ledcSetup(pwmchannel, freq, resolution);
    ledcAttachPin(ledPin, pwmchannel);


 
	ledcWrite(_pwmchannel, 0);

}


void MyBeep::update()
{
 
	if (TIMER.update_darfich())
	{
		debugln("aus");
		ledcWrite(_pwmchannel, 0);  //ausschalten
		TIMER.stop(); 
	}

 
		
 

}
bool  MyBeep::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length < 3)
		return false;
	if (length > 10)
		return false;
	if (payload[0] != 'P')
		return false;
	//P f�r Piel
	//0-9 Tonh�he
	//0-9 Dauer

	

	dauer = ( ((payload[1] - '0') * 100))+100;  //100-1100ms
	debug("Dauer: ");
	debugln(dauer);
	
	uint8_t oktave = payload[3] - '0';
	debug("Oktave: ");
	debugln(oktave);
	
	note_t meineNote =  NOTE_C;
	char noteroh = payload[2];



	switch (noteroh) {
        case 'C':
            meineNote =  NOTE_C; break;
        case 'c':
            meineNote =  NOTE_Cs; break;
        case 'D':
            meineNote =  NOTE_D; break;
        case 'd':
            meineNote =  NOTE_Eb; break;
        case 'E':
            meineNote =  NOTE_E; break;
        case 'F':
            meineNote =  NOTE_F; break;
        case 'f':
            meineNote =  NOTE_Fs; break;
        case 'G':
            meineNote =  NOTE_G; break;
        case 'g':
            meineNote =  NOTE_Gs; break;
        case 'A':
            meineNote =  NOTE_A; break;
        case 'b':
            meineNote =  NOTE_Bb; break;
        case 'B':
            meineNote =  NOTE_B; break;
    }



	debug("noteroh: ");
	debugln(noteroh);
	debug("note: ");
	debugln(meineNote);

	ledcWriteNote(_pwmchannel,meineNote,oktave);
	TIMER.start(dauer, false);  //Timerreset, dass das piepsen nicht sofort wieder ausgeschaltet wird

	return true;
}

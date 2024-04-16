
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
 
class myTimer
{
public:
	myTimer();
	void start(unsigned long delay, bool myfirsttimeforce);
	void stop();
	bool  update_darfich();
private:
	bool firsttimeforce = false;
	bool isrunning = false;
	unsigned long lasttick = 0;
	unsigned long dauer_ticks = 1000;
};
 
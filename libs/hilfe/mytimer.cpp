#include "mytimer.h"


     


//#############################################################################################
//Timer
//#############################################################################################
myTimer::myTimer()
{

}
void myTimer::start(unsigned long delay, bool myfirsttimeforce)
{
	dauer_ticks = delay;
	lasttick = millis();
	isrunning = true;
	firsttimeforce = myfirsttimeforce;
}
void myTimer::stop()
{
	isrunning = false;
}
bool  myTimer::update_darfich()
{
	if (isrunning)
	{
		unsigned long jetzt = millis();

		if (((jetzt - lasttick) > dauer_ticks) || firsttimeforce)
		{
			firsttimeforce = false;
			lasttick = jetzt;
			return true;
		}
	}
	return false;
}



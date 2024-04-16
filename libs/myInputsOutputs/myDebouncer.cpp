// Please read Bounce2.h for information about the liscence and authors
// Please read Bounce2.h for information about the liscence and authors


#include "myDebouncer.h"

//////////////
// DEBOUNCE //
//////////////

myDebouncer::myDebouncer():previous_millis(0)
    , interval_millis(10)
    , state(0) {}

void myDebouncer::interval(uint16_t interval_millis)
{
    this->interval_millis = interval_millis;
}

void myDebouncer::begin(bool currentState) {
	 state = 0;
    if (currentState) {
        setStateFlag(DEBOUNCED_STATE | UNSTABLE_STATE);
    }

	#ifdef BOUNCE_LOCK_OUT
    previous_millis = 0;
#else
    previous_millis = millis();
#endif
}

bool myDebouncer::update(bool currentState)
{

    unsetStateFlag(CHANGED_STATE);

 
    
    // If the reading is different from last reading, reset the debounce counter
    if ( currentState != getStateFlag(UNSTABLE_STATE) ) {
        previous_millis = millis();
         toggleStateFlag(UNSTABLE_STATE);
    } else
        if ( millis() - previous_millis >= interval_millis ) {
            // We have passed the threshold time, so the input is now stable
            // If it is different from last state, set the STATE_CHANGED flag
            if (currentState != getStateFlag(DEBOUNCED_STATE) ) {
                previous_millis = millis();

                 changeState();
            }
        }

		return  changed(); 

}

// WIP HELD
unsigned long myDebouncer::previousDuration() {
	return durationOfPreviousState;
}

unsigned long myDebouncer::duration() {
	return (millis() - stateChangeLastTime);
}

inline void myDebouncer::changeState() {
	toggleStateFlag(DEBOUNCED_STATE);
	setStateFlag(CHANGED_STATE) ;
	durationOfPreviousState = millis() - stateChangeLastTime;
	stateChangeLastTime = millis();
}

bool myDebouncer::read()
{
    return  getStateFlag(DEBOUNCED_STATE);
}

bool myDebouncer::rose()
{
    return getStateFlag(DEBOUNCED_STATE) && getStateFlag(CHANGED_STATE);
}

bool myDebouncer::fell()
{
    return  !getStateFlag(DEBOUNCED_STATE) && getStateFlag(CHANGED_STATE);
}




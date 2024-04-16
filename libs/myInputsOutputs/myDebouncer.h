#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include <inttypes.h>

//Die Klasse basiert auf Bounce2, wurde nur ein wenig abgeändert um das Handling von I2C Eingängen und anderen zu vereinfachen

class myDebouncer
{
	// Note : this is private as it migh change in the futur
private:
	static const uint8_t DEBOUNCED_STATE = 0b00000001;
	static const uint8_t UNSTABLE_STATE = 0b00000010;
	static const uint8_t CHANGED_STATE = 0b00000100;

private:
	inline void changeState();
	inline void setStateFlag(const uint8_t flag) { state |= flag; }
	inline void unsetStateFlag(const uint8_t flag) { state &= ~flag; }
	inline void toggleStateFlag(const uint8_t flag) { state ^= flag; }
	inline bool getStateFlag(const uint8_t flag) { return((state & flag) != 0); }

public:

	myDebouncer();
	void interval(uint16_t interval_millis);
	bool update(bool currentState);
	bool read();
	bool fell();
	bool rose();

public:

	bool changed() { return getStateFlag(CHANGED_STATE); }
	unsigned long duration();
	unsigned long previousDuration();
	void begin(bool currentState);

protected:

	unsigned long previous_millis;
	uint16_t interval_millis;
	uint8_t state;
	unsigned long stateChangeLastTime;
	unsigned long durationOfPreviousState;

};

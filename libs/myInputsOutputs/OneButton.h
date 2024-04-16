#pragma once
#ifndef OneButton_h
#define OneButton_h

#include "Arduino.h"
#include "hilfe.h"
#include "mytimer.h"

class OneButton
{
public:
	// ----- Constructor -----
	OneButton();
	OneButton(const int pin, const boolean activeLow = true, const bool pullupActive = true);
	void setDebounceTicks(const int ticks);
	void setClickTicks(const int ticks);
	void setPressTicks(const int ticks);

	void attachClick();
	void attachDoubleClick();
	void attachMultiClick();
	void attachLongPressStart();
	void attachLongPressStop();
	void attachDuringLongPress();

	void tick(bool level);
	void reset(void);
	int getNumberClicks(void);
	bool isIdle() const { return _state == OCS_INIT; }
	bool isLongPressed() const { return _state == OCS_PRESS; };



	bool nowIsclick() { bool tmp = _isNowclick;                _isNowclick = false;				return tmp; };
	bool nowIsdoubleClick() { bool tmp = _isNowdoubleClick;			 _isNowdoubleClick = false;			return tmp; };
	bool nowIsmultiClick() { bool tmp = _isNowmultiClick;			 _isNowmultiClick = false;			return tmp; };
	bool nowIslongPressStart() { bool tmp = _isNowlongPressStart;       _isNowlongPressStart = false;      return tmp; };
	bool nowIslongPressStop() { bool tmp = _isNowlongPressStop;		 _isNowlongPressStop = false;       return tmp; };
	bool nowIsduringLongPress() { bool tmp = _isNowduringLongPress;      _isNowduringLongPress = false;     return tmp; };





private:

	myTimer _hilfTimerLongpress; 
	int _pin;                         // hardware pin number.
	unsigned int _debounceTicks = 50; // number of ticks for debounce times.
	unsigned int _clickTicks = 200;   // number of msecs before a click is detected.
	unsigned int _pressTicks = 800;   // number of msecs before a long button press is detected
	unsigned int _longpresstimer = 300;   // number of msecs before a long button press is detected


	int _buttonPressed;


	bool _clickFunc = false;
	bool _doubleClickFunc = false;
	bool _multiClickFunc = false;
	bool _longPressStartFunc = false;
	bool _longPressStopFunc = false;
	bool _duringLongPressFunc = false;



	bool _isNowclick = false;
	bool _isNowdoubleClick = false;
	bool _isNowmultiClick = false;
	bool _isNowlongPressStart = false;
	bool _isNowlongPressStop = false;
	bool _isNowduringLongPress = false;



	// These variables that hold information across the upcoming tick calls.
	// They are initialized once on program start and are updated every time the
	// tick function is called.

	// define FiniteStateMachine
	enum stateMachine_t : int {
		OCS_INIT = 0,
		OCS_DOWN = 1,
		OCS_UP = 2,
		OCS_COUNT = 3,
		OCS_PRESS = 6,
		OCS_PRESSEND = 7,
		UNKNOWN = 99
	};

	/**
	 *  Advance to a new state and save the last one to come back in cas of bouncing detection.
	 */
	void _newState(stateMachine_t nextState);

	stateMachine_t _state = OCS_INIT;
	stateMachine_t _lastState = OCS_INIT; // used for debouncing

	unsigned long _startTime; // start of current input change to checking debouncing
	int _nClicks;             // count the number of clicks with this variable
	int _maxClicks = 1;       // max number (1, 2, multi=3) of clicks of interest by registration of event functions.
};

#endif

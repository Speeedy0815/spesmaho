#include "mydisplaybeleuchter.h"


DiditalBeleuchtungDisplay::DiditalBeleuchtungDisplay(myBasisHW* Hardware, uint8_t ledpin)
{
	_Hardware = Hardware;
	_ledpin = ledpin;


	_Hardware->setBit(_ledpin, false);
	_Hardware->set_as_Output(_ledpin);

}
void DiditalBeleuchtungDisplay::schaltestate()
{
	_Hardware->setBit(_ledpin, _state);
}

void	DiditalBeleuchtungDisplay::schalteAn()
{
	_state = true;
	schaltestate();
}
void	DiditalBeleuchtungDisplay::schalteAus()
{
	_state = false;
	schaltestate();
}
bool	DiditalBeleuchtungDisplay::istDisplayAn()
{
	return _state;
}
//##############################################################################################
 PWMBeleuchtungDisplay::PWMBeleuchtungDisplay(uint8_t ledpin)
{

	debugln("init Beleuchtung");
	//Hintergrundbeleuchtung initialisieren
	ledcSetup(ledChannelhintergrundbel, freqhintergrundbel, resolutionhintergrundbel);
	ledcAttachPin(ledpin, ledChannelhintergrundbel);
	Hintergrundbeleuchtungstimer.start(Hintergrundbeleuchtungstimerzeit, true);

}

void	PWMBeleuchtungDisplay::schalteAn()
{
	sollstate = true;
}
void	PWMBeleuchtungDisplay::schalteAus()
{
	sollstate = false;
}
bool	PWMBeleuchtungDisplay::istDisplayAn()
{
	return (dutyCycleHintergrundbeleuchtung >= 40);
}
void PWMBeleuchtungDisplay::update()
{
	if (Hintergrundbeleuchtungstimer.update_darfich())
	{
		if (sollstate)
		{
			setHintergrund(1);
		}
		else
		{
			setHintergrund(-1);
		}
	}
}
void PWMBeleuchtungDisplay::setHintergrund(int8_t val)
{
	dutyCycleHintergrundbeleuchtung += val;
	if (dutyCycleHintergrundbeleuchtung > 255) { dutyCycleHintergrundbeleuchtung = 255; }
	if (dutyCycleHintergrundbeleuchtung < 0) { dutyCycleHintergrundbeleuchtung = 0; }
	ledcWrite(ledChannelhintergrundbel, dutyCycleHintergrundbeleuchtung);


}
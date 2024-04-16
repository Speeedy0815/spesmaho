#include "sperrausgang.h"
#include "hilfe.h"
SperrAusgang::SperrAusgang(myBasisHW* Hardware,uint8_t * Adressliste,uint8_t laenge)
{ 
	_adressliste = Adressliste;
	_anzahl_ausgaenge =  laenge;
	_Hardware = Hardware;
	
  for (uint8_t i = 0; i < _anzahl_ausgaenge; i++)
  {
	  _Hardware->setBit(_adressliste[i], 0);
	  _Hardware->set_as_Output(_adressliste[i]);
  }

} 

 
void SperrAusgang::update()
{
	//nichts zu tun in dem Modul
} 
 
bool  SperrAusgang::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	if (length < 2)
		return false;
	if (length > 2)
		return false;
	if (payload[0] != 'I')
		return false;
	uint8_t Ausgangsnummer = ((payload[1] - 48)-1);
	if (!((Ausgangsnummer == 0xff) ||  plausibel(Ausgangsnummer, 0, _anzahl_ausgaenge - 1)))
		return false;
	
	debugln(Ausgangsnummer);
	for (uint8_t i = 0; i < _anzahl_ausgaenge; i++)
    { 
       
          if (i == Ausgangsnummer)
          {

			  _Hardware->setBit(_adressliste[i], 1);
           
          }else
          {
         
			  _Hardware->setBit(_adressliste[i], 0);
          }
    }
	

	return true;
}

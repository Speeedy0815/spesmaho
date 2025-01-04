#include "softwareserialrx.h"
MySsRx::MySsRx(Basiskommunikation* mqtt,const char * MYMQTTANTWORTADR,uint8_t RX_Pin):_mySerial(RX_Pin ,0)
{ 
   _MYMQTTANTWORTADR = MYMQTTANTWORTADR;
   _mqtt = mqtt;
	_mySerial.begin(9600);
} 

 
void MySsRx::update()
{
	String msgString;
	while (_mySerial.available() ) {
    char inByte = _mySerial.read();
        if (ziffernzaehler >28) // irgendwas ist schief gelaufen, Puffer darf nicht überschritten werden! Speicherüberschreiber
        {
           ziffernzaehler = 0;
           zahl[0] = 0; // leerer String
        }
        if (inByte == 13)  //NL wird ignoriert
        {
        }else if (inByte == 10) // Zeilenende --> Sende raus
        {
           zahl[ziffernzaehler+1] = 0;
          _mqtt->sendmessage(_MYMQTTANTWORTADR, zahl);

          ziffernzaehler = 0;
          zahl[0] = 0; // leerer String
        }else
        {
          zahl[ziffernzaehler] = inByte;
          ziffernzaehler ++;
        }
    } 
}  
bool  MySsRx::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	return false;
}

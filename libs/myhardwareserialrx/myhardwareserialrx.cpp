#include "myhardwareserialrx.h"
MyHsRx::MyHsRx(Basiskommunikation* mqtt)
{ 
 
   _mqtt = mqtt;
	Serial1.begin(9600);
} 

 
void MyHsRx::update()
{
	String msgString;
	while (Serial1.available() ) {
    char inByte = Serial1.read();
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
bool  MyHsRx::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	return false;
}

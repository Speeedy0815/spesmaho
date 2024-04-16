#include "myRGBWbasis.h"




bool  myRGBWbasis::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	//Serial.println((char*)payload);
	if (length < 4)
		return false;
	if (length > 12)
		return false;
	if (payload[0] != 'L')
		return false;
	if (payload[1] - '0' != _bandnummer)  //richtige Bandnummer?
		return false;
	if (payload[2] == 'H')  //Helligkeit ganzes LEDband
	{
		//Serial.println("1");
		//payload[6] = 0;// String abschließen
		debugln((char*)payload);
		uint8_t helligkeit = atoi((char*)payload + 3);
		debugln(helligkeit);
		sethell(helligkeit);
	}
	else  //einzelnes Pixel
	{
		if (length < 10)
			return false;
		//Serial.println("2");
		uint8_t nr = (payload[2] - '0') * 10 + payload[3] - '0';
		//Serial.println(nr);
		payload[length] = 0x00; //String abschließen!
		uint32_t wrgb = strtoul((char*)&payload[4], 0, 16);
		//Serial.println(wrgb);
		setWRGB(nr, wrgb);
	}







	return true;
}





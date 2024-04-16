#include "canhardMCP2515.h"


uint8_t CAN_RINGBUFF::buffctrwrite;
uint8_t CAN_RINGBUFF::buffctrread;
mycan_frame CAN_RINGBUFF::internalbuffer[CAN_RINGBUFF::bufflen];
Adafruit_MCP2515 * CAN_RINGBUFF::_mcp = 0;

int8_t canhardMCP2515::send(const long id, const uint8_t * buffer8,uint8_t size)
{
 
	debugln("sende");
	mcp.beginPacket(id);
	mcp.write(buffer8,size);	
	return mcp.endPacket();
	

}

canhardMCP2515::canhardMCP2515(uint8_t CS_RXpin, uint8_t IRQ_TXpin, long geschwindigkeit) : mcp(CS_RXpin),mybuf(&mcp)
{
	//lt Wiki sollten dies Geschwindigkeiten passen:
	//125 kbits / s	500 m
	//250 kbits / s	250 m
	//500 kbits / s	125 m
	// theoretisch könnte ich bis 500kbit/s hochdrehen

	if (!mcp.begin(geschwindigkeit))
	{
		Serial.println("Error initializing MCP2515.");
		while(1) delay(10);
	}
	debugln("MCP2515 chip found");
  
	
	 
	mcp.onReceive(IRQ_TXpin,mybuf.onReceive);
	  
}


 
bool canhardMCP2515::hasgetmsg(mycan_frame* newframe)
{
	 
	
	if (mybuf.hasmsg())
	{
		debugln("true");
		//newframe = mybuf.getactmsg();
		
		
		mycan_frame* tmpfr;
		tmpfr =  mybuf.getactmsg();
		
		
		
		newframe->id = tmpfr->id;
		for (int i = 0; i < 8; i++) {
			newframe->data[i] = tmpfr->data[i];
			debug("i: "); debug(i); debug("\t"); debug(newframe->data[i]); debug("\t"); debugln((char)newframe->data[i]);
		}
		
		
		
		
		return true;
	}
	 
	return false;
	
}































CAN_RINGBUFF::CAN_RINGBUFF(Adafruit_MCP2515 * mcp)
{
	buffctrwrite = 0;
	buffctrread = 0;
	_mcp = mcp;
}

void CAN_RINGBUFF::onReceive(int packetSize)
{
 
	debugln("Interrupt");
	
	if (_mcp->packetExtended()) {
		debugln("Extended not used");
		return; // nutze ich nicht
	}
	if (_mcp->packetRtr()) {
		debugln("Rtr not used");
		return; // nutze ich nicht
	}

	long id = _mcp->packetId();

	debug("packet with id 0x");
	debug(id);

	debug(" and length ");
	debugln(packetSize);
	for (uint8_t z = 0; z < 8; z++)
	{
		if (_mcp->available()) {
			char tmp = (char)_mcp->read();
			debug(tmp);	debug("|");	debug((uint8_t)tmp); debug("   ");

			internalbuffer[buffctrwrite].data[z] = (uint8_t)tmp;

		}
	}
	internalbuffer[buffctrwrite].id = id;
	//Serial.println("");
	internalbuffer[buffctrwrite].ready = true;
	buffctrwrite++;
	if (buffctrwrite >= bufflen) buffctrwrite = 0;

}
bool CAN_RINGBUFF::hasmsg()
{

	return internalbuffer[buffctrread].ready;
};
mycan_frame* CAN_RINGBUFF::getactmsg()  // achtung kracht, wenn vorher nicht mit hasmsg abgeprüft!!!!
{
	mycan_frame* tmp = NULL;
	if (hasmsg())
	{
		tmp = &internalbuffer[buffctrread];
	}
	internalbuffer[buffctrread].ready = false;
	buffctrread++;
	if (buffctrread >= bufflen) buffctrread = 0;

	return tmp;
};




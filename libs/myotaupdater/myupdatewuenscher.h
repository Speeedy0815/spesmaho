#pragma once

class updatewuenscher
{
private:
	bool updatewunsch = false;
public:
	void wuenscheupdate() {  updatewunsch = true; };
	bool isupdatewunsch() { return (updatewunsch); };
};
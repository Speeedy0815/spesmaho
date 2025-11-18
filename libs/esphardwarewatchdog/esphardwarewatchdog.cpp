#include "esphardwarewatchdog.h"






EspHwWTD::EspHwWTD()
{
	Serial.println("ESP32 Watchdog Beispiel startet...");
	esp_err_t res = esp_task_wdt_init(WDT_TIMEOUT_S, true);
    if (res == ESP_OK) {
        Serial.printf("Watchdog mit %d s Timeout initialisiert\n", WDT_TIMEOUT_S);
    } else {
        Serial.printf("Fehler bei esp_task_wdt_init: %d\n", (int)res);
    }
	esp_task_wdt_add(NULL);
}



void EspHwWTD::update()
{
	esp_task_wdt_reset(); 
}
bool EspHwWTD::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	return false;
}




 
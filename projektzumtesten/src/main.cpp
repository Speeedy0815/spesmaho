#include <Arduino.h>
#include <esp_task_wdt.h>

// Watchdog-Timeout in Sekunden (max. offiziell 60)
#define WDT_TIMEOUT_S 5

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    Serial.println("ESP32 Watchdog Beispiel startet...");

    // Hardware-Task-Watchdog initialisieren:
    // 1. Parameter: Timeout in Sekunden (1..60)
    // 2. Parameter: panic = true -> bei Timeout Reset mit Guru Meditation
    esp_err_t res = esp_task_wdt_init(WDT_TIMEOUT_S, true);
    if (res == ESP_OK) {
        Serial.printf("Watchdog mit %d s Timeout initialisiert\n", WDT_TIMEOUT_S);
    } else {
        Serial.printf("Fehler bei esp_task_wdt_init: %d\n", (int)res);
    }

    // Aktuellen Task (loopTask) dem WDT hinzufügen
    // NULL bedeutet: „aktueller Task“
    esp_task_wdt_add(NULL);
}

void loop() {
    // --- Normaler Betrieb: regelmäßig füttern ---
    Serial.println("Loop lebt, fuettere Watchdog...");
    esp_task_wdt_reset();   // WDT füttern
    delay(1000);            // irgendwas tun

    // --- Test: absichtlich den WDT auslösen ---
    // zum Testen: diesen Block UN-kommentieren
    
    Serial.println("Jetzt blockieren wir laenger als der Timeout...");
    // länger warten als der eingestellte Timeout (z.B. 5 s + Reserve)
    delay((WDT_TIMEOUT_S + 2) * 1000);
    // Kein esp_task_wdt_reset() -> der ESP32 sollte nun automatisch resetten
    
}
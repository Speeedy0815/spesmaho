#include "SmartHomeSettings.h"

// Konstruktor

void SmartHomeSettings::update()
{
}


bool SmartHomeSettings::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
	debugln(F("SmartHomeSettings"));
	debugln((char*)payload);
	if (length != 3)
	{ // mindestens X + M + HHH + AAA
        debugln("Payload passt nicht kurz");
        return false;
    }


    
    // Mode extrahieren
    if (payload[0] != 'D')
	{
		return false;
	}	
    if (payload[1] != 'E')
	{
		return false;
	}
    if (payload[2] != 'L')
	{
		return false;
	}	
	deleteSettings();
    return true;
}



SmartHomeSettings::SmartHomeSettings(const char* apPrefix,
                                     const char* settingsFile)
  : _server(80),
    _apPrefix(apPrefix),
    _settingsFile(settingsFile),
    _configured(false)
{
    // Alle Arrays in _settings mit 0 füllen -> gültige leere C-Strings
    memset(&_settings, 0, sizeof(_settings));

    init();
}
void SmartHomeSettings::deleteSettings() {
    Serial.println(F("Loesche Settings-Datei..."));

    if (SPIFFS.exists(_settingsFile)) {
        if (SPIFFS.remove(_settingsFile)) {
            Serial.println(F("Settings-Datei erfolgreich geloescht."));
        } else {
            Serial.println(F("Fehler beim Loeschen der Settings-Datei."));
        }
    } else {
        Serial.println(F("nichts zu loeschen."));
    }

    Serial.println(F("Neustart in 1s..."));
    delay(1000);
    ESP.restart();
}
void SmartHomeSettings::init() {
    Serial.println();
    Serial.println(F("Init ====="));

    if (!mountSpiffs()) {
        Serial.println(F("nicht persistent gespeichert werden."));
    }

    if (loadSettings()) {
        _configured = true;
        Serial.println(F("Settings gefunden."));
        printSettingsToSerial();
        Serial.println(F("Init fertig (configured) ====="));
    } else {
        Serial.println(F("starte Konfigurationsportal."));
        startConfigPortal();
        // startConfigPortal blockiert und endet typischerweise in ESP.restart()
    }
}

bool SmartHomeSettings::mountSpiffs() {
    Serial.println(F("SPIFFS mounten..."));
    if (!SPIFFS.begin(false)) {
        Serial.println(F("versuche Formatierung..."));
        if (!SPIFFS.begin(true)) {
            Serial.println(F("nicht formatiert werden!"));
            return false;
        }
        Serial.println(F("erfolgreich gemountet."));
        return true;
    }
    Serial.println(F("SPIFFS gemountet."));
    return true;
}

bool SmartHomeSettings::settingsValid(const Settings& s) const {
    if (strlen(s.wifiSsid)       == 0) return false;
    if (strlen(s.wifiPassword)   == 0) return false;
    if (strlen(s.mqttPassword)   == 0) return false;
    if (strlen(s.mqttUser)       == 0) return false;
    if (strlen(s.mqttServeraddr) == 0) return false;
    if (strlen(s.mqttName)       == 0) return false;
    if (s.canAddr == 0)                return false; // 0 = ungültig

    return true;
}

void SmartHomeSettings::printSettingsToSerial() const {
    Serial.println(F("----- SmartHomeSettings: Aktuelle Settings -----"));
    Serial.print(F("wifiSsid: "));       Serial.println(_settings.wifiSsid);
    Serial.print(F("wifiPassword: "));   Serial.println(_settings.wifiPassword);
    Serial.print(F("mqttUser: "));       Serial.println(_settings.mqttUser);
    Serial.print(F("mqttPassword: "));   Serial.println(_settings.mqttPassword);
    Serial.print(F("mqttServeraddr: ")); Serial.println(_settings.mqttServeraddr);
    Serial.print(F("mqttName: "));       Serial.println(_settings.mqttName);
    Serial.print(F("canAddr: "));        Serial.println(_settings.canAddr);
    Serial.println(F("------------------------------------------------"));
}

bool SmartHomeSettings::loadSettings() {
    Serial.println(F("[SmartHomeSettings] Lade Settings aus SPIFFS..."));

    if (!SPIFFS.exists(_settingsFile)) {
        Serial.println(F("existiert nicht."));
        return false;
    }

    File file = SPIFFS.open(_settingsFile, "r");
    if (!file) {
        Serial.println(F("konnte nicht geoeffnet werden."));
        return false;
    }

    Settings tmp = {};  // alles auf 0 setzen (leere C-Strings)
    String key, value;

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();
        if (line.length() == 0) continue;
        if (line.startsWith("#")) continue;

        int idx = line.indexOf('=');
        if (idx < 0) continue;

        key   = line.substring(0, idx);
        value = line.substring(idx + 1);
        key.trim();
        value.trim();

        if      (key == "wifiSsid") {
            value.toCharArray(tmp.wifiSsid, sizeof(tmp.wifiSsid)); // max 29 + '\0'
        }
        else if (key == "wifiPassword") {
            value.toCharArray(tmp.wifiPassword, sizeof(tmp.wifiPassword));
        }
        else if (key == "mqttPassword") {
            value.toCharArray(tmp.mqttPassword, sizeof(tmp.mqttPassword));
        }
        else if (key == "mqttUser") {
            value.toCharArray(tmp.mqttUser, sizeof(tmp.mqttUser));
        }
        else if (key == "mqttServeraddr") {
            value.toCharArray(tmp.mqttServeraddr, sizeof(tmp.mqttServeraddr));
        }
        else if (key == "mqttName") {
            value.toCharArray(tmp.mqttName, sizeof(tmp.mqttName));
        }
        else if (key == "canAddr") {
            tmp.canAddr = (uint8_t)value.toInt();
        }
    }

    file.close();

    if (!settingsValid(tmp)) {
        Serial.println(F("unvollstaendig oder ungueltig."));
        return false;
    }

    _settings = tmp;
    Serial.println(F("Einstellungen erfolgreich geladen."));
    return true;
}

bool SmartHomeSettings::saveSettings() {
    Serial.println(F("Speichere Settings nach SPIFFS..."));

    File file = SPIFFS.open(_settingsFile, "w");
    if (!file) {
        Serial.println(F("nicht zum Schreiben geoeffnet werden."));
        return false;
    }

    file.printf("wifiSsid=%s\n",        _settings.wifiSsid);
    file.printf("wifiPassword=%s\n",    _settings.wifiPassword);
    file.printf("mqttPassword=%s\n",    _settings.mqttPassword);
    file.printf("mqttUser=%s\n",        _settings.mqttUser);
    file.printf("mqttServeraddr=%s\n",  _settings.mqttServeraddr);
    file.printf("mqttName=%s\n",        _settings.mqttName);
    file.printf("canAddr=%u\n",         _settings.canAddr);

    file.close();
    Serial.println(F("Speichern abgeschlossen."));
    printSettingsToSerial();
    return true;
}

void SmartHomeSettings::startConfigPortal() {
    Serial.println(F("Starte Konfigurationsportal..."));

    WiFi.mode(WIFI_AP);

    // Zufällige SSID basierend auf AP-Prefix
    uint32_t rnd = esp_random();
    uint16_t suffix = rnd % 10000; // 0..9999
    char ssid[32];
    snprintf(ssid, sizeof(ssid), "%s%04u", _apPrefix.c_str(), suffix);

    Serial.print(F("Starte AP mit SSID: "));
    Serial.println(ssid);

    bool apOk = WiFi.softAP(ssid); // offen, ohne Passwort
 
    if (!apOk) {
        Serial.println(F("Konnte Access Point nicht starten!"));
    } else {
        Serial.print(F("AP IP-Adresse: "));
        Serial.println(WiFi.softAPIP());
        Serial.println(F("http://192.168.4.1 auf."));
    }

    // HTTP-Handler registrieren
    using namespace std::placeholders;
    _server.on("/", HTTP_GET,  std::bind(&SmartHomeSettings::handleRoot, this));
    _server.on("/save", HTTP_POST, std::bind(&SmartHomeSettings::handleSave, this));
    _server.onNotFound(std::bind(&SmartHomeSettings::handleNotFound, this));

    _server.begin();
    Serial.println(F("Webserver gestartet."));

    // Blockierende Schleife: bleibt im Konfigurationsmodus
    while (true) {
        _server.handleClient();
        delay(10);
    }
}

void SmartHomeSettings::handleRoot() {
    Serial.println(F("[Web] GET /"));

    String html = F(
        "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
        "<title>SmartHome Settings</title>"
        "<meta name='viewport' content='width=device-width,initial-scale=1'>"
        "<style>body{font-family:sans-serif;margin:20px;}label{display:block;margin-top:10px;}"
        "input[type=text],input[type=password],input[type=number]{width:100%;padding:8px;box-sizing:border-box;}"
        "button{margin-top:20px;padding:10px 15px;font-size:16px;}</style>"
        "</head><body>"
        "<h1>SmartHome Einstellungen</h1>"
        "<form method='POST' action='/save'>"
    );

    String maxLen = String(MAX_STR_LEN - 1); // 29

    html += "WiFi SSID:<br><input type='text' maxlength='" + maxLen +
            "' name='wifiSsid' value='" + String(_settings.wifiSsid) + "'><br>";
    html += "WiFi Passwort:<br><input type='password' maxlength='" + maxLen +
            "' name='wifiPassword' value='" + String(_settings.wifiPassword) + "'><br>";
    html += "MQTT Benutzer:<br><input type='text' maxlength='" + maxLen +
            "' name='mqttUser' value='" + String(_settings.mqttUser) + "'><br>";
    html += "MQTT Passwort:<br><input type='password' maxlength='" + maxLen +
            "' name='mqttPassword' value='" + String(_settings.mqttPassword) + "'><br>";
    html += "MQTT Server Adresse:<br><input type='text' maxlength='" + maxLen +
            "' name='mqttServeraddr' value='" + String(_settings.mqttServeraddr) + "'><br>";
    html += "MQTT Name (ClientID o.ä.):<br><input type='text' maxlength='" + maxLen +
            "' name='mqttName' value='" + String(_settings.mqttName) + "'><br>";
    html += "CAN Adresse:<br><input type='number' min='1' max='255' name='canAddr' value='" +
            String(_settings.canAddr) + "'><br>";

    html += "<button type='submit'>Speichern und Neustart</button>";
    html += "</form></body></html>";

    _server.send(200, "text/html", html);
}
 
void SmartHomeSettings::handleSave() {
    Serial.println(F("[SmartHomeSettings][Web] POST /save"));

    Settings tmp = {};  // alles auf 0

    String value;

    value = _server.arg("wifiSsid");
    value.toCharArray(tmp.wifiSsid, sizeof(tmp.wifiSsid));

    value = _server.arg("wifiPassword");
    value.toCharArray(tmp.wifiPassword, sizeof(tmp.wifiPassword));

    value = _server.arg("mqttUser");
    value.toCharArray(tmp.mqttUser, sizeof(tmp.mqttUser));

    value = _server.arg("mqttPassword");
    value.toCharArray(tmp.mqttPassword, sizeof(tmp.mqttPassword));

    value = _server.arg("mqttServeraddr");
    value.toCharArray(tmp.mqttServeraddr, sizeof(tmp.mqttServeraddr));

    value = _server.arg("mqttName");
    value.toCharArray(tmp.mqttName, sizeof(tmp.mqttName));

    tmp.canAddr = (uint8_t)_server.arg("canAddr").toInt();

    if (!settingsValid(tmp)) {
        Serial.println(F("Ungueltige Einstellungen erhalten, nichts gespeichert."));
        _server.send(400, "text/plain", "Fehler: Ungueltige oder unvollstaendige Eingaben.");
        return;
    }

    _settings = tmp;

    if (!saveSettings()) {
        _server.send(500, "text/plain", "Fehler beim Speichern der Einstellungen.");
        return;
    }

    _server.send(200, "text/html",
        "<html><body><h1>Gespeichert</h1>"
        "<p>Die Einstellungen wurden gespeichert. Der ESP32 startet neu...</p>"
        "</body></html>"
    );

    Serial.println(F("[SmartHomeSettings] Neustart in 1s..."));
    delay(1000);
    ESP.restart();
}
void SmartHomeSettings::handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += _server.uri();
    message += "\n";
    _server.send(404, "text/plain", message);
}
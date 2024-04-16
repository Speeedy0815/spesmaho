#pragma once 
#ifndef VERSIONHEADER
#define VERSIONHEADER

const double GLOBALVERSION = 1.59;


/*
XXXX BEGIN
1.59	DMX (das einfache auf den Arduinos) initialisiert die ersten 11 Devices auf 0
1.58	Can optimiert und auf Adafruit-Bibliothek umgebaut Displays müüssen jetzt auch Kommunikation zum Server haben
1.57	Onewire optimiert
1.56	Resetzeit biem Watchdog einstellbar + Displays resetten sich alle 5min
1.55	Geschwindigkeit der Ein und Ausgänge start optimiert, Refresh nur noch alle 10/60 Sekunden (Hardwarezugriff)
1.55	Zeiten im Output gefixt + Alles aus etwas (nicht viel) beschleunigt
1.54	extra CanBibliothek für den Flur repariert, nach dem letzten Umbau war es kaputt
1.54	Hardwarewatchdog für die Arduinos herausgelöst, dass ich es vor MQTT strten kann.... Ethernet hängt sich auf, wenn kein DNS verfügbar ist
1.53	Displaymodul für Artist Titel und so
1.52	Umbau Display (modularer + CS auswählbar) und erste Inbetriebnahmen Audioboard
1.51	Temperatursensow wieder 1.3° Offset rein genommen --> die neue Bibliothek war nicht besser (an der Stelle)
1.50	neue Can Bibliothek eingebaut +  Update ESP Tools
1.482	OTA Updater vereinfacht
1.481	Resetter vereinfacht
1.48	CAN Geschwindigkeit hoch genommen
1.47	Timeout in ESPCan eingebaut und wieder alle Protokolle in IR rein genommen
1.46	IR  Rollback zu 1.44 (gab Abbrüche)
1.45	ESP Echtzeituhr verwendet + ESP kann jetzt alle IR Signale dekodieren (Arduinos nur eine Auswahl)
1.44	IR volkommen überarbeitet
1.43	intern: Debugging eingebaut
1.42	DMX hinzugefügt
1.41	Warntext optional mit Display anschalten oder nicht
1.40	Umbau CAN mit Timeout rückgängig gemacht
1.39	Umbau auf Interfacesammler, dass das Hauptprogramm einfacher wird
1.38	Umbau CAN mit Timeout + Canadresse wird im Flash abgelegt
1.37	kleiner Fix Farben Text Display
1.36	Display kann Text auf Button anzeigen
1.32	neue Bibliothek für CAN beim ESP, nicht mehr alte verwenden. Ist Mist
1.31	intern Umbau CAN --> Hardware abstrahiert
1.30	Update und Reset versteckt auf Display (links oben rechts unten)
1.22	Bewegungsmelder gedreht	 
1.21	Serielle Schnittstelle Eingabe auch bei anderen Kommunikationsmodulen aktiviert
1.20	OTA aktiviert  --> Über MQTT UPD schicken, dann wird  Port geöffnet
1.13	Temperatursensor wird auf Plausibilität überprüft, ggf. wird der I2C neu initialisiert
1.12	Schwellwert für Helligkeit hoch genommen
1.11	Helligkeit in den WS2812LES eingebaut
1.10	CAN hängt sich an den ESP's auf  --> behoben
1.01	Versionierung sollte jetzt auch funktionieren
1.00	Versionierung eingebaut + Taster refreshen sich
XXXX END
*/

#endif // !VERSIONHEADER

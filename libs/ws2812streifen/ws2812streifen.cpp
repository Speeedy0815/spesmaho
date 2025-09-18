#include "ws2812streifen.h"


Ws2812streifen::Ws2812streifen(uint8_t bandnummer, uint32_t Pixelanzahl,
                                     uint8_t pin, uint8_t maxbrightness, bool store)
:   
  pixels(Pixelanzahl, pin, NEO_GRB + NEO_KHZ800)
{
	_bandnummer = bandnummer;
	_store = store;
	if (_store)
	{

		data = new uint32_t[Pixelanzahl];

		for (int i = 0; i < Pixelanzahl; i++)
		{

			data[i] = 0;
		}
	}

	pixels.begin();
	for (int i = 0; i < Pixelanzahl; i++) { pixels.setPixelColor(i, pixels.Color(0, 0, 0)); }
	sethell(maxbrightness);
	pixels.show();
	  animationsupdatetimer.start(animationsupdatetimerfreshzeit, true);

}
Ws2812streifen::~Ws2812streifen()
{
	if (_store)
	{
		delete[] data;
	}
}
void Ws2812streifen::sethell(uint8_t helligkeit)
{

	pixels.setBrightness(helligkeit);
	if (_store)
	{
		for (int i = 0; i < pixels.numPixels(); i++)
		{

			pixels.setPixelColor(i, data[i]);
		}

	}
	pixels.show();
}

void Ws2812streifen::update()
{

	if (_mode == 'A') {
       
        if (animationsupdatetimer.update_darfich()) {
            animiere();
        }
    }
	return;
}

void Ws2812streifen::setWRGB(uint8_t nr, uint32_t wrgb)
{

	if (_store)
	{
		data[nr] = wrgb;
	}
	pixels.setPixelColor(nr, wrgb);
	pixels.show();
}



bool Ws2812streifen::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
    if (length < 4) return false;
    if (length > 20) return false;   // etwas erhöht wegen Animationsstring
    if (payload[0] != 'L') return false;
    if (payload[1] - '0' != _bandnummer) return false; // richtige Bandnummer?

    debug("topic ");
    debugln(topic);
    debug("payload ");
    debugln((char*)payload);
    debug("length ");
    debugln(length);

    if (payload[2] == 'H')  // Helligkeit ganzes LED-Band
    {
        uint8_t helligkeit = atoi((char*)payload + 3);
        debug("Helligkeit: "); debugln(helligkeit);
        sethell(helligkeit);
    }
    else if (payload[2] == 'A')  // Animationsmodus
    {
        _mode = 'A';

        // String terminieren für sichere Verarbeitung
        payload[length] = 0x00;

        // Animationsnummer (2 Stellen, ASCII Ziffern)
        _animationsnummer = atoi((char*)payload + 3);

        // Farbe ab Position 5 (nach L,K,A,ZZ)
        if (length >= 9) {
            _animationsfarbe = strtoul((char*)payload + 5, 0, 16);
        }

        debug("Animationsmodus: Nummer ");
        debug(_animationsnummer);
        debug(" Farbe 0x");
        debugln(_animationsfarbe);
    }
    else  // einzelnes Pixel
    {
        if (length < 10) return false;
        _mode = 'L';

        payload[length] = 0x00; // String abschließen

        uint8_t nr = (payload[2] - '0') * 10 + payload[3] - '0';
        uint32_t wrgb = strtoul((char*)&payload[4], 0, 16);

        setWRGB(nr, wrgb);
    }

    return true;
}

void Ws2812streifen::animiere() {
    const uint8_t MIN_BRIGHT = 30;     // dunkelste Stelle im Fleck
    const uint8_t MAX_BRIGHT = 255;    // Grundhelligkeit
    const int FLECK_BREITE = 10;       // Breite des Flecks (in LEDs)

    static int step = 0;               // Mittelpunkt des Flecks
    static int tick = 0;               // Zähler für langsameres Weiterlaufen
    int numPixels = pixels.numPixels();

    // Grundfarbe zerlegen
    uint8_t baseR = (_animationsfarbe >> 16) & 0xFF;
    uint8_t baseG = (_animationsfarbe >> 8) & 0xFF;
    uint8_t baseB = (_animationsfarbe) & 0xFF;

    for (int i = 0; i < numPixels; i++) {
        // Abstand im Ring
        int dist = (i - step + numPixels) % numPixels;
        if (dist > numPixels / 2) dist = numPixels - dist;

        int brightness = MAX_BRIGHT;

        if (dist < FLECK_BREITE) {
            // Quadratische Kurve für weichen Übergang
            int x = dist * 255 / FLECK_BREITE;  // normiert 0..255
            int wave = (x * x) >> 8;            // 0..255, weich
            brightness = MIN_BRIGHT + ((MAX_BRIGHT - MIN_BRIGHT) * wave) / 255;
        }

        // Farbe skalieren
        uint8_t r = (baseR * brightness) >> 8;
        uint8_t g = (baseG * brightness) >> 8;
        uint8_t b = (baseB * brightness) >> 8;

        pixels.setPixelColor(i, pixels.Color(r, g, b));
    }

    pixels.show();

    // Schritt nur alle 10 Aufrufe weiterschieben
    tick++;
    if (tick >= 10) {     // 10× 10 ms = 100 ms wie vorher
        step = (step + 1) % numPixels;
        tick = 0;
    }
}
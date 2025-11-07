#define MYDEBUG 0
#include "mydebug.h"

#pragma once
#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "basisinterface.h"
#include "mytimer.h"
#include <FastLED.h>


template <uint8_t DATA_PIN>
class Ws2812streifen : public BasisInterface
{
public:
    // bandnummer: nur dein eigenes Feld
    // Pixelanzahl: Anzahl LEDs
    // pin: Datenpin
    // maxbrightness: 0..255
    // store: wie bei dir
    Ws2812streifen(uint8_t bandnummer,
                   uint32_t Pixelanzahl,
                  
                   uint8_t maxbrightness,
                   bool store);
    ~Ws2812streifen();

private:
    uint8_t  _bandnummer = 0;
    uint32_t _animationsnummer = 0;
    uint32_t _animationsfarbe = 0;
    uint32_t* data = nullptr;
    bool _store = false;

    // FastLED-spezifisch
    CRGB*   _leds = nullptr;
    uint32_t _numLeds = 0;
   
    uint8_t  _maxbrightness = 255;

    myTimer  animationsupdatetimer;
    uint16_t animationsupdatetimerfreshzeit = 10; // alle 10 ms

    void setWRGB(uint16_t nr, uint32_t wrgb);
    void sethell(uint8_t helligkeit);
    void animiere();
    char _mode = 'A';
    void update();
    bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
};



template <uint8_t DATA_PIN>
Ws2812streifen<DATA_PIN>::Ws2812streifen(uint8_t bandnummer,
                               uint32_t Pixelanzahl,
                              
                               uint8_t maxbrightness,
                               bool store)
: _bandnummer(bandnummer),
  _animationsnummer(0),
  _animationsfarbe(0),
  _store(store),
  _numLeds(Pixelanzahl),

  _maxbrightness(maxbrightness)
{
    // Speicher für LEDs anlegen
    _leds = new CRGB[_numLeds];

    	if (_store)
	{

		data = new uint32_t[Pixelanzahl];

		for (int i = 0; i < Pixelanzahl; i++)
		{

			data[i] = 0;
		}
	}


    // alles schwarz initialisieren
    for (uint32_t i = 0; i < _numLeds; i++)
        _leds[i] = CRGB::Black;

    // FastLED setup
    // NEOPIXEL: WS2812/WS2812B mit GRB
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(_leds, Pixelanzahl);  // pin wird gleich gefixt
    // FastLED.addLeds erlaubt keinen Laufzeit-pin in der Template-Variante,
    // deshalb machen wir das unten noch einmal dynamisch, falls du einen
    // anderen LED-Typ nutzt. Für WS2812 geht das i.d.R. so:
    FastLED.clear(true);
    sethell(_maxbrightness);

    // Wenn du unbedingt den Laufzeit-Pin brauchst:
    // FastLED.addLeds<WS2812B, pin, GRB>(_leds, _numLeds);  // dafür pin statt 6
    // -> dann bitte oben den festen 6er rausnehmen

    animationsupdatetimer.start(animationsupdatetimerfreshzeit, true);
}

template <uint8_t DATA_PIN>
Ws2812streifen<DATA_PIN>::~Ws2812streifen()
{
	if (_store)
	{
		delete[] data;
	}
    if (_leds) {
        delete[] _leds;
        _leds = nullptr;
    }
}
template <uint8_t DATA_PIN>
void Ws2812streifen<DATA_PIN>::sethell(uint8_t helligkeit)
{
    FastLED.setBrightness(helligkeit);
	 
	if (_store)
	{
		for (int i = 0; i < _numLeds; i++)
		{
            	uint8_t r = (data[i] >> 16) & 0xFF;
                uint8_t g = (data[i] >> 8)  & 0xFF;
                uint8_t b = (data[i])       & 0xFF;
                _leds[i].setRGB(r, g, b);
		}

	}
	FastLED.show();
}

template <uint8_t DATA_PIN>
void Ws2812streifen<DATA_PIN>::update()
{

	if (_mode == 'A') {
       
        if (animationsupdatetimer.update_darfich()) {
            animiere();
        }
    }
	return;
}

template <uint8_t DATA_PIN>
void Ws2812streifen<DATA_PIN>::setWRGB(uint16_t nr, uint32_t wrgb)
{

    if (nr >= _numLeds) return;

	if (_store)
	{
		data[nr] = wrgb;
	}
	uint8_t r = (wrgb >> 16) & 0xFF;
    uint8_t g = (wrgb >> 8)  & 0xFF;
    uint8_t b = (wrgb)       & 0xFF;

    _leds[nr].setRGB(r, g, b);
	FastLED.show();
}


template <uint8_t DATA_PIN>
bool Ws2812streifen<DATA_PIN>::callbackismineanddo(char* topic, byte* payload, unsigned int length)
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
template <uint8_t DATA_PIN>
void Ws2812streifen<DATA_PIN>::animiere() {
    const uint8_t MIN_BRIGHT = 30;     // dunkelste Stelle im Fleck
    const uint8_t MAX_BRIGHT = 255;    // Grundhelligkeit
    const int FLECK_BREITE = 10;       // Breite des Flecks (in LEDs)

    static int step = 0;               // Mittelpunkt des Flecks
    static int tick = 0;               // Zähler für langsameres Weiterlaufen
    int numPixels =  _numLeds;

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



 

        _leds[i].setRGB(r, g, b);
	
 
    }

    
    FastLED.show();
    // Schritt nur alle 10 Aufrufe weiterschieben
    tick++;
    if (tick >= 10) {     // 10× 10 ms = 100 ms wie vorher
        step = (step + 1) % numPixels;
        tick = 0;
    }
}
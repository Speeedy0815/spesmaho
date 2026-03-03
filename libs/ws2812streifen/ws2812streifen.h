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
    void animiereEule();
    void animiereWeihnachtsstern();
    char _mode = 'A';
    void update();
    bool callbackismineanddo(char* topic, byte* payload, unsigned int length);
    void setCircleLevel(uint8_t value);
 
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

            switch (_animationsnummer) {
                case 0:  animiereEule(); break;
                case 1:  animiereWeihnachtsstern(); break;
                default: animiereEule(); break;
            }
        }
    }
    return;
}



template <uint8_t DATA_PIN>
void Ws2812streifen<DATA_PIN>::animiereWeihnachtsstern()
{
    if (_numLeds == 0) return;

    static uint16_t t = 0; // Zeit

    uint8_t br = (_animationsfarbe >> 16) & 0xFF;
    uint8_t bg = (_animationsfarbe >> 8)  & 0xFF;
    uint8_t bb = (_animationsfarbe)       & 0xFF;
    CHSV baseHsv = rgb2hsv_approximate(CRGB(br, bg, bb));
    uint8_t hue0 = baseHsv.h;

    uint8_t pulse   = sin8((uint8_t)(t >> 1));  // 0..255
    uint8_t globalV = 120 + (pulse >> 1);       // 120..247

    for (uint16_t i = 0; i < _numLeds; i++) {

        uint16_t segPos = (uint32_t)i * 5UL * 256UL / _numLeds;
        uint8_t within  = segPos & 0xFF;

        uint8_t dist = (within > 128) ? (within - 128) : (128 - within);

        uint8_t spikeOuter = qsub8(255, dist * 3);
        uint8_t spikeInner = qsub8(255, dist * 1);

        uint8_t glow  = scale8(spikeInner, 180);
        uint8_t spike = (spikeOuter > glow) ? spikeOuter : glow;

        uint8_t v = scale8(spike, globalV);

        uint8_t hue = hue0 + (uint8_t)((i * 255UL) / _numLeds) + (uint8_t)(t >> 2);

        CRGB c = CHSV(hue, 255, v);

        if (v > 150 && random8() < 6) {
            c += CRGB(60, 60, 60);
        }

        _leds[i] = c;

        if (_store) {
            data[i] = ((uint32_t)c.r << 16) | ((uint32_t)c.g << 8) | (uint32_t)c.b;
        }
    }

    FastLED.show();
    t++;
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
    else if (payload[2] == 'K')  // neuer statischer Kreis-Modus
    {
        _mode = 'K';

        payload[length] = 0x00;   // String terminieren
        int val = atoi((char*)payload + 3);
        if (val < 0)   val = 0;
        if (val > 255) val = 255;

        debug("Kreiswert: ");
        debugln(val);

        setCircleLevel((uint8_t)val);
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
void Ws2812streifen<DATA_PIN>::animiereEule() {
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



template <uint8_t DATA_PIN>
void Ws2812streifen<DATA_PIN>::setCircleLevel(uint8_t value)
{
    // physikalische Reihenfolge der LEDs auf dem "Kreis"
    //static const uint8_t ringOrder[9] = { 0, 1, 2, 3, 8, 4, 5, 6, 7 };
	static const uint8_t ringOrder[9] = { 7, 6, 5, 4, 8, 3, 2, 1, 0 };

    // Korrekturwerte pro LED (0..255). 255 = volle Helligkeit.
    // Diese Werte kannst du nach Bedarf anpassen.
    static const uint8_t ledCorrection[9] = {
        255, 50, 50, 255, 
        25,
         255, 50, 50, 255
    };

    const uint8_t ringCount = sizeof(ringOrder) / sizeof(ringOrder[0]);
    uint8_t num = (_numLeds < ringCount) ? _numLeds : ringCount;

    // alles erst mal aus
    for (uint32_t i = 0; i < _numLeds; ++i) {
        _leds[i] = CRGB::Black;
        if (_store) {
            data[i] = 0;
        }
    }

    if (value == 0 || num == 0) {
        FastLED.show();
        return;
    }

    if (value > 255) value = 255;

    // 0..255 auf 0..num LEDs abbilden
    // 255 -> alle LEDs voll an
    uint16_t total = (uint16_t)value * num; // 0 .. 255*num
    uint8_t full   = total / 255;          // voll leuchtende Segmente
    uint8_t rem    = total % 255;          // Rest -> Teil-Helligkeit

    const uint8_t baseValue = 255;         // Basishelligkeit weiß

    // voll leuchtende LEDs
    for (uint8_t seg = 0; seg < full && seg < num; ++seg) {
        uint8_t ledIndex = ringOrder[seg];
        if (ledIndex >= _numLeds) continue;

        uint8_t corr = ledCorrection[seg];
        uint8_t v    = (uint16_t)baseValue * corr / 255;

        _leds[ledIndex].setRGB(v, v, v);
        if (_store) {
            uint32_t wrgb = (uint32_t(v) << 16) | (uint32_t(v) << 8) | v;
            data[ledIndex] = wrgb;
        }
    }

    // letzte LED anteilig
    if (rem > 0 && full < num) {
        uint8_t seg      = full;
        uint8_t ledIndex = ringOrder[seg];
        if (ledIndex < _numLeds) {
            uint8_t corr = ledCorrection[seg];
            uint8_t v    = (uint32_t)baseValue * corr * rem / (255UL * 255UL);

            _leds[ledIndex].setRGB(v, v, v);
            if (_store) {
                uint32_t wrgb = (uint32_t(v) << 16) | (uint32_t(v) << 8) | v;
                data[ledIndex] = wrgb;
            }
        }
    }

    FastLED.show();
}


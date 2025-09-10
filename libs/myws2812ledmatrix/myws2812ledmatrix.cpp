#include "myws2812ledmatrix.h"

 
// 5x6 Font für ASCII 32..126
static const uint8_t font5x6_ascii[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // 32 ' '
    {0x00,0x00,0x5F,0x00,0x00}, // 33 '!'
    {0x00,0x07,0x00,0x07,0x00}, // 34 '"'
    {0x14,0x7F,0x14,0x7F,0x14}, // 35 '#'
    {0x24,0x2A,0x7F,0x2A,0x12}, // 36 '$'
    {0x23,0x13,0x08,0x64,0x62}, // 37 '%'
    {0x36,0x49,0x55,0x22,0x50}, // 38 '&'
    {0x00,0x05,0x03,0x00,0x00}, // 39 '''
    {0x00,0x1C,0x22,0x41,0x00}, // 40 '('
    {0x00,0x41,0x22,0x1C,0x00}, // 41 ')'
    {0x14,0x08,0x3E,0x08,0x14}, // 42 '*'
    {0x08,0x08,0x3E,0x08,0x08}, // 43 '+'
    {0x00,0x50,0x30,0x00,0x00}, // 44 ','
    {0x08,0x08,0x08,0x08,0x08}, // 45 '-'
    {0x00,0x60,0x60,0x00,0x00}, // 46 '.'
    {0x20,0x10,0x08,0x04,0x02}, // 47 '/'
    {0x3E,0x51,0x49,0x45,0x3E}, // 48 '0'
    {0x00,0x42,0x7F,0x40,0x00}, // 49 '1'
    {0x42,0x61,0x51,0x49,0x46}, // 50 '2'
    {0x21,0x41,0x45,0x4B,0x31}, // 51 '3'
    {0x18,0x14,0x12,0x7F,0x10}, // 52 '4'
    {0x27,0x45,0x45,0x45,0x39}, // 53 '5'
    {0x3C,0x4A,0x49,0x49,0x30}, // 54 '6'
    {0x01,0x71,0x09,0x05,0x03}, // 55 '7'
    {0x36,0x49,0x49,0x49,0x36}, // 56 '8'
    {0x06,0x49,0x49,0x29,0x1E}, // 57 '9'
    {0x00,0x36,0x36,0x00,0x00}, // 58 ':'
    {0x00,0x56,0x36,0x00,0x00}, // 59 ';'
    {0x08,0x14,0x22,0x41,0x00}, // 60 '<'
    {0x14,0x14,0x14,0x14,0x14}, // 61 '='
    {0x00,0x41,0x22,0x14,0x08}, // 62 '>'
    {0x02,0x01,0x51,0x09,0x06}, // 63 '?'
    {0x32,0x49,0x79,0x41,0x3E}, // 64 '@'
    {0x7E,0x11,0x11,0x11,0x7E}, // 65 'A'
    {0x7F,0x49,0x49,0x49,0x36}, // 66 'B'
    {0x3E,0x41,0x41,0x41,0x22}, // 67 'C'
    {0x7F,0x41,0x41,0x22,0x1C}, // 68 'D'
    {0x7F,0x49,0x49,0x49,0x41}, // 69 'E'
    {0x7F,0x09,0x09,0x09,0x01}, // 70 'F'
    {0x3E,0x41,0x49,0x49,0x7A}, // 71 'G'
    {0x7F,0x08,0x08,0x08,0x7F}, // 72 'H'
    {0x00,0x41,0x7F,0x41,0x00}, // 73 'I'
    {0x20,0x40,0x41,0x3F,0x01}, // 74 'J'
    {0x7F,0x08,0x14,0x22,0x41}, // 75 'K'
    {0x7F,0x40,0x40,0x40,0x40}, // 76 'L'
    {0x7F,0x02,0x04,0x02,0x7F}, // 77 'M'
    {0x7F,0x04,0x08,0x10,0x7F}, // 78 'N'
    {0x3E,0x41,0x41,0x41,0x3E}, // 79 'O'
    {0x7F,0x09,0x09,0x09,0x06}, // 80 'P'
    {0x3E,0x41,0x51,0x21,0x5E}, // 81 'Q'
    {0x7F,0x09,0x19,0x29,0x46}, // 82 'R'
    {0x46,0x49,0x49,0x49,0x31}, // 83 'S'
    {0x01,0x01,0x7F,0x01,0x01}, // 84 'T'
    {0x3F,0x40,0x40,0x40,0x3F}, // 85 'U'
    {0x1F,0x20,0x40,0x20,0x1F}, // 86 'V'
    {0x7F,0x20,0x18,0x20,0x7F}, // 87 'W'
    {0x63,0x14,0x08,0x14,0x63}, // 88 'X'
    {0x03,0x04,0x78,0x04,0x03}, // 89 'Y'
    {0x61,0x51,0x49,0x45,0x43}, // 90 'Z'
    {0x00,0x7F,0x41,0x41,0x00}, // 91 '['
    {0x02,0x04,0x08,0x10,0x20}, // 92 '\'
    {0x00,0x41,0x41,0x7F,0x00}, // 93 ']'
    {0x04,0x02,0x01,0x02,0x04}, // 94 '^'
    {0x40,0x40,0x40,0x40,0x40}, // 95 '_'
    {0x00,0x01,0x02,0x04,0x00}, // 96 '`'
    {0x20,0x54,0x54,0x54,0x78}, // 97 'a'
    {0x7F,0x48,0x44,0x44,0x38}, // 98 'b'
    {0x38,0x44,0x44,0x44,0x20}, // 99 'c'
    {0x38,0x44,0x44,0x48,0x7F}, // 100 'd'
    {0x38,0x54,0x54,0x54,0x18}, // 101 'e'
    {0x08,0x7E,0x09,0x01,0x02}, // 102 'f'
    {0x0C,0x52,0x52,0x52,0x3E}, // 103 'g'
    {0x7F,0x08,0x04,0x04,0x78}, // 104 'h'
    {0x00,0x44,0x7D,0x40,0x00}, // 105 'i'
    {0x20,0x40,0x44,0x3D,0x00}, // 106 'j'
    {0x7F,0x10,0x28,0x44,0x00}, // 107 'k'
    {0x00,0x41,0x7F,0x40,0x00}, // 108 'l'
    {0x7C,0x04,0x18,0x04,0x78}, // 109 'm'
    {0x7C,0x08,0x04,0x04,0x78}, // 110 'n'
    {0x38,0x44,0x44,0x44,0x38}, // 111 'o'
    {0x7C,0x14,0x14,0x14,0x08}, // 112 'p'
    {0x08,0x14,0x14,0x18,0x7C}, // 113 'q'
    {0x7C,0x08,0x04,0x04,0x08}, // 114 'r'
    {0x48,0x54,0x54,0x54,0x20}, // 115 's'
    {0x04,0x3F,0x44,0x40,0x20}, // 116 't'
    {0x3C,0x40,0x40,0x20,0x7C}, // 117 'u'
    {0x1C,0x20,0x40,0x20,0x1C}, // 118 'v'
    {0x3C,0x40,0x30,0x40,0x3C}, // 119 'w'
    {0x44,0x28,0x10,0x28,0x44}, // 120 'x'
    {0x0C,0x50,0x50,0x50,0x3C}, // 121 'y'
    {0x44,0x64,0x54,0x4C,0x44}, // 122 'z'
    {0x00,0x08,0x36,0x41,0x00}, // 123 '{'
    {0x00,0x00,0x7F,0x00,0x00}, // 124 '|'
    {0x00,0x41,0x36,0x08,0x00}, // 125 '}'
    {0x08,0x08,0x2A,0x1C,0x08}, // 126 '~'
};
 
void myLEDMatrix::drawText(const char* text)
{
    // Matrix löschen
    for (int i = 0; i < _Pixelanzahl; i++)
        pixels.setPixelColor(i, pixels.Color(0,0,0));

    uint8_t xOffset = 1;
    const uint8_t verticalOffset = 0; // 6 Pixel Font auf 8-Zeilen-Matrix

    for (const char* p = text; *p != '\0'; p++) {
        char c = *p;
        uint8_t index;

        if (c >= 32 && c <= 126) {
            index = c - 32; // ASCII 32..126
        } else {
            index = 0; // nicht druckbare Zeichen = Leerzeichen
        }

        for (uint8_t col = 0; col < 5; col++) {
            uint8_t colBits = font5x6_ascii[index][col];
            for (uint8_t row = 0; row < 7; row++) {
                if (colBits & (1 << row)) {
                    uint8_t y = row + verticalOffset;
                    if ((xOffset + col) < _width && y < _height) {
                        pixels.setPixelColor(xyToIndex(xOffset + col, y), pixels.Color(255,0,0));
                    }
                }
            }
        }

        xOffset += 6; // 5 Pixel + 1 Leer
        if (xOffset >= _width) break;
    }

    pixels.show();
}
void myLEDMatrix::drawTextDyn()
{
    // Matrix löschen
    for (int i = 0; i < _Pixelanzahl; i++)
        pixels.setPixelColor(i, pixels.Color(0,0,0));

    // Gesamtlänge des Textes in Pixeln
    int textLen = strlen(textBuffer);
    int textLengthPx = textLen * 6;
    if (textLengthPx <= 0) {
        pixels.show();
        return;
    }

    // Zykluslänge: kompletter Text + einmal Bildschirmbreite
    int cycleLength = textLengthPx + _width;

    // Scroll-Tick (Geschwindigkeit steuert ihr per Division)
    int scrollTick = (tickCounter / 2);
    int scrollPos = scrollTick % cycleLength;

    // Erkennen, ob ein kompletter Zyklus gerade zu Ende gegangen ist
    static int lastScrollPos = -1;
    bool cycleCompleted = false;
    if (lastScrollPos == -1) {
        // initialisieren (kein Decrement beim ersten Aufruf)
        lastScrollPos = scrollPos;
    } else {
        if (lastScrollPos != 0 && scrollPos == 0) {
            // Übergang in den neuen Zyklus -> vorheriger Durchlauf beendet
            cycleCompleted = true;
        }
        lastScrollPos = scrollPos;
    }

    // Wenn bereits 0, nicht weiter scrollen (Anzeige bleibt wie zuletzt)
    if (_anzahllauftext == 0) {
        // nichts weiter zeichnen -> letzte Anzeige bleibt stehen
        return;
    }

    // X-Offset berechnen (Text läuft von rechts nach links)
    int16_t xOffset = _width - scrollPos;
    const uint8_t verticalOffset = 0; // wie zuvor

    // Zeichen zeichnen
    for (uint8_t i = 0; textBuffer[i] != '\0'; i++) {
        char c = textBuffer[i];
        uint8_t index = (c >= 32 && c <= 126) ? (c - 32) : 0;

        for (uint8_t col = 0; col < 5; col++) {
            int16_t x = xOffset + (i * 6) + col;
            if (x >= -5 && x < _width) {
                uint8_t colBits = font5x6_ascii[index][col];
                for (uint8_t row = 0; row < 7; row++) {
                    if (colBits & (1 << row)) {
                        uint8_t y = row + verticalOffset;
                        if (x >= 0 && x < _width && y < _height) {
                            pixels.setPixelColor(xyToIndex(x, y), pixels.Color(255,0,0));
                        }
                    }
                }
            }
        }
    }

    pixels.show();

    // Nach dem Zeichnen die Durchlaufzählung einmalig erniedrigen (falls ein Durchlauf endete)
    if (cycleCompleted) {
        if (_anzahllauftext > 0) {
            _anzahllauftext--;
            debug("Durchlauf beendet, Rest: ");
            debugln(_anzahllauftext);
        }
    }
}

void myLEDMatrix::drawUhrzeit()
{
	drawText(_Uhrzeit->getTime("%R").c_str());

}




void myLEDMatrix::update()
{
    if (_mode == 'U') {
        // Uhr-Modus
        if (Uhrtimer.update_darfich()) {
            drawUhrzeit();
        }
    }
    else if (_mode == 'T') {
        // Statischer Text
        if (DyntextTimer.update_darfich()) {
            drawText(textBuffer);
        }
    }
    else if (_mode == 'D') {
        // Dynamischer Lauftext
        if (DyntextTimer.update_darfich()) {
            if (_anzahllauftext > 0) {
                tickCounter++;
                drawTextDyn();
            }
        }
    }
}

myLEDMatrix::myLEDMatrix(uint8_t pin, myUhrBasis* Uhrzeit):
pixels(_Pixelanzahl, pin, NEO_GRB + NEO_KHZ800)
 {
    // Konstruktor: initialisiert ggf. BasisInterface
 	memset(textBuffer, 0, sizeof(textBuffer)); // Füllt den Buffer mit Nullen
	strcpy(textBuffer, "      ");  // leeres Display darstellen





    debugln("myLEDMatrix Constructor called");
 	_Uhrzeit = Uhrzeit;
	pixels.begin();
	for (int i = 0; i < _Pixelanzahl; i++) { pixels.setPixelColor(i, pixels.Color(0, 0, 0)); }
    pixels.setBrightness(10);  //relativ dunkel stellen als Ausgangswert
	pixels.show();
    Uhrtimer.start(Uhrrefreshzeit, true);
	DyntextTimer.start(DynTextrefreshzeit, true);

}
uint16_t myLEDMatrix::xyToIndex(uint8_t x, uint8_t y)
{
    // x = Spalte (0.._width-1), y = Zeile (0.._height-1)
    if (x % 2 == 0) {
        // gerade Spalte: oben nach unten
        return x * _height + y;
    } else {
        // ungerade Spalte: unten nach oben
        return x * _height + (_height - 1 - y);
    }
}
 

bool myLEDMatrix::callbackismineanddo(char* topic, byte* payload, unsigned int length)
{
    if (length < 8) { // mindestens X + M + HHH + AAA
        debugln("Payload zu kurz");
        return false;
    }

    if (payload[0] != 'X') {
        debugln("Falsches Prefix, ignoriere");
        return false;
    }

    // Mode extrahieren
    _mode = payload[1];
    debug("Mode gesetzt auf: ");
    debugln(_mode);

    // Helligkeit (HHH, 3 ASCII-Zeichen)
    char bufH[4] = { payload[2], payload[3], payload[4], '\0' };
    int hellVal = atoi(bufH);
    if (hellVal < 0) hellVal = 0;
    if (hellVal > 255) hellVal = 255;
    pixels.setBrightness((uint8_t)hellVal);
    debug("Helligkeit gesetzt auf: ");
    debugln(hellVal);

    // Anzahl Lauftextdurchläufe (AAA, 3 ASCII-Zeichen)
    char bufA[4] = { payload[5], payload[6], payload[7], '\0' };
    int anzVal = atoi(bufA);
    if (anzVal < 0) anzVal = 0;
    if (anzVal > 100) anzVal = 100;
    _anzahllauftext = (uint8_t)anzVal;
    debug("Anzahl Lauftexte gesetzt auf: ");
    debugln(_anzahllauftext);

    // Text (ab Position 8 bis max 49 Zeichen)
    int textLen = length - 8;
    if (textLen > 99) textLen = 99;

    memset(textBuffer, 0, sizeof(textBuffer));
    strncpy(textBuffer, (char*)payload + 8, textLen);
    textBuffer[textLen] = '\0'; // Sicherheit

    debug("Text gesetzt: ");
    debugln(textBuffer);
	tickCounter = 1;
    return true;
}
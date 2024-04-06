/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6, SCK: 13, MOSI: 11, MISO: 12
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-S2 various dev board  : CS: 34, DC: 38, RST: 33, BL: 21, SCK: 36, MOSI: 35, MISO: nil
 * ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil
 * ESP8266 various dev board   : CS: 15, DC:  4, RST:  2, BL:  5, SCK: 14, MOSI: 13, MISO: 12
 * Raspberry Pi Pico dev board : CS: 17, DC: 27, RST: 26, BL: 28, SCK: 18, MOSI: 19, MISO: 16
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23, SCK: 19, MOSI: 21, MISO: 20
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3, SCK: 10, MOSI: 12, MISO: 11
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23, SCK: 13, MOSI: 11, MISO: 12
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13, SCK: 11, MOSI:  9, MISO: 10
 * Seeeduino XIAO dev board    : CS:  3, DC:  2, RST:  1, BL:  0, SCK:  8, MOSI: 10, MISO:  9
 * Teensy 4.1 dev board        : CS: 39, DC: 41, RST: 40, BL: 22, SCK: 13, MOSI: 11, MISO: 12
 ******************************************************************************/
#include <Arduino_GFX_Library.h>

#define TFT_DC    5
#define TFT_CS    4
#define TFT_WR    3
#define TFT_RD    2
#define TFT_PORTLOW 1        // PORTA
#define TFT_PORTHIGH 3       // PORTC
#define TFT_RESET 6

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_AVRPAR16(TFT_DC, TFT_CS, TFT_WR, TFT_RD, TFT_PORTLOW, TFT_PORTHIGH);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, TFT_RESET, 3 /* rotation */, false /* IPS */);

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

void setup(void)
{
    gfx->begin();
    gfx->fillScreen(BLACK);

    gfx->setTextColor(WHITE);
    gfx->setTextSize(2, 2, 2);

    gfx->setCursor(10, 10);
    gfx->println("XIAO ESP32C3 (in Arduino framework)");

    gfx->setCursor(10, 30);
    gfx->println("+ ILI9488 SPI TFT");

    gfx->setCursor(10, 50);
    gfx->println("using Arduino_GFX Library");

    int w = gfx->width();
    int h = gfx->height();

    gfx->setCursor(10, 70);
    gfx->println(w + " x " + h);
    gfx->drawRect(0, 0, w, h, WHITE);

    delay(3000);

    for(int i=0; i<w; i++){
      int d = (int)(255 * i/w);
      gfx->drawLine(i, 0, i, w, RGB565(d, 0, 0));
      delay(10);
    }
    for(int i=0; i<w; i++){
      int d = (int)(255 * i/w);
      gfx->drawLine(w-i, 0, w-i, w, RGB565(0, d, 0));
      delay(10);
    }
    for(int i=0; i<w; i++){
      int d = (int)(255 * i/w);
      gfx->drawLine(i, 0, i, w, RGB565(0, 0, d));
      delay(10);
    }
}

void loop()
{
    gfx->setTextColor(WHITE);
    gfx->setTextSize(6, 6, 2);

    gfx->fillScreen(RED);
    gfx->setCursor(100, 100);
    gfx->println("RED");
    delay(2000);

    gfx->fillScreen(GREEN);
    gfx->setCursor(100, 100);
    gfx->println("GREEN");
    delay(2000);

    gfx->fillScreen(BLUE);
    gfx->setCursor(100, 100);
    gfx->println("BLUE");
    delay(2000);
}

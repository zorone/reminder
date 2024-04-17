// NOTE: run:
// .\library\avr_debug\hub4com-2.1.0.0-386\com2tcp --baud 115200 \\.\COM9 11000 
// at the root directory of the project.
// Before debugging.

// NOTE:Make sure pio is in System Variables
// %USERPROFILE%\.platformio\penv\Scripts

//The Game of Life, also known simply as Life, is a cellular automaton
//devised by the British mathematician John Horton Conway in 1970.
// https://en.wikipedia.org/wiki/Conway's_Game_of_Life

/*
  Port:
  PIN_SPI_SS    53  -- Not Connected
  PIN_SPI_MOSI  51
  PIN_SPI_MISO  50  -- Not Connected; plan to implement for touch screen
  PIN_SPI_SCK   52

  DC            49
  CS            48
  RST           46
*/

// TODO: do LCD backlight control, currently it is directly connected 
// TODO: implement touchscreen features
/* // TODO: Add fonts
from https://github.com/moononournation/ArduinoFreeFontFile/tree/master 
or   https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
*/

/*
  Serial Monitor Command
  for Arduino Mega2560 wifi, default baud value from manufature is 74880 baud
  C:\Users\ASUS\.platformio\penv\Scripts\platformio.exe device monitor -b 74880 --environment reminder

  dump command
  avrdude -p m2560 -P COM10 -c wiring -U flash:r:"mega2560wifi_default.bin":r ;  with DIP: 11110000; no Vin
  avrdude -p m2560 -P COM10 -c wiring -U flash:r:"mega2560wifi_default.bin":r ;  with DIP: 00110000; no Vin
  python ..\tools\esptool-4.7.0\esptool.py -b 115200 --port COM10 read_flash 0x00000 ALL mega2560wifi_default-3.bin;  with DIP: 00001110; no Vin

  reflash
  avrdude -p m2560 -P COM9 -b 115200 -c wiring -D -U flash:w:mega2560wifi_default-2.bin:r

  IMPORTANT!!!
  Check if it was connecting correctly. Check if USB port is seated securely. And check if DIP switch is set, and was securely set.

  Troubleshoot Process:
  Flash esp8266 -> try DIP: 11000000 -> try DIP: 11100000 -> try DIP 11110000; It should eliminate "programmer is not responding" error.
  sometimes you should go take a break for a bit. It might work after you come back...
  (Actually, maybe processor get too hot? If that the case, we should lower baud rates a bit.)
*/

#include <Arduino.h>
#include <Arduino_GFX_Library.h>

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

#define TFT_DC    6          // TFT_RS
#define TFT_CS    4
#define TFT_WR    3
#define TFT_RD    -1         // connected to 3V3
#define TFT_PORTLOW  11       // PORTL
#define TFT_PORTHIGH  3       // PORTC
#define TFT_RESET 2

#define TFT_D0   49
#define TFT_D1   48
#define TFT_D2   47
#define TFT_D3   46
#define TFT_D4   45
#define TFT_D5   44
#define TFT_D6   43
#define TFT_D7   42
#define TFT_D8   37
#define TFT_D9   36
#define TFT_D10  35
#define TFT_D11  34
#define TFT_D12  33
#define TFT_D13  32
#define TFT_D14  31
#define TFT_D15  30

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_SWPAR16(TFT_DC, TFT_CS, TFT_WR, TFT_RD, 49, 48, 47, 46, 45, 44, 43, 42, 37, 36, 35, 34, 33, 32, 31, 30);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9486_18bit(bus, TFT_RESET, 3 /* rotation */, false /* IPS */);

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/
#define BACKGROUND BLACK
#define MARK_COLOR WHITE
#define SUBMARK_COLOR DARKGREY // LIGHTGREY

int8_t colorIdx = 0;
int16_t colorSet[] = {BACKGROUND, RED, GREEN, BLUE};
int8_t colorSetSize = sizeof(colorSet) / 16;

int8_t dbgPin[] = {TFT_DC, TFT_CS, TFT_WR, TFT_RESET,
                   TFT_D0, TFT_D1, TFT_D2, TFT_D3, TFT_D4, TFT_D5, TFT_D6, TFT_D7,
                   TFT_D8, TFT_D9, TFT_D10, TFT_D11, TFT_D12, TFT_D13, TFT_D14, TFT_D15};
String dbgPinName[] = {"DC", "CS", "WR", "RST",
                     "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7",
                     "D8", "D9", "D10", "D11", "D12", "D13", "D14", "D15"};

void shiftBGcolor(int colorIdx);

void setup(void)
{
    Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX RGB shift example");

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  
  gfx->fillScreen(BACKGROUND);
  delay(5000);
}

void loop()
{
  shiftBGcolor(colorIdx);
  delay(1000);
}

void shiftBGcolor(int colorIdx){
  colorIdx++;

  if(colorIdx > colorSetSize){
    colorIdx = 1;
  }

  gfx->fillScreen(colorSet[colorIdx]);
}
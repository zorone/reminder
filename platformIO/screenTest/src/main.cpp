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

#define TFT_DC    5          // TFT_RS
#define TFT_CS    4
#define TFT_WR    3
#define TFT_RD    -1         // connected to 3V3
#define TFT_PORTLOW  PINL       // PORTL
#define TFT_PORTHIGH PINC       // PORTC
#define TFT_RESET 2

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = new Arduino_AVRPAR16(TFT_DC, TFT_CS, TFT_WR, TFT_RD, TFT_PORTLOW, TFT_PORTHIGH);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9486_18bit(bus, TFT_RESET, 3 /* rotation */, false /* IPS */);

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

void drawGrid(void);
void initGrid(void);
void computeCA();
uint8_t getNumberOfNeighbors(int x, int y);

//#define GRIDX 80
//#define GRIDY 60
//#define CELLXY 4

#define GRIDX 60
#define GRIDY 40
#define CELLXY 8

#define GEN_DELAY 0

//Current grid
int8_t grid[GRIDX][GRIDY];

//The new grid for the next generation
int8_t newgrid[GRIDX][GRIDY];

//Number of generations
#define NUMGEN 600

uint16_t genCount = 0;

void setup()   {

  Serial.begin(115200);
  Serial.println("Arduino_GFX testing");
  Serial.println("Conway's Game of Life Demo");

  //Set up the display
  if(!gfx->begin()){
    Serial.println("gfx->begin() failed!");
  }

  gfx->setRotation(3);
  gfx->fillScreen(BLACK);
  gfx->setTextSize(1);
  gfx->setTextColor(WHITE);
  gfx->setCursor(0, 0);

}

void loop() {

  //Display a simple splash screen
  gfx->fillScreen(BLACK);
  gfx->setTextSize(2);
  gfx->setTextColor(WHITE);
  gfx->setCursor(40, 5);
  gfx->println(F("Arduino"));
  gfx->setCursor(35, 25);
  gfx->println(F("Cellular"));
  gfx->setCursor(35, 45);
  gfx->println(F("Automata"));

  delay(1000);

  gfx->fillScreen(BLACK);

  initGrid();

  genCount = NUMGEN;

  drawGrid();

  //Compute generations
  for (uint16_t gen = 0; gen < genCount; gen++)
  {
    computeCA();
    drawGrid();
    delay(GEN_DELAY);
    for (int16_t x = 1; x < GRIDX-1; x++) {
      for (int16_t y = 1; y < GRIDY-1; y++) {
        grid[x][y] = newgrid[x][y];
      }
    }

  }
}

//Draws the grid on the display
void drawGrid(void) {

  uint16_t color = WHITE;
  for (int16_t x = 1; x < GRIDX - 1; x++) {
    for (int16_t y = 1; y < GRIDY - 1; y++) {
      if ((grid[x][y]) != (newgrid[x][y])) {
        if (newgrid[x][y] == 1) color = 0xFFFF; //random(0xFFFF);
        else color = 0;
        gfx->fillRect(CELLXY * x, CELLXY * y, CELLXY, CELLXY, color);
      }
    }
  }
}

//Initialise Grid
void initGrid(void) {
  for (int16_t x = 0; x < GRIDX; x++) {
    for (int16_t y = 0; y < GRIDY; y++) {
      newgrid[x][y] = 0;

      if (x == 0 || x == GRIDX - 1 || y == 0 || y == GRIDY - 1) {
        grid[x][y] = 0;
      }
      else {
        if (random(3) == 1)
          grid[x][y] = 1;
        else
          grid[x][y] = 0;
      }

    }
  }
}

//Compute the CA. Basically everything related to CA starts here
void computeCA() {
  for (int16_t x = 1; x < GRIDX; x++) {
    for (int16_t y = 1; y < GRIDY; y++) {
      uint8_t neighbors = getNumberOfNeighbors(x, y);
      if (grid[x][y] == 1 && (neighbors == 2 || neighbors == 3 ))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 1)  newgrid[x][y] = 0;
      if (grid[x][y] == 0 && (neighbors == 3))
      {
        newgrid[x][y] = 1;
      }
      else if (grid[x][y] == 0) newgrid[x][y] = 0;
    }
  }
}

// Check the Moore neighbourhood
uint8_t getNumberOfNeighbors(int x, int y) {
  int a = x-1;
  int b = x+1;
  int c = y-1;
  int d = y+1;
  return grid[a][y] + grid[a][c] + grid[x][c] + grid[b][c] + grid[b][y] + grid[b][d] + grid[x][d] + grid[a][d];
}

/*
   The MIT License (MIT)

   Copyright (c) 2016 RuntimeProjects.com

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/


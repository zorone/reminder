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
  //incorrect avrdude -p m2560 -P COM9 -c arduino -U flash:r:"D:/shared/Documents/CPE2422/reminder/dump mega2560wifi_default.bin":r
  //incorrect avrdude -p m2560 -P COM9 -c stk500v2 -b 74880 -U flash:r:"mega2560wifi_default.bin":r
  //incorrect avrdude -p m2560 -P COM9 -c wiring -U flash:r:"mega2560wifi_default.bin":r
  //not work; DIP: 11110000; avrdude -p m2560 -P COM10 -c arduino -U flash:r:"mega2560wifi_default.bin":r
  //not work; DIP: 00110000; avrdude -p m2560 -P COM10 -c arduino -U flash:r:"mega2560wifi_default.bin":r
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

Arduino_DataBus *bus = new Arduino_HWSPI(49 /* DC */, 48 /* CS */);
Arduino_GFX *gfx = new Arduino_ILI9488_18bit(bus, 46 /* RST */);

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


/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define PIN_A          14
#define PIN_B          12
#define PIN_KEY        13

void showTextOnScreen();

void IRAM_ATTR Ainterupt();
void IRAM_ATTR Binterupt();

int16_t idx = 0;
int8_t rotatingState = 0;

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!

  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  pinMode(PIN_A, INPUT_PULLUP);
  attachInterrupt(PIN_A, Ainterupt, RISING);

  pinMode(PIN_B, INPUT_PULLUP);
  attachInterrupt(PIN_B, Binterupt, RISING);

  pinMode(PIN_KEY, INPUT_PULLUP);

  showTextOnScreen();
}

void loop() {
  rotatingState = 0;

  for(uint8_t i = 0; i < 255; i++){
    // do nothing... This loop is for delaying idx value update.
  }
  display.clearDisplay();
  display.setCursor(0, 8);
  display.println(idx);
  display.display();
}

void showTextOnScreen(){
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.cp437(true);
  display.setTextWrap(true);

  display.println("Hello, World!");

  display.display();
  delay(1);
}

void IRAM_ATTR Ainterupt(){
  if(rotatingState != 0) return;

  rotatingState = 1;
  idx++;

}
void IRAM_ATTR Binterupt(){
  if(rotatingState != 0) return;

  rotatingState = -1;
  idx--;
}
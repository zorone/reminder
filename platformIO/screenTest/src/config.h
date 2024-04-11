////////////////////////////////////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////////////////////////////////////
//#define TFT_PARALLEL_8_BIT
#define TFT_PARALLEL_16_BIT

// The parallel interface write cycle period is derived from a division of the CPU clock
// speed so scales with the processor clock. This means that the divider ratio may need
// to be increased when overclocking. I may also need to be adjusted dependant on the
// display controller type (ILI94341, HX8357C etc.). If RP2040_PIO_CLK_DIV is not defined
// the library will set default values which may not suit your display.
// The display controller data sheet will specify the minimum write cycle period. The
// controllers often work reliably for shorter periods, however if the period is too short
// the display may not initialise or graphics will become corrupted.
// PIO write cycle frequency = (CPU clock/(4 * RP2040_PIO_CLK_DIV))
//#define RP2040_PIO_CLK_DIV 1 // 32ns write cycle at 125MHz CPU clock
//#define RP2040_PIO_CLK_DIV 2 // 64ns write cycle at 125MHz CPU clock
//#define RP2040_PIO_CLK_DIV 3 // 96ns write cycle at 125MHz CPU clock
//#define RP2040_PIO_CLK_DIV 4 // 96ns write cycle at 125MHz CPU clock

////////////////////////////////////////////////////////////////////////////////////////////
// Display driver type
////////////////////////////////////////////////////////////////////////////////////////////
#define ILI9486_DRIVER
//#define HX8357B_DRIVER
//#define HX8357C_DRIVER

////////////////////////////////////////////////////////////////////////////////////////////
// RP2040 pins used
////////////////////////////////////////////////////////////////////////////////////////////

// These pins can be moved and are controlled directly by the library software
#define TFT_RST   2  // Reset pin
#define TFT_CS    4  // Do not define if chip select control pin permanently connected to 0V
//#define TFT_RD   -1   // Do not define, read pin must be permanently connected to 3V3

// Note: All the following pins are PIO hardware configured and driven
  #define TFT_WR   3    // Write strobe pin
  #define TFT_DC   5    // Data Command control pin

  // PIO requires these to be sequentially increasing
  #define TFT_D0   22
  #define TFT_D1   23
  #define TFT_D2   24
  #define TFT_D3   25
  #define TFT_D4   26
  #define TFT_D5   27
  #define TFT_D6   28
  #define TFT_D7   29
  #define TFT_D8   30
  #define TFT_D9   31
  #define TFT_D10  32
  #define TFT_D11  33
  #define TFT_D12  34
  #define TFT_D13  35
  #define TFT_D14  36
  #define TFT_D15  37
//*/

////////////////////////////////////////////////////////////////////////////////////////////
// Fonts to be available
////////////////////////////////////////////////////////////////////////////////////////////
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

////////////////////////////////////////////////////////////////////////////////////////////
/**
* Blink with debugger
*/
#include <Arduino.h>
#include <avr8-stub.h>
#include <app_api.h>   // only needed with flash breakpoints

// run:
// .\library\avr_debug\hub4com-2.1.0.0-386\com2tcp --baud 115200 \\.\COM9 11000 
// at the root directory of the project.
// Before debugging.

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize the avr-debugger
  debug_init();
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
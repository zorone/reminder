/**
* Blink with debugger
*/
#include <Arduino.h>
#include <avr8-stub.h>
#include <app_api.h>   // only needed with flash breakpoints

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
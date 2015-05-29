/*
  Test InputDebounce Arduino Library

  Mario Ban, 05.2015

*/

#include "InputDebounce.h"

#define BUTTON_DEBOUNCE_DELAY   20   /* [ms] */

static const int pinLED = LED_BUILTIN; // 13
static const int pinSwitch = 2;

static InputDebounce buttonTest;

void setup()
{
  // initialize digital pin 13 as an output
  pinMode(pinLED, OUTPUT);
  // initialize digital pin 2 as an input
  pinMode(pinSwitch, INPUT);
  
  // init serial
  Serial.begin(9600);
  
  // setup input button (debounced)
  buttonTest.setup(pinSwitch, BUTTON_DEBOUNCE_DELAY);
}

void loop()
{
  static unsigned int buttonTest_StateOnCount = 0;
  
  unsigned long now = millis();
  
  // poll button state, return on-time [ms] if pressed (debounced)
  unsigned int buttonTest_OnTime = buttonTest.process(now);
  
  if(buttonTest_OnTime) {
    unsigned int count = buttonTest.getStateOnCount();
    if(buttonTest_StateOnCount != count) {
      buttonTest_StateOnCount = count;
      digitalWrite(pinLED, HIGH); // turn the LED on
      Serial.print("HIGH");
    }
    else {
      Serial.print("HIGH still pressed");
    }
    Serial.print(" (");
    Serial.print(buttonTest_OnTime);
    Serial.println("ms)");
  }
  else {
    digitalWrite(pinLED, LOW); // turn the LED off
  }
}

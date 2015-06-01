/*
  Test InputDebounce Arduino Library

  Mario Ban, 05.2015

*/

#include "InputDebounce.h"

#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

static const int pinLED = LED_BUILTIN; // 13
static const int pinSwitch = 2;

static InputDebounce buttonTest;

void setup()
{
  // initialize digital pin as an output
  pinMode(pinLED, OUTPUT);
  
  // init serial
  Serial.begin(9600);
  
  // setup input button (debounced)
  buttonTest.setup(pinSwitch, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
}

void loop()
{
  static unsigned int buttonTest_StateOnCount = 0;
  static unsigned int buttonTest_OnTimeLast = 0;
  
  unsigned long now = millis();
  
  // poll button state, return on-time [ms] if pressed (debounced)
  unsigned int buttonTest_OnTime = buttonTest.process(now);
  
  if(buttonTest_OnTime) {
    // save current on-time (button pressed), to be used when released
    buttonTest_OnTimeLast = buttonTest_OnTime;
    // check for state change
    unsigned int count = buttonTest.getStateOnCount();
    if(buttonTest_StateOnCount != count) {
      buttonTest_StateOnCount = count;
      // handle pressed state
      digitalWrite(pinLED, HIGH); // turn the LED on
      Serial.print("HIGH");
    }
    else {
      // handle still pressed state
      Serial.print("HIGH still pressed");
    }
    Serial.print(" (");
    Serial.print(buttonTest_OnTime);
    Serial.println("ms)");
  }
  else {
    if(buttonTest_OnTimeLast) {
      // handle released state
      digitalWrite(pinLED, LOW); // turn the LED off
      Serial.print("LOW (");
      Serial.print(buttonTest_OnTimeLast);
      Serial.println("ms)");
      buttonTest_OnTimeLast = 0;
    }
  }
  
  delay(1); // [ms]
}

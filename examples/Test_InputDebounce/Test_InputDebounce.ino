/*
  Test InputDebounce Arduino Library

  Mario Ban, 05.2015
  https://github.com/Mokolea/InputDebounce

  GNU General Public License v2.0
  Copyright (C) 2015 Mario Ban
*/

#include "InputDebounce.h"

#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

static const int pinLED = LED_BUILTIN; // 13
static const int pinSwitch = 2;

static InputDebounce buttonTest; // not enabled yet, setup has to be called later

void setup()
{
  // initialize digital pin as an output
  pinMode(pinLED, OUTPUT);
  
  // init serial
  Serial.begin(9600);
  
  Serial.println("Test InputDebounce library");
  
  // setup input button (debounced)
  buttonTest.setup(pinSwitch, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  
  // examples
  // buttonTest.setup(pinSwitch);
  // buttonTest.setup(pinSwitch, DEFAULT_INPUT_DEBOUNCE_DELAY);
  // buttonTest.setup(pinSwitch, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_UP_RES);
}

void loop()
{
  static unsigned int buttonTest_StateOnCount = 0; // to handle new pressed state, and still pressed state
  static unsigned int buttonTest_OnTimeLast = 0; // to handle new released state, and remember last on-time (button pressed)
  
  unsigned long now = millis();
  
  // poll button state, return continuous on-time [ms] if pressed (debounced)
  unsigned int buttonTest_OnTime = buttonTest.process(now);
  
  // handle input button
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
      Serial.print("LOW (last on-time: HIGH ");
      Serial.print(buttonTest_OnTimeLast);
      Serial.println("ms)");
      buttonTest_OnTimeLast = 0; // reset, do not handle this released state again
    }
  }
  
  delay(1); // [ms]
}

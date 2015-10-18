/*
  Test InputDebounce Arduino Library, using callbacks

  Mario Ban, 10.2015
  https://github.com/Mokolea/InputDebounce

  GNU General Public License v2.0
  Copyright (C) 2015 Mario Ban
*/

#include "InputDebounce.h"

#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

static const int pinLED = LED_BUILTIN; // 13
static const int pinSwitch = 2;

static InputDebounce buttonTest; // not enabled yet, setup has to be called later

void buttonTest_pressedCallback()
{
  // handle pressed state
  digitalWrite(pinLED, HIGH); // turn the LED on
  Serial.println("HIGH");
}

void buttonTest_releasedCallback()
{
  // handle released state
  digitalWrite(pinLED, LOW); // turn the LED off
  Serial.println("LOW");
}

void buttonTest_pressedDurationCallback(unsigned long duration)
{
  // handle still pressed state
  Serial.print("HIGH still pressed (");
  Serial.print(duration);
  Serial.println("ms)");
}

void setup()
{
  // initialize digital pin as an output
  pinMode(pinLED, OUTPUT);
  
  // init serial
  Serial.begin(9600);
  
  Serial.println("Test InputDebounce library, using callbacks");
  
  // register callbacks
  buttonTest.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback);
  
  // setup input button (debounced)
  buttonTest.setup(pinSwitch, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  
  // examples
  // buttonTest.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, NULL);
  // buttonTest.setup(pinSwitch);
  // buttonTest.setup(pinSwitch, DEFAULT_INPUT_DEBOUNCE_DELAY);
  // buttonTest.setup(pinSwitch, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_UP_RES);
}

void loop()
{
  unsigned long now = millis();
  
  // poll button state
  buttonTest.process(now); // callbacks called in context of this function
  
  delay(1); // [ms]
}

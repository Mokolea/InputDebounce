/*
  Test InputDebounce Arduino Library, using callbacks

  Mario Ban, 10.2015
  https://github.com/Mokolea/InputDebounce


  Copyright 2017 Mario Ban

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include "InputDebounce.h"

#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

static const int pinLED = LED_BUILTIN; // 13
static const int pinSwitch = 2;

static InputDebounce buttonTest; // not enabled yet, setup has to be called later

void buttonTest_pressedCallback(uint8_t pinIn)
{
  // handle pressed state
  digitalWrite(pinLED, HIGH); // turn the LED on
  Serial.println("HIGH");
}

void buttonTest_releasedCallback(uint8_t pinIn)
{
  // handle released state
  digitalWrite(pinLED, LOW); // turn the LED off
  Serial.println("LOW");
}

void buttonTest_pressedDurationCallback(uint8_t pinIn, unsigned long duration)
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

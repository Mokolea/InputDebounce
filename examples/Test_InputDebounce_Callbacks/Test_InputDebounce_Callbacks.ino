/*
  Test InputDebounce Arduino Library, using callbacks

  Mario Ban, 10.2015
  https://github.com/Mokolea/InputDebounce


  Copyright 2018 Mario Ban

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
static const int pinSwitchA = 2;
static const int pinSwitchB = 3;

static InputDebounce buttonTestA; // not enabled yet, setup has to be called first, see setup() below
static InputDebounce buttonTestB;

void buttonTest_pressedCallback(uint8_t pinIn)
{
  // handle pressed state
  digitalWrite(pinLED, HIGH); // turn the LED on
  Serial.print("HIGH (pin: ");
  Serial.print(pinIn);
  Serial.println(")");
}

void buttonTest_releasedCallback(uint8_t pinIn)
{
  // handle released state
  digitalWrite(pinLED, LOW); // turn the LED off
  Serial.print("LOW (pin: ");
  Serial.print(pinIn);
  Serial.println(")");
}

void buttonTest_pressedDurationCallback(uint8_t pinIn, unsigned long duration)
{
  // handle still pressed state
  Serial.print("HIGH (pin: ");
  Serial.print(pinIn);
  Serial.print(") still pressed, duration ");
  Serial.print(duration);
  Serial.println("ms");
}

void buttonTest_releasedDurationCallback(uint8_t pinIn, unsigned long duration)
{
  // handle released state
  Serial.print("LOW (pin: ");
  Serial.print(pinIn);
  Serial.print("), duration ");
  Serial.print(duration);
  Serial.println("ms");
}

void setup()
{
  // initialize digital pin as an output
  pinMode(pinLED, OUTPUT);
  
  // init serial
  Serial.begin(9600);
  
  Serial.println("Test InputDebounce library, using callback functions");
  
  // register callback functions (shared, used by all buttons)
  buttonTestA.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback, buttonTest_releasedDurationCallback);
  buttonTestB.registerCallbacks(buttonTest_pressedCallback, buttonTest_releasedCallback, buttonTest_pressedDurationCallback, buttonTest_releasedDurationCallback);
  
  // setup input buttons (debounced)
  buttonTestA.setup(pinSwitchA, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  buttonTestB.setup(pinSwitchB, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES, 300); // single-shot pressed-on time duration callback
  
  // examples
  // buttonTestA.registerCallbacks(buttonTest_pressedCallback, NULL, NULL, buttonTest_releasedDurationCallback); // no continuous pressed-on time duration, ...
  // buttonTestA.setup(pinSwitchA);
  // buttonTestA.setup(pinSwitchA, BUTTON_DEBOUNCE_DELAY);
  // buttonTestA.setup(pinSwitchA, DEFAULT_INPUT_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_UP_RES);
  // buttonTestA.setup(pinSwitchA, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES, 0, InputDebounce::ST_NORMALLY_CLOSED); // switch-type normally closed
}

void loop()
{
  unsigned long now = millis();
  
  // poll button state
  buttonTestA.process(now); // callbacks called in context of this function
  buttonTestB.process(now);
  
  delay(1); // [ms]
}

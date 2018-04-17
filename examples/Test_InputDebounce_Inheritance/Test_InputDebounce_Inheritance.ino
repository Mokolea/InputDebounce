/*
  Test InputDebounce Arduino Library, using inheritance

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

class MyInputDebounce : public InputDebounce
{
public:
  MyInputDebounce(int8_t pinIn = -1, unsigned long debDelay = DEFAULT_INPUT_DEBOUNCE_DELAY, PinInMode pinInMode = PIM_INT_PULL_UP_RES, unsigned long pressedDuration = 0)
    : InputDebounce(pinIn, debDelay, pinInMode, pressedDuration)
    , _pinLED(-1)
  {}
  virtual ~MyInputDebounce()
  {}
  void setPinLED(int8_t pinLED)
  {
    _pinLED = pinLED;
  }
protected:
  virtual void pressed()
  {
    // handle pressed state
    if(_pinLED >= 0) {
      digitalWrite(_pinLED, HIGH); // turn the LED on
    }
    Serial.print("HIGH (pin: ");
    Serial.print(getPinIn());
    Serial.println(")");
  }
  virtual void released()
  {
    // handle released state
    if(_pinLED >= 0) {
      digitalWrite(_pinLED, LOW); // turn the LED off
    }
    Serial.print("LOW (pin: ");
    Serial.print(getPinIn());
    Serial.println(")");
  }
  virtual void pressedDuration(unsigned long duration)
  {
    // handle still pressed state
    Serial.print("HIGH (pin: ");
    Serial.print(getPinIn());
    Serial.print(") still pressed, duration ");
    Serial.print(duration);
    Serial.println("ms");
  }
  virtual void releasedDuration(unsigned long duration)
  {
    // handle released state
    Serial.print("LOW (pin: ");
    Serial.print(getPinIn());
    Serial.print("), duration ");
    Serial.print(duration);
    Serial.println("ms");
  }
private:
  int8_t _pinLED;
};

static MyInputDebounce buttonTestA; // not enabled yet, setup has to be called first, see setup() below
static MyInputDebounce buttonTestB;

void setup()
{
  // initialize digital pin as an output
  pinMode(pinLED, OUTPUT);
  
  // init serial
  Serial.begin(9600);
  
  Serial.println("Test InputDebounce library, using inheritance");
  
  // setup input buttons (debounced)
  buttonTestA.setPinLED(pinLED);
  buttonTestA.setup(pinSwitchA, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  buttonTestB.setPinLED(-1);
  buttonTestB.setup(pinSwitchB, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES, 300); // single-shot pressed-on time duration callback
  
  // examples
  // buttonTestA.setup(pinSwitchA);
  // buttonTestA.setup(pinSwitchA, BUTTON_DEBOUNCE_DELAY);
  // buttonTestA.setup(pinSwitchA, DEFAULT_INPUT_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_UP_RES);
  // buttonTestA.setup(pinSwitchA, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES, 0, InputDebounce::ST_NORMALLY_CLOSED); // switch-type normally closed
}

void loop()
{
  unsigned long now = millis();
  
  // poll button state
  buttonTestA.process(now);
  buttonTestB.process(now);
  
  delay(1); // [ms]
}

/*
  Test InputDebounce Arduino Library, using inheritance

  Mario Ban, 10.2015
  https://github.com/Mokolea/InputDebounce


  Copyright 2015 Mario Ban

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

class MyInputDebounce : public InputDebounce
{
public:
  MyInputDebounce(int8_t pinIn = -1, unsigned long debDelay = DEFAULT_INPUT_DEBOUNCE_DELAY, PinInMode pinInMode = PIM_INT_PULL_UP_RES)
    : InputDebounce(pinIn, debDelay, pinInMode)
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
    Serial.println("HIGH");
  }
  virtual void released()
  {
    // handle released state
    if(_pinLED >= 0) {
      digitalWrite(_pinLED, LOW); // turn the LED off
    }
    Serial.println("LOW");
  }
  virtual void pressedDuration(unsigned long duration)
  {
    // handle still pressed state
    Serial.print("HIGH still pressed (");
    Serial.print(duration);
    Serial.println("ms)");
  }
private:
  int8_t _pinLED;
};

static MyInputDebounce buttonTest; // not enabled yet, setup has to be called later

void setup()
{
  // initialize digital pin as an output
  pinMode(pinLED, OUTPUT);
  
  // init serial
  Serial.begin(9600);
  
  Serial.println("Test InputDebounce library, using inheritance");
  
  // setup input button (debounced)
  buttonTest.setPinLED(pinLED);
  buttonTest.setup(pinSwitch, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);
  
  // examples
  // buttonTest.setup(pinSwitch);
  // buttonTest.setup(pinSwitch, DEFAULT_INPUT_DEBOUNCE_DELAY);
  // buttonTest.setup(pinSwitch, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_EXT_PULL_UP_RES);
}

void loop()
{
  unsigned long now = millis();
  
  // poll button state
  buttonTest.process(now);
  
  delay(1); // [ms]
}

/*
  InputDebounce Arduino Library

  Mario Ban, 05.2015
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

#ifndef _INPUT_DEBOUNCE_H
#define _INPUT_DEBOUNCE_H

#include "Arduino.h"

#define DEFAULT_INPUT_DEBOUNCE_DELAY   20   // [ms]

//namespace inputdebounce
//{

typedef void (*inputdebounce_state_cb)(uint8_t);
typedef void (*inputdebounce_duration_cb)(uint8_t, unsigned long);

class InputDebounce
{
public:
  enum PinInMode {
    PIM_EXT_PULL_DOWN_RES,
    PIM_EXT_PULL_UP_RES,
    PIM_INT_PULL_UP_RES
  };
  
  InputDebounce(int8_t pinIn = -1, // set input pin >= 0 to enable --> calls setup
                unsigned long debDelay = DEFAULT_INPUT_DEBOUNCE_DELAY,
                PinInMode pinInMode = PIM_INT_PULL_UP_RES,
                unsigned long pressedDuration = 0); // pressed-on time duration: 0 continuous; >0 single-shot [ms]
  virtual ~InputDebounce();
  
  void setup(int8_t pinIn,
             unsigned long debDelay = DEFAULT_INPUT_DEBOUNCE_DELAY,
             PinInMode pinInMode = PIM_INT_PULL_UP_RES,
             unsigned long pressedDuration = 0);
  unsigned long process(unsigned long now); // poll button state, returns continuous pressed-on time duration if on (> debounce delay)
  
  uint8_t getPinIn() const;
  unsigned long getStateOnCount() const;
  
  void registerCallbacks(inputdebounce_state_cb pressedCallback, inputdebounce_state_cb releasedCallback, inputdebounce_duration_cb pressedDurationCallback);
  
protected:
  virtual void pressed(); // called once for state change
  virtual void released(); // called once for state change
  virtual void pressedDuration(unsigned long duration); // still pressed state: continuous pressed-on time duration
  
private:
  // implicitly implemented, not to be used
  InputDebounce(const InputDebounce&);
  InputDebounce& operator=(const InputDebounce&);
  
  uint8_t _pinIn;
  unsigned long _debDelay;
  PinInMode _pinInMode;
  unsigned long _pressedDuration;
  
  bool _enabled;
  bool _valueLast; // last input value
  bool _stateOn; // current on state (debounced)
  unsigned long _timeStamp; // last input value (state) change, start debounce time
  unsigned long _stateOnCount;
  unsigned long _stateOnCountSingleShot;
  
  inputdebounce_state_cb _pressedCallback;
  inputdebounce_state_cb _releasedCallback;
  inputdebounce_duration_cb _pressedDurationCallback;
};

//} // namespace

#endif // _INPUT_DEBOUNCE_H

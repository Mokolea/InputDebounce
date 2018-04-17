/*
  InputDebounce Arduino Library

  Mario Ban, 05.2015
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
  enum SwitchType {
    ST_NORMALLY_OPEN,
    ST_NORMALLY_CLOSED
  };
  
  explicit InputDebounce(int8_t pinIn = -1, // set input pin >= 0 to enable --> calls setup
                unsigned long debounceDelay = DEFAULT_INPUT_DEBOUNCE_DELAY,
                PinInMode pinInMode = PIM_INT_PULL_UP_RES,
                unsigned long pressedDurationMode = 0, // pressed-on time duration mode: 0 continuous; >0 single-shot threshold [ms]
                SwitchType switchType = ST_NORMALLY_OPEN);
  virtual ~InputDebounce();
  
  void setup(int8_t pinIn,
             unsigned long debounceDelay = DEFAULT_INPUT_DEBOUNCE_DELAY,
             PinInMode pinInMode = PIM_INT_PULL_UP_RES,
             unsigned long pressedDurationMode = 0,
             SwitchType switchType = ST_NORMALLY_OPEN);
  
  unsigned long process(unsigned long now); // poll button state, returns continuous pressed-on time duration if pressed state (> debounce delay)
  
  uint8_t getPinIn() const;
  unsigned long getDebounceDelay() const;
  InputDebounce::PinInMode getPinInMode() const;
  unsigned long getPressedDurationMode() const;
  InputDebounce::SwitchType getSwitchType() const;
  
  bool isEnabled() const;
  bool isPressed() const;
  bool isReleased() const;
  unsigned long getStatePressedCount() const;
  unsigned long getCurrentPressedDuration() const; // if currently in pressed state
  unsigned long getLastPressedDuration() const; // if currently in released state
  
  void registerCallbacks(inputdebounce_state_cb pressedCallback,
                         inputdebounce_state_cb releasedCallback,
                         inputdebounce_duration_cb pressedDurationCallback = NULL, // still pressed state: continuous or single-shot pressed-on time duration [ms]
                         inputdebounce_duration_cb releasedDurationCallback = NULL); // pressed-on time duration on release [ms]
  
protected:
  virtual void pressed(); // called once for state change
  virtual void released(); // called once for state change
  virtual void pressedDuration(unsigned long duration); // still pressed state: continuous or single-shot pressed-on time duration [ms]
  virtual void releasedDuration(unsigned long duration); // pressed-on time duration on release [ms]
  
private:
  // implicitly implemented, not to be used
  InputDebounce(const InputDebounce&);
  InputDebounce& operator=(const InputDebounce&);
  
  uint8_t _pinIn;
  unsigned long _debounceDelay;
  PinInMode _pinInMode;
  unsigned long _pressedDurationMode;
  SwitchType _switchType;
  
  bool _enabled;
  bool _valueLast; // last input value
  bool _statePressed; // current pressed/released state (debounced)
  unsigned long _timeStamp; // last input value (state) change, start debounce time
  unsigned long _statePressedCount;
  unsigned long _statePressedCountSingleShot;
  unsigned long _currentPressedDuration;
  
  inputdebounce_state_cb _pressedCallback;
  inputdebounce_state_cb _releasedCallback;
  inputdebounce_duration_cb _pressedDurationCallback;
  inputdebounce_duration_cb _releasedDurationCallback;
};

//} // namespace

#endif // _INPUT_DEBOUNCE_H

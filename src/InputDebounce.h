/*
  InputDebounce Arduino Library

  Mario Ban, 05.2015

  GNU General Public License v2.0
  Copyright (C) 2015 Mario Ban
*/

#ifndef _INPUT_DEBOUNCE_H
#define _INPUT_DEBOUNCE_H

#include "Arduino.h"

#define DEFAULT_INPUT_DEBOUNCE_DELAY   20   // [ms]

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
                PinInMode pinInMode = PIM_INT_PULL_UP_RES);
                
  void setup(int8_t pinIn,
             unsigned long debDelay = DEFAULT_INPUT_DEBOUNCE_DELAY,
             PinInMode pinInMode = PIM_INT_PULL_UP_RES);
  unsigned long process(unsigned long now); // poll button state, returns continuous pressed time if on (> debounce delay)
  unsigned long getStateOnCount() const;
  
private:
  // implicitly implemented, not to be used
  InputDebounce(const InputDebounce&);
  InputDebounce& operator=(const InputDebounce&);
  
  uint8_t _pinIn;
  unsigned long _debDelay;
  PinInMode _pinInMode;
  
  bool _enabled;
  bool _valueLast; // last input value
  bool _stateOn; // current on state (debounced)
  unsigned long _timeStamp; // last input value (state) change, start debounce time
  unsigned long _stateOnCount;
};

#endif // _INPUT_DEBOUNCE_H

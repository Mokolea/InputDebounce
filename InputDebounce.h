/*
  InputDebounce Arduino Library

  Mario Ban, 05.2015

*/

#ifndef _INPUT_DEBOUNCE_H
#define _INPUT_DEBOUNCE_H

#include "Arduino.h"

class InputDebounce
{
public:
  enum PinInMode {
    PIM_EXT_PULL_DOWN_RES,
    PIM_EXT_PULL_UP_RES,
    PIM_INT_PULL_UP_RES
  };

  InputDebounce(int8_t pinIn = -1, unsigned long debDelay = 0, PinInMode pinInMode = PIM_EXT_PULL_UP_RES); // set input pin >= 0 to enable

  void setup(int8_t pinIn, unsigned long debDelay, PinInMode pinInMode);
  unsigned long process(unsigned long now); // poll button state, returns pressed time if on (> debounce delay)
  unsigned long getStateOnCount() const;

private:
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

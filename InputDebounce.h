/*
  Library InputDebounce

  Mario Ban, 05.2015

  TODO:
   - handle input pin with:
     - pull-down resistor
     - pull-up resistor
     - internal pull-up resistor
*/

#ifndef _INPUT_DEBOUNCE_H
#define _INPUT_DEBOUNCE_H

#include "Arduino.h"

class InputDebounce
{
public:
  InputDebounce();

  void setup(uint8_t pinIn, unsigned long debDelay); // input pin with pull-up resistor
  unsigned long process(unsigned long now); // return pressed time if on (> debounce delay)
  unsigned long getStateOnCount() const;

private:
  uint8_t _pinIn;
  unsigned long _debDelay;

  bool _enabled;
  bool _valueLast; // last input value
  bool _stateOn; // current on state (debounced)
  unsigned long _timeStamp; // last input value (state) change, start debounce time
  unsigned long _stateOnCount;

};

#endif // _INPUT_DEBOUNCE_H

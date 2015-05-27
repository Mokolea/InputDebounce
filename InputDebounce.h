/*
  Input-Debounce

  Test TCLite on Arduino Due

  Mario Ban, 05.2015

*/

#ifndef _INPUT_DEBOUNCE_H
#define _INPUT_DEBOUNCE_H

#include "Arduino.h"

class InputDebounce
{
public:
  InputDebounce();

  void setup(uint8_t pinIn, unsigned long debDelay);
  unsigned long process(unsigned long now); // return pressed time if on (> debounce delay)
  unsigned long getStateOnCount();

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

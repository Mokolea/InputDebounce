/*
  Input-Debounce

  Test TCLite on Arduino Due

  Mario Ban, 05.2015

*/

#include "InputDebounce.h"

InputDebounce::InputDebounce()
  : _pinIn(0)
  , _debDelay(0)
  , _enabled(false)
  , _valueLast(false)
  , _stateOn(false)
  , _timeStamp(0)
  , _stateOnCount(0)
{
}

void InputDebounce::setup(uint8_t pinIn, unsigned long debDelay)
{
  if(pinIn > 0) {
    _pinIn = pinIn;
    _debDelay = debDelay;
    pinMode(_pinIn, INPUT);
    _enabled = true;
  }
  else {
    _enabled = false;
  }
}

unsigned long InputDebounce::process(unsigned long now)
{
  if(!_enabled) {
    return 0;
  }
  int value = digitalRead(_pinIn); // LOW (with pull-up res) when button pressed (on)
  // adjust value pressed (on)
  value = !value;
  // check if input value changed
  if(_valueLast != value) {
    _valueLast = value;
    _timeStamp = now;
  }
  // wait debouncing time
  if(now - _timeStamp > _debDelay) {
    // input value (state) has been stable longer than the debounce period
    if(_stateOn != _valueLast) {
      _stateOn = _valueLast;
      if(_stateOn) {
        _stateOnCount++;
      }
    }
  }
  return _stateOn ? now - _timeStamp : 0;
}

unsigned long InputDebounce::getStateOnCount()
{
  return _stateOnCount;
}

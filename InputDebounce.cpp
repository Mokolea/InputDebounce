/*
  InputDebounce Arduino Library

  Mario Ban, 05.2015

*/

#include "InputDebounce.h"

InputDebounce::InputDebounce(int8_t pinIn, unsigned long debDelay, PinInMode pinInMode)
  : _pinIn(0)
  , _debDelay(0)
  , _pinInMode(PIM_EXT_PULL_UP_RES)
  , _enabled(false)
  , _valueLast(false)
  , _stateOn(false)
  , _timeStamp(0)
  , _stateOnCount(0)
{
  setup(pinIn, debDelay, pinInMode);
}

void InputDebounce::setup(int8_t pinIn, unsigned long debDelay, PinInMode pinInMode)
{
  if(pinIn >= 0) {
    _pinIn = pinIn;
    _debDelay = debDelay;
    _pinInMode = pinInMode;
    // configure input pin
    if(_pinInMode == PIM_INT_PULL_UP_RES) {
      pinMode(_pinIn, INPUT_PULLUP);
    }
    else {
      pinMode(_pinIn, INPUT);
    }
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
  bool value = digitalRead(_pinIn) ? true : false; // LOW (with pull-up res) when button pressed (on)
  // adjust value pressed (on)
  if(_pinInMode != PIM_EXT_PULL_DOWN_RES) {
    value = !value;
  }
  // check if input value changed
  if(_valueLast != value) {
    _valueLast = value;
    _timeStamp = now;
    return 0;
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
    return _stateOn ? now - _timeStamp : 0;
  }
  return 0;
}

unsigned long InputDebounce::getStateOnCount() const
{
  return _stateOnCount;
}

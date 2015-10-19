/*
  InputDebounce Arduino Library

  Mario Ban, 05.2015
  https://github.com/Mokolea/InputDebounce

  GNU General Public License v2.0
  Copyright (C) 2015 Mario Ban
*/

#include "InputDebounce.h"

//namespace inputdebounce
//{

InputDebounce::InputDebounce(int8_t pinIn, unsigned long debDelay, PinInMode pinInMode)
  : _pinIn(0)
  , _debDelay(0)
  , _pinInMode(PIM_INT_PULL_UP_RES)
  , _enabled(false)
  , _valueLast(false)
  , _stateOn(false)
  , _timeStamp(0)
  , _stateOnCount(0)
  , _pressedCallback(NULL)
  , _releasedCallback(NULL)
  , _pressedDurationCallback(NULL)
{
  setup(pinIn, debDelay, pinInMode);
}

InputDebounce::~InputDebounce()
{}

void InputDebounce::setup(int8_t pinIn, unsigned long debDelay, PinInMode pinInMode)
{
  if(pinIn >= 0) {
    _pinIn = pinIn;
    _debDelay = debDelay;
    _pinInMode = pinInMode;
    // initialize digital pin as an input
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
        pressed();
      }
      else {
        released();
      }
    }
    unsigned long duration = _stateOn ? now - _timeStamp : 0;
    if(duration) {
      pressedDuration(duration);
    }
    return duration;
  }
  return 0;
}

unsigned long InputDebounce::getStateOnCount() const
{
  return _stateOnCount;
}

void InputDebounce::registerCallbacks(inputdebounce_state_cb pressedCallback, inputdebounce_state_cb releasedCallback, inputdebounce_duration_cb pressedDurationCallback)
{
  _pressedCallback = pressedCallback;
  _releasedCallback = releasedCallback;
  _pressedDurationCallback = pressedDurationCallback;
}

void InputDebounce::pressed()
{
  if(_pressedCallback) {
    _pressedCallback();
  }
}

void InputDebounce::released()
{
  if(_releasedCallback) {
    _releasedCallback();
  }
}

void InputDebounce::pressedDuration(unsigned long duration)
{
  if(_pressedDurationCallback) {
    _pressedDurationCallback(duration);
  }
}

//} // namespace

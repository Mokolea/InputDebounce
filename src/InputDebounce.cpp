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

#include "InputDebounce.h"

//namespace inputdebounce
//{

InputDebounce::InputDebounce(int8_t pinIn, unsigned long debDelay, PinInMode pinInMode, unsigned long pressedDuration)
  : _pinIn(0)
  , _debDelay(0)
  , _pinInMode(PIM_INT_PULL_UP_RES)
  , _pressedDuration(0)
  , _enabled(false)
  , _valueLast(false)
  , _stateOn(false)
  , _timeStamp(0)
  , _stateOnCount(0)
  , _stateOnCountSingleShot(0)
  , _pressedCallback(NULL)
  , _releasedCallback(NULL)
  , _pressedDurationCallback(NULL)
{
  setup(pinIn, debDelay, pinInMode, pressedDuration);
}

InputDebounce::~InputDebounce()
{}

void InputDebounce::setup(int8_t pinIn, unsigned long debDelay, PinInMode pinInMode, unsigned long pressedDuration)
{
  if(pinIn >= 0) {
    _pinIn = pinIn;
    _debDelay = debDelay;
    _pinInMode = pinInMode;
    _pressedDuration = pressedDuration;
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
      if(!_pressedDuration) {
        pressedDuration(duration); // continuous
      }
      else if(duration >= _pressedDuration && _stateOnCountSingleShot != _stateOnCount) {
        _stateOnCountSingleShot = _stateOnCount;
        pressedDuration(duration); // single-shot
      }
    }
    return duration;
  }
  return 0;
}

uint8_t InputDebounce::getPinIn() const
{
  return _pinIn;
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
    _pressedCallback(_pinIn);
  }
}

void InputDebounce::released()
{
  if(_releasedCallback) {
    _releasedCallback(_pinIn);
  }
}

void InputDebounce::pressedDuration(unsigned long duration)
{
  if(_pressedDurationCallback) {
    _pressedDurationCallback(_pinIn, duration);
  }
}

//} // namespace

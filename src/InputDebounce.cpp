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

#include "InputDebounce.h"

//namespace inputdebounce
//{

InputDebounce::InputDebounce(int8_t pinIn, unsigned long debounceDelay, PinInMode pinInMode, unsigned long pressedDurationMode, SwitchType switchType)
  : _pinIn(0)
  , _debounceDelay(0)
  , _pinInMode(PIM_INT_PULL_UP_RES)
  , _pressedDurationMode(0)
  , _switchType(ST_NORMALLY_OPEN)
  , _enabled(false)
  , _valueLast(false)
  , _statePressed(false)
  , _timeStamp(0)
  , _statePressedCount(0)
  , _statePressedCountSingleShot(0)
  , _currentPressedDuration(0)
  , _pressedCallback(NULL)
  , _releasedCallback(NULL)
  , _pressedDurationCallback(NULL)
  , _releasedDurationCallback(NULL)
{
  setup(pinIn, debounceDelay, pinInMode, pressedDurationMode, switchType);
}

InputDebounce::~InputDebounce()
{}

void InputDebounce::setup(int8_t pinIn, unsigned long debounceDelay, PinInMode pinInMode, unsigned long pressedDurationMode, SwitchType switchType)
{
  if(pinIn >= 0) {
    _pinIn = pinIn;
    _debounceDelay = debounceDelay;
    _pinInMode = pinInMode;
    _pressedDurationMode = pressedDurationMode;
    _switchType = switchType;
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
  // handle switch-type
  if(_switchType == ST_NORMALLY_CLOSED) {
    value = !value;
  }
  // check if input value changed
  if(_valueLast != value) {
    _valueLast = value;
    _timeStamp = now;
    return 0;
  }
  // wait debouncing time
  if(now - _timeStamp > _debounceDelay) {
    // input value (state) has been stable longer than the debounce period
    if(_statePressed != _valueLast) {
      _statePressed = _valueLast;
      if(_statePressed) {
        _statePressedCount++;
        pressed();
      }
      else {
        released();
        releasedDuration(_currentPressedDuration);
      }
    }
    unsigned long duration = _statePressed ? now - _timeStamp : 0;
    if(duration) {
      _currentPressedDuration = duration;
      if(!_pressedDurationMode) {
        pressedDuration(duration); // continuous
      }
      else if(duration >= _pressedDurationMode && _statePressedCountSingleShot != _statePressedCount) {
        _statePressedCountSingleShot = _statePressedCount;
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

unsigned long InputDebounce::getDebounceDelay() const
{
  return _debounceDelay;
}

InputDebounce::PinInMode InputDebounce::getPinInMode() const
{
  return _pinInMode;
}

unsigned long InputDebounce::getPressedDurationMode() const
{
  return _pressedDurationMode;
}

InputDebounce::SwitchType InputDebounce::getSwitchType() const
{
  return _switchType;
}

bool InputDebounce::isEnabled() const
{
  return _enabled;
}

bool InputDebounce::isPressed() const
{
  return _statePressed;
}

bool InputDebounce::isReleased() const
{
  return !_statePressed;
}

unsigned long InputDebounce::getStatePressedCount() const
{
  return _statePressedCount;
}

unsigned long InputDebounce::getCurrentPressedDuration() const
{
  return _statePressed ? _currentPressedDuration : 0;
}

unsigned long InputDebounce::getLastPressedDuration() const
{
  return !_statePressed ? _currentPressedDuration : 0;
}

void InputDebounce::registerCallbacks(inputdebounce_state_cb pressedCallback, inputdebounce_state_cb releasedCallback,
                                      inputdebounce_duration_cb pressedDurationCallback, inputdebounce_duration_cb releasedDurationCallback)
{
  _pressedCallback = pressedCallback;
  _releasedCallback = releasedCallback;
  _pressedDurationCallback = pressedDurationCallback;
  _releasedDurationCallback = releasedDurationCallback;
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

void InputDebounce::releasedDuration(unsigned long duration)
{
  if(_releasedDurationCallback) {
    _releasedDurationCallback(_pinIn, duration);
  }
}

//} // namespace

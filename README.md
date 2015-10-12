# InputDebounce Arduino Library [![Build Status](https://travis-ci.org/Mokolea/InputDebounce.svg)](https://travis-ci.org/Mokolea/InputDebounce)

Simple polling input debounce [Arduino](https://www.arduino.cc/) library.

Facts:
 - used for push-button like switches
 - delivers input value (state) after it has been stable (not flickering) for longer than the debounce period
 - delivers pressed-on time [ms]
 - handles input pin with:
    - external pull-down resistor
    - external pull-up resistor
    - internal pull-up resistor (default)

Available from the Arduino IDE [Library Manager](https://www.arduino.cc/en/Guide/Libraries)

*GNU General Public License v2.0*

-- Mario

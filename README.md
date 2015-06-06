# InputDebounce Arduino Library

Simple polling input debounce Arduino library.

Facts:
 - used for push-button like switches
 - delivers input value (state) after it has been stable (not flickering) for longer than the debounce period
 - delivers pressed-on time [ms]
 - handles input pin with:
    - external pull-down resistor
    - external pull-up resistor
    - internal pull-up resistor (default)

*GNU General Public License v2.0*

-- Mario

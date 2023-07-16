#include "JoystickKey.h"

#include <Arduino.h>

JoystickKey::JoystickKey(byte up_, byte right_, byte down_, byte left_, byte ok_, byte none_):
    _up_output(up_),
    _right_output(right_),
    _down_output(down_),
    _left_output(left_),
    _ok_output(ok_),
    _none_output(none_) 
    {
        trigger = _none_output;
        current = _none_output;
    }

void JoystickKey::detect(){
    adc_key_in = analogRead(A0);     // read the value from the sensor
    trigger = get_key(adc_key_in);  // convert into key press
    if (trigger != current)         // if keypress is detected
    {
        delay(50);                      // wait for debounce time
        adc_key_in = analogRead(A0);     // read the value from the sensor
        trigger = get_key(adc_key_in);  // convert into key press
        if (trigger != current)
        {
            current = trigger;
        }
    }
}

byte JoystickKey::get_key(unsigned int input){
    if (input < _left)
    return _left_output;
  else if (input < _ok)
    return _ok_output;
  else if (input < _down)
    return _down_output;
  else if (input < _right)
    return _right_output;
  else if (input < _up)
    return _up_output;
  else
    return _none_output;
}
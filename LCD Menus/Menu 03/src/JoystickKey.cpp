#include "JoystickKey.h"

#include <Arduino.h>
#include <GEM_u8g2.h>

JoystickKey::JoystickKey(){}

void JoystickKey::detect(){
    adc_key_in = analogRead(0);     // read the value from the sensor
    trigger = get_key(adc_key_in);  // convert into key press
    if (trigger != current)         // if keypress is detected
    {
        delay(50);                      // wait for debounce time
        adc_key_in = analogRead(0);     // read the value from the sensor
        trigger = get_key(adc_key_in);  // convert into key press
        if (trigger != current)
        {
            current = trigger;
        }
    }
}

byte JoystickKey::get_key(unsigned int input){
    if (input < _left)
    return GEM_KEY_LEFT;
  else if (input < _ok)
    return GEM_KEY_OK;
  else if (input < _down)
    return GEM_KEY_DOWN;
  else if (input < _right)
    return GEM_KEY_RIGHT;
  else if (input < _up)
    return GEM_KEY_UP;
  else
    return GEM_KEY_NONE;
}
#ifndef _JOYSTICK_KEY_H
#define _JOYSTICK_KEY_H

#include <Arduino.h>
#include <Gem_u8g2.h>

class JoystickKey {
    public:
        JoystickKey();

        byte trigger = GEM_KEY_NONE;
        byte current = GEM_KEY_NONE;

        void detect();

    // Convert ADC value to key number
    //          900
    //           |
    //   100 -- 300 -- 700
    //           |
    //          500
    //

    private:
        unsigned int adc_key_in;

        unsigned int _up    = 900;
        unsigned int _right = 700;
        unsigned int _down  = 500;
        unsigned int _left  = 100;
        unsigned int _ok    = 300;

        byte get_key(unsigned int input);
};

#endif
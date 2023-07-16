#ifndef _JOYSTICK_KEY_H
#define _JOYSTICK_KEY_H

#include <Arduino.h>

class JoystickKey {
    public:
        JoystickKey(byte up_, byte right_, byte down_, byte left_, byte ok_, byte none_);

        byte trigger = 0;
        byte current = 0;

        void detect();

    // Convert ADC value to key number
    //          900
    //           |
    //   100 -- 300 -- 700
    //           |
    //          500
    //

    private:
        int adc_key_in;

        unsigned int _up    = 900;
        unsigned int _right = 700;
        unsigned int _down  = 500;
        unsigned int _left  = 100;
        unsigned int _ok    = 300;

        byte _up_output;
        byte _right_output;
        byte _down_output;
        byte _left_output;
        byte _ok_output;
        byte _none_output;

        byte get_key(unsigned int input);
};

#endif
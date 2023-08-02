#ifndef _SUBMENU_TEST_H
#define _SUBMENU_TEST_H

#include <Arduino.h>
#include <Gem_u8g2.h>
#include <JoystickKey.h>

class SubMenuTest{
    public:
        U8G2 u8g2;
        GEM_u8g2 menu;

        JoystickKey joystickKey;

        SubMenuTest();
        void subMenu(); 

    private:
        static uint8_t _screen_redraw_required;

        void subMenuContextEnter();
        void subMenuContextLoop();
        void subMenuContextExit();

};

#endif
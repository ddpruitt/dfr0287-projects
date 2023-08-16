#include "DFR0287.h"

U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);

JoystickKey joystickKey;

uint8_t screen_redraw_required = 0;

void setupDFR0287()
{
    u8g2.begin();
    u8g2.setDisplayRotation(U8G2_R2);
    u8g2.setContrast(50);

    joystickKey.current = GEM_KEY_NONE;
    joystickKey.trigger = GEM_KEY_NONE;
}
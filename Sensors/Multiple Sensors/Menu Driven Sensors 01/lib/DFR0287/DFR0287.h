#ifndef _DFR0287_H
#define _DFR0287_H

#include <Arduino.h>
#include <U8g2lib.h>
#include "JoystickKey.h"

extern U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2;
extern JoystickKey joystickKey;
extern uint8_t screen_redraw_required;

void setupDFR0287();

#endif
#include <Arduino.h>
#include <U8g2lib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);
uint8_t menu_redraw_required = 0;

void setup()
{
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);
  u8g2.setContrast(190);

  menu_redraw_required = 1;
}

void loop()
{
  //int update_page = 0;
  if (menu_redraw_required != 0)
  {
    u8g2.firstPage();
    do
    {
      /* all graphics commands have to appear within the loop body. */
      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.drawStr(0, 20, "Hello World!");
      u8g2.drawStr(0, 40, "Hello World!");
      u8g2.drawStr(0, 60, "Hello World!");
    } while (u8g2.nextPage());
    menu_redraw_required = 0;
  }
}
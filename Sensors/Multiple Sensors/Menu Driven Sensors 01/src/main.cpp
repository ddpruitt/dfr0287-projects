#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;

int adc_key_in;
int key = -1;
int oldkey = -1;

// Convert ADC value to key number
//         4
//         |
//   0 --  1 -- 3
//         |
//         2
int get_key(unsigned int input)
{
  if (input < 100)
    return 0;
  else if (input < 300)
    return 1;
  else if (input < 500)
    return 2;
  else if (input < 700)
    return 3;
  else if (input < 900)
    return 4;
  else
    return -1;
}

void uiStep(void)
{

  adc_key_in = analogRead(0); // read the value from the sensor
  key = get_key(adc_key_in);  // convert into key press
  if (key != oldkey)          // if keypress is detected
  {
    delay(50);                  // wait for debounce time
    adc_key_in = analogRead(0); // read the value from the sensor
    key = get_key(adc_key_in);  // convert into key press
    if (key != oldkey)
    {
      oldkey = key;
      if (key >= 0)
      {
        //Serial.println(key);
        if (key == 0)
          uiKeyCodeFirst = KEY_BACK;
        else if (key == 1)
          uiKeyCodeFirst = KEY_SELECT;
        else if (key == 2)
          uiKeyCodeFirst = KEY_NEXT;
        else if (key == 4)
          uiKeyCodeFirst = KEY_PREV;
        else
          uiKeyCodeFirst = KEY_NONE;

        uiKeyCode = uiKeyCodeFirst;
      }
    }
  }
  delay(100);
}

#define MENU_ITEMS 6
char *menu_strings[MENU_ITEMS] = {"LCD12864 Shield", "www.DFRobot.com", "Temperature", "Humidity", "About", "OK"};

uint8_t menu_current = 0;
uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;

void drawMenu(void)
{
  uint8_t i, h;
  u8g2_uint_t w, d;

  u8g2.setFont(u8g_font_6x12); //4x6 5x7 5x8 6x10 6x12 6x13
  u8g2.setFontRefHeightText();
  u8g2.setFontPosTop();

  h = u8g2.getFontAscent() - u8g2.getFontDescent();
  w = u8g2.getWidth();
  for (i = 0; i < MENU_ITEMS; i++)
  {
    d = (w - u8g2.getStrWidth(menu_strings[i])) / 2;
    u8g2.setDrawColor(1);
    if (i == menu_current)
    {
      u8g2.drawBox(0, i * h + 1, w, h);
      u8g2.setDrawColor(0);
    }
    u8g2.drawStr(d, i * h + 1, menu_strings[i]);
  }
}

void updateMenu(void)
{
  switch (uiKeyCode)
  {
  case KEY_NEXT:
    menu_current++;
    if (menu_current >= MENU_ITEMS)
      menu_current = 0;
    menu_redraw_required = 1;
    break;
  case KEY_PREV:
    if (menu_current == 0)
      menu_current = MENU_ITEMS;
    menu_current--;
    menu_redraw_required = 1;
    break;
  }
  uiKeyCode = KEY_NONE;
}

void setup(void)
{
  u8g2.begin();

  // flip screen, if required
  u8g2.setDisplayRotation(U8G2_R2);

  // Testing shows that a contrast of 190 is acceptable.
  u8g2.setContrast(50);

  menu_redraw_required = 1; // force initial redraw
}

void loop(void)
{
  uiStep();     // check for key press
  updateMenu(); // update menu bar

  if (menu_redraw_required != 0)
  {
    u8g2.firstPage();
    do
    {
      drawMenu();
    } while (u8g2.nextPage());
    menu_redraw_required = 0;
  }
}

#include "SubMenuTest.h"

SubMenuTest::SubMenuTest(){}

void SubMenuTest::subMenuContextEnter()
{
  // Clear sreen
  u8g2.clear();
  _screen_redraw_required = 1;
  // Serial.println(F("Opening SubMenu 01"));
}

// Invoked every loop iteration
void SubMenuTest::subMenuContextLoop()
{
  // Serial.print(F("_screen_redraw_required: "));
  // Serial.print(_screen_redraw_required);

  // Serial.print(F(", trigger: "));
  // Serial.println(joystickKey.trigger);

  if (_screen_redraw_required != 0)
  {
    u8g2.firstPage();
    do
    {
      /* all graphics commands have to appear within the loop body. */
      u8g2.setFont(u8g2_font_ncenB10_tr); // font height 15, width 16
      u8g2.drawStr(0, 15, "SubMenu 01");

      // Manual mode.
      // Check pressed keys and navigate through frames accordingly
      // Detect key press manually using joystickKey
      joystickKey.detect();
      byte key = joystickKey.trigger;
      switch (key)
      {
      case GEM_KEY_OK:
        // Serial.println(F("Key OK - SubMenu Exit"));
        menu.context.exit();
        break;
      case GEM_KEY_RIGHT:
        u8g2.drawStr(0, 30, "Right");
        break;
      case GEM_KEY_LEFT:
        u8g2.drawStr(0, 30, "Left");
        break;
      case GEM_KEY_UP:
        u8g2.drawStr(0, 30, "Up");
        break;
      case GEM_KEY_DOWN:
        u8g2.drawStr(0, 30, "Down");
        break;
      }

    } while (u8g2.nextPage());
    _screen_redraw_required = 0;
  }
  delay(100);
  _screen_redraw_required = 1;
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
void SubMenuTest::subMenuContextExit()
{
  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();
  u8g2.setContrast(50);
}

void SubMenuTest::subMenu()
{
  menu.context.loop = []() { subMenuContextLoop(); };
  menu.context.enter = []() { subMenuContextEnter(); };
  menu.context.exit = []() { subMenuContextExit(); };
  menu.context.allowExit = false; // Setting to false will require manual exit from the loop
  menu.context.enter();
}
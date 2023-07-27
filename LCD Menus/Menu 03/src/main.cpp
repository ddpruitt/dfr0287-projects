#include "JoystickKey.h"

JoystickKey joystickKey;

U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);
uint8_t screen_redraw_required = 0;

void subMenu01(); // Forward declaration
void subMenu01ContextEnter();
void subMenu01ContextLoop();
void subMenu01ContextExit();

void subMenu02(); // Forward declaration
void subMenu02ContextEnter();
void subMenu02ContextLoop();
void subMenu02ContextExit();

GEMItem menuItemButton01("Test 01", subMenu01);
GEMItem menuItemButton02("Test 02", subMenu02);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("SubMenus and Pages");

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);

// ---
void setupMenu()
{
  menuPageMain.addMenuItem(menuItemButton01);
  menuPageMain.addMenuItem(menuItemButton02);
  menu.setMenuPageCurrent(menuPageMain);
}

void subMenu01ContextEnter()
{
  // Clear sreen
  u8g2.clear();
  screen_redraw_required = 1;
  // Serial.println(F("Opening SubMenu 01"));
}

// Invoked every loop iteration
void subMenu01ContextLoop()
{
  // Serial.print(F("screen_redraw_required: "));
  // Serial.print(screen_redraw_required);

  // Serial.print(F(", trigger: "));
  // Serial.println(joystickKey.trigger);

  if (screen_redraw_required != 0)
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
    screen_redraw_required = 0;
  }
  delay(100);
  screen_redraw_required = 1;
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
void subMenu01ContextExit()
{
  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();
  u8g2.setContrast(50);
}

// Setup context
void subMenu01()
{
  menu.context.loop = subMenu01ContextLoop;
  menu.context.enter = subMenu01ContextEnter;
  menu.context.exit = subMenu01ContextExit;
  menu.context.allowExit = false; // Setting to false will require manual exit from the loop
  menu.context.enter();
}

void subMenu02ContextEnter()
{
  // Clear sreen
  u8g2.clear();
  screen_redraw_required = 1;
  // Serial.println(F("Opening SubMenu 02"));
}

// Invoked every loop iteration
void subMenu02ContextLoop()
{

  if (screen_redraw_required != 0)
  {
    u8g2.firstPage();
    do
    {
      /* all graphics commands have to appear within the loop body. */
      u8g2.setFont(u8g2_font_ncenB10_tr); // font height 15, width 16
      u8g2.drawStr(0, 15, "SubMenu 02");

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
    screen_redraw_required = 0;
  }
  delay(100);
  screen_redraw_required = 1;
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
void subMenu02ContextExit()
{
  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();
  u8g2.setContrast(50);
}

// Setup context
void subMenu02()
{
  menu.context.loop = subMenu02ContextLoop;
  menu.context.enter = subMenu02ContextEnter;
  menu.context.exit = subMenu02ContextExit;
  menu.context.allowExit = false; // Setting to false will require manual exit from the loop
  menu.context.enter();
}

void setup()
{
  Serial.begin(9600);

  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);
  u8g2.setContrast(50);

  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();

  joystickKey.current = GEM_KEY_NONE;
  joystickKey.trigger = GEM_KEY_NONE;
}

void loop()
{
  if (menu.readyForKey())
  {
    joystickKey.detect();
    menu.registerKeyPress(joystickKey.trigger);
  }
  delay(150);
}

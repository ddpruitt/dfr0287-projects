#include <Arduino.h>
#include <U8g2lib.h>
#include <DFRobot_DHT11.h>
#include "JoystickKey.h"
#include "DFR0287.h"

uint8_t temp_refresh_required = 0;


#define DHT11_PIN 12 // Digital pin connected to the DHT sensor
DFRobot_DHT11 DHT;
int dht11_temp = 0;
int dht11_hum = 0;
uint16_t tm35_temp = 0;

// min delay is ~1110
uint32_t delayMS = 2000;

void subMenu01(); // Forward declaration
void subMenu01ContextEnter();
void subMenu01ContextLoop();
void subMenu01ContextExit();

void subMenu02(); // Forward declaration
void subMenu02ContextEnter();
void subMenu02ContextLoop();
void subMenu02ContextExit();

void read_dht11(void);

GEMItem menuItemButton01("DHT11 & TM35", subMenu01);
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
  temp_refresh_required = 0;
  // Serial.println(F("Opening SubMenu 01"));
}

// Invoked every loop iteration
void subMenu01ContextLoop()
{
  if (screen_redraw_required != 0)
  {
    u8g2.firstPage();
    do
    {
      /* all graphics commands have to appear within the loop body. */
      // this next line needs a delay of ~1110ms to work
      read_dht11();

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
      default:
        break;
      }

    } while (u8g2.nextPage());
    screen_redraw_required = 0;
  }
  //delay(1110);
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
  Serial.begin(115200);

  setupDFR0287();

  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();

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


void read_dht11(void)
{
  u8g2.setFont(u8g2_font_ncenB10_tr); // font height 15, width 16
  u8g2.drawStr(0, 0, "DHT11 & TM35");

  u8g2.setFont(u8g2_font_courR10_tr); // font height 14, width 10

  DHT.read(DHT11_PIN);

  if(temp_refresh_required <= 0)
  {
    dht11_temp = DHT.temperature;
    dht11_hum = DHT.humidity;
    tm35_temp = analogRead(A1); //Connect LM35 on Analog 1
    temp_refresh_required = 15;
  }
  else
  {
    temp_refresh_required--;
  }


  char humidityResult[5];
  dtostrf(dht11_hum, 4, 1, humidityResult);
  u8g2.drawStr(0, 15, "Humidity:");
  u8g2.drawStr(90, 15, humidityResult);

  char temperatureResult[5];
  dtostrf((1.8*dht11_temp + 32), 4, 1, temperatureResult);
  u8g2.drawStr(0, 30, "DHT11:");
  u8g2.drawStr(90, 30, temperatureResult);
  
  char tm35TempResult[8]; // Buffer big enough for 7-character float
  dtostrf((1.8*((double) tm35_temp * (5/10.24)) + 32), 4, 1, tm35TempResult);
  u8g2.drawStr(0, 45, "LM35:");
  u8g2.drawStr(90, 45, tm35TempResult);


}
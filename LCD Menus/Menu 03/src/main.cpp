#include "JoystickKey.h"

JoystickKey joystickKey;

U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);


// Create menu button that will trigger rock() function. It will run animation sequence.
// We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void rock(); // Forward declaration
GEMItem menuItemButton("Open SubMenu 01", rock);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("Submenus and Pages");

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);

// ---
void setupMenu() {
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemButton);

  // Add menu page to menu and set it as current
  menu.setMenuPageCurrent(menuPageMain);
}

void setup() {
  // Serial communication setup
  Serial.begin(9600);

  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);
  u8g2.setContrast(50);
  
  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();
}


void loop() {
  joystickKey.detect();
  menu.registerKeyPress(joystickKey.trigger);
  delay(150);
}

void rockContextEnter() {
  // Clear sreen
  u8g2.clear();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(0, 15, "SubMenu 01");
  u8g2.nextPage();
  Serial.println("Opening SubMenu 01");
}

// Invoked every loop iteration
void rockContextLoop() {
  // Manual mode.
  // Check pressed keys and navigate through frames accordingly
  // Detect key press manually using joystickKey
  joystickKey.detect();
  byte key = joystickKey.trigger;
  switch (key) {
    case GEM_KEY_OK:
    Serial.println("Key OK - SubMenu Exit");
      menu.context.exit();
      break;
    case GEM_KEY_RIGHT:
      Serial.println("Right");
      break;
    case GEM_KEY_LEFT:
      Serial.println("Left");
      break;
    case GEM_KEY_UP:
      Serial.println("Up");
      break;
    case GEM_KEY_DOWN:
      Serial.println("Down");
      break;
  }
}

// Invoked once when the GEM_KEY_CANCEL key is pressed
void rockContextExit() {
  // Draw menu back on screen and clear context
  menu.reInit();
  menu.drawMenu();
  menu.clearContext();
  u8g2.setContrast(50);
}

// Setup context
void rock() {
  menu.context.loop = rockContextLoop;
  menu.context.enter = rockContextEnter;
  menu.context.exit = rockContextExit;
  menu.context.allowExit = false; // Setting to false will require manual exit from the loop
  menu.context.enter();
}


/*
  Basic menu example using GEM library.

  Simple one page menu with one editable menu item associated with int variable, one with boolean variable,
  and a button, pressing of which will result in int variable value printed to Serial monitor if boolean variable is set to true.

  U8g2lib library is used to draw menu and to detect push-buttons presses.
  
  Additional info (including the breadboard view) available on GitHub:
  https://github.com/Spirik/GEM
  
  This example code is in the public domain.
*/
#include <Arduino.h>
#include <KeyDetector.h>
#include <GEM_u8g2.h>

const byte potPin = A0; 
//Key keys[] = {{GEM_KEY_LEFT, potPin, 100}, {GEM_KEY_OK, potPin, 300}, {GEM_KEY_DOWN, potPin, 500}, {GEM_KEY_RIGHT, potPin, 700}, {GEM_KEY_UP, potPin, 900}};
Key keys[] = {{GEM_KEY_LEFT, potPin, 50}, {GEM_KEY_OK, potPin, 250}, {GEM_KEY_DOWN, potPin, 450}, {GEM_KEY_RIGHT, potPin, 650}, {GEM_KEY_UP, potPin, 850}};

KeyDetector myKeyDetector(keys, sizeof(keys)/sizeof(Key), 0, 50);

// Create an instance of the U8g2 library.
// Use constructor that matches your setup (see https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for details).
// This instance is used to call all the subsequent U8g2 functions (internally from GEM library,
// or manually in your sketch if it is required).
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);


// Create variables that will be editable through the menu and assign them initial values
int number = -512;
boolean enablePrint = false;

// Create two menu item objects of class GEMItem, linked to number and enablePrint variables 
GEMItem menuItemInt("Number:", number);
GEMItem menuItemBool("Enable print:", enablePrint);

// Create menu button that will trigger printData() function. It will print value of our number variable
// to Serial monitor if enablePrint is true. We will write (define) this function later. However, we should
// forward-declare it in order to pass to GEMItem constructor
void printData(); // Forward declaration
GEMItem menuItemButton("Print", printData);

// Create menu page object of class GEMPage. Menu page holds menu items (GEMItem) and represents menu level.
// Menu can have multiple menu pages (linked to each other) with multiple menu items each
GEMPage menuPageMain("Main Menu");

// Create menu object of class GEM_u8g2. Supply its constructor with reference to u8g2 object we created earlier
GEM_u8g2 menu(u8g2);

void setupMenu() {
  // Add menu items to menu page
  menuPageMain.addMenuItem(menuItemInt);
  menuPageMain.addMenuItem(menuItemBool);
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

int adc_key_in = 0;

void loop() {

  // adc_key_in = analogRead(0);
  // Serial.println(adc_key_in);

  // myKeyDetector.detect();
  // Serial.println(myKeyDetector.trigger);
  // If menu is ready to accept button press...
  if (menu.readyForKey()) {
    myKeyDetector.detect();
    Serial.print(myKeyDetector.current);
    Serial.print("\t");
    Serial.println(myKeyDetector.trigger);
    menu.registerKeyPress(myKeyDetector.trigger);
  }
  delay(150);
}

void printData() {
  // If enablePrint flag is set to true (checkbox on screen is checked)...
  if (enablePrint) {
    // ...print the number to Serial
    Serial.print("Number is: ");
    Serial.println(number);
  } else {
    Serial.println("Printing is disabled, sorry:(");
  }
}
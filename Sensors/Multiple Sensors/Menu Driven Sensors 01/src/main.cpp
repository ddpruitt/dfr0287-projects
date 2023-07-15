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

#include <GEM_u8g2.h>


// Create an instance of the U8g2 library.
// Use constructor that matches your setup (see https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for details).
// This instance is used to call all the subsequent U8g2 functions (internally from GEM library,
// or manually in your sketch if it is required).
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);


int adc_key_in;
byte key = GEM_KEY_NONE;
byte oldkey = GEM_KEY_NONE;

/*
menu.registerKeyPress(GEM_KEY_NONE); // Do nothing

menu.registerKeyPress(GEM_KEY_UP); // Navigate up through the menu items list,
                                   // select next value of the digit/char
                                   // of editable variable, or previous option in select

menu.registerKeyPress(GEM_KEY_RIGHT); // Navigate through the link to another (child) menu page,
                                      // select next digit/char of editable variable,
                                      // execute code associated with button

menu.registerKeyPress(GEM_KEY_DOWN); // Navigate down through the menu items list,
                                     // select previous value of the digit/char
                                     // of editable variable, or next option in select

menu.registerKeyPress(GEM_KEY_LEFT); // Navigate through the Back button to the previous menu page,
                                     // select previous digit/char of editable variable

menu.registerKeyPress(GEM_KEY_CANCEL); // Navigate to the previous (parent) menu page,
                                       // exit edit mode without saving the variable,
                                       // exit context loop if allowed within context's settings

menu.registerKeyPress(GEM_KEY_OK); // Toggle boolean menu item, enter edit mode
                                   // of the associated non-boolean variable,
                                   // exit edit mode with saving the variable,
                                   // execute code associated with button
*/

// Convert ADC value to key number
//         4
//         |
//   0 --  1 -- 3
//         |
//         2
byte get_key(unsigned int input)
{
  if (input < 100)
    return GEM_KEY_LEFT;
  else if (input < 300)
    return GEM_KEY_OK;
  else if (input < 500)
    return GEM_KEY_DOWN;
  else if (input < 700)
    return GEM_KEY_RIGHT;
  else if (input < 900)
    return GEM_KEY_UP;
  else
    return GEM_KEY_NONE;
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
    }
  }
  delay(100);
}


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
  Serial.begin(115200);

  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);
  u8g2.setContrast(50);

  // Menu init, setup and draw
  menu.init();
  setupMenu();
  menu.drawMenu();
}


void loop() {

  uiStep(); 
  menu.registerKeyPress(key);
  // If menu is ready to accept button press...
  // if (menu.readyForKey()) {

  //   menu.registerKeyPress();
  // }

  delay(100);
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
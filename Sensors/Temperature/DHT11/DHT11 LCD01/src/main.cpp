#include <Arduino.h>
#include <U8g2lib.h>
#include <DFRobot_DHT11.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);
uint8_t menu_redraw_required = 0;

#define DHT11_PIN 12 // Digital pin connected to the DHT sensor
DFRobot_DHT11 DHT;

// min delay is ~1110
uint32_t delayMS = 2000;

void read_dht11(void)
{
  u8g2.setFont(u8g2_font_courR10_tr); // font height 14, width 10

  uint32_t startRead = 0;
  uint32_t stopRead = 0;

  startRead = micros();
  DHT.read(DHT11_PIN);

  stopRead = micros();

  char humidityResult[5];
  dtostrf(DHT.humidity, 4, 1, humidityResult);
  u8g2.drawStr(0, 30, "Humidity:");
  u8g2.drawStr(90, 30, humidityResult);

  char temperatureResult[5];
  dtostrf(DHT.temperature, 4, 1, temperatureResult);
  u8g2.drawStr(0, 45, "Temp:");
  u8g2.drawStr(90, 45, temperatureResult);

  char result[8]; // Buffer big enough for 7-character float
  dtostrf(stopRead - startRead, 6, 2, result);

  u8g2.drawStr(0, 60, result);
  Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print(" humidity:");
  Serial.println(DHT.humidity);
}

void setup()
{
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);
  u8g2.setContrast(50);

  menu_redraw_required = 1;
}

void loop()
{
  if (menu_redraw_required != 0)
  {
    u8g2.firstPage();
    do
    {
      /* all graphics commands have to appear within the loop body. */
      u8g2.setFont(u8g2_font_ncenB10_tr); // font height 15, width 16
      u8g2.drawStr(0, 15, "DHT11 Sensor");

      read_dht11();

    } while (u8g2.nextPage());
    menu_redraw_required = 0;
  }
  delay(delayMS);
  menu_redraw_required = 1;
}
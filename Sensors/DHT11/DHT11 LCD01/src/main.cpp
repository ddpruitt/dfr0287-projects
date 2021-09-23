#include <Arduino.h>
#include <U8g2lib.h>
#include <dht.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_ST7565_NHD_C12864_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* cs=*/10, /* dc=*/9, /* reset=*/8);
uint8_t menu_redraw_required = 0;

#define DHT11_PIN 12 // Digital pin connected to the DHT sensor
dht DHT;
uint32_t delayMS;

void setup()
{
  Serial.begin(9600);

  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type,\tstatus,\t\tHumidity (%),\tTemperatur (C),\tT_Read (us)\tT_Cycle (us)\tT_Delay (us)");

  // u8g2.begin();
  // u8g2.setDisplayRotation(U8G2_R2);
  // u8g2.setContrast(190);

  // menu_redraw_required = 1;

}

// min delay is ~1110
int del = 1500;
uint32_t startRead = 0;
uint32_t stopRead = 0;
uint32_t startCycle = 0;
uint32_t stopCycle = 0;
uint32_t tempCycle = 0;

void loop()
{
  while (del > 0)
  {
    // READ DATA
    Serial.print("DHT11, \t");

    startRead = micros();
    int chk = DHT.read11(DHT11_PIN);
    stopRead = micros();

    switch (chk)
    {
    case DHTLIB_OK:
      Serial.print("OK,\t\t");
      del -= 10;
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Time out,\t");
      del += 10;
      break;
    case DHTLIB_ERROR_CONNECT:
      Serial.print("Connect,\t");
      del += 10;
      break;
    case DHTLIB_ERROR_ACK_L:
      Serial.print("Ack Low,\t");
      break;
    case DHTLIB_ERROR_ACK_H:
      Serial.print("Ack High,\t");
      break;
    default:
      Serial.print("Unknown,\t");
      break;
    }
    // DISPLAY DATA
    Serial.print(DHT.humidity, 1);
    Serial.print(",\t\t");
    Serial.print(DHT.temperature, 1);
    Serial.print(",\t\t");
    Serial.print(stopRead - startRead);
    Serial.print(",\t\t");
    stopCycle = micros();
    tempCycle = micros();
    Serial.print(stopCycle - startCycle);
    startCycle = tempCycle;
    Serial.print(",\t\t");
    Serial.print(del);
    Serial.print("000");
    Serial.println();

    delay(del);
  }
  while (1)
    ;
  //int update_page = 0;
  // if (menu_redraw_required != 0)
  // {
  //   u8g2.firstPage();
  //   do
  //   {
  //     /* all graphics commands have to appear within the loop body. */
  //     u8g2.setFont(u8g2_font_ncenB14_tr);
  //     u8g2.drawStr(0, 20, "DHT11 Sensor");

  //     u8g2.setFont(u8g2_font_courR10_tr); // font height 14, width 10
  //     delay(delayMS);

  //   } while (u8g2.nextPage());
  //   menu_redraw_required = 0;
  // }
}
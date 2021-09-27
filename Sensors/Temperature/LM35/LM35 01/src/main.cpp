#include <Arduino.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint16_t val;
    double dat;
    val=analogRead(A1);//Connect LM35 on Analog 1
    dat = (double) val * (5/10.24);
    Serial.print("Tep:"); //Display the temperature on Serial monitor
    Serial.print(dat);
    Serial.println("C");
    delay(500);
}
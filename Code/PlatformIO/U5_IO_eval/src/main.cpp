#include <Arduino.h>

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED_BLUE, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BLUE, HIGH);
  delay(1000);
  digitalWrite(LED_BLUE, LOW);
  delay(1000);
}
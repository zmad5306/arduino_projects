#include <SPI.h>
#include "Adafruit_MAX31855.h"

#define MAXDO   3
#define MAXCS   4
#define MAXCLK  5
#define FAN     11

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

const int MIN_TEMP  = 75;
const int TARGET_TEMP = 250;
const int MIN_PWM = 100;
int pwmValue = 0;

void setup() {
  Serial.begin(9600);
 
  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc

  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);

  pinMode(FAN, OUTPUT);
}

void loop() {
   Serial.print("Internal Temp = ");
   Serial.println(thermocouple.readInternal());

   double c = thermocouple.readCelsius();
   double f = thermocouple.readFarenheit();
   if (isnan(c)) {
     Serial.println("Something wrong with thermocouple!");
   } else {
     Serial.print("C = "); 
     Serial.println(c);
     Serial.print("F = ");
     Serial.println(f);
   }

   if (f >= TARGET_TEMP) {
    pwmValue = 0;
   } else if (f <= MIN_TEMP) {
    pwmValue = 255;
   } else {
    pwmValue = map(f, TARGET_TEMP, MIN_TEMP, MIN_PWM, 255);
   }

   Serial.print("PWM = ");
   Serial.println(pwmValue);

   analogWrite(FAN, pwmValue);
 
   delay(500);
}
#include <SPI.h>
#include "Adafruit_MAX31855.h"

//////////////////////
////PIN/WIRE COLOR////
//////////////////////
//MAXDO   3     BLUE
//MAXCS   4     YELLOW
//MAXCLK  5     WHITE
//FAN     11    GREEN
//GROUND  GND   BLACK
//POWER   5V    RED
//TEMP    A0    YELLOW
//////////////////////

#define MAXDO   3
#define MAXCS   4
#define MAXCLK  5
#define FAN     11
#define TEMP    A0

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

const int MIN_TEMP  = 200;
const int MAX_TEMP = 350;
const int MIN_PWM = 125;
const int MAX_PWM = 255;
const int REFRESH_INTERVAL = 1000;
const int MAX_TEMP_VAL = 1023;
const int MIN_TEMP_VAL = 0;
int pwmValue = 0;

void setup() {
  Serial.begin(9600);
 
  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc

  // wait for MAX chip to stabilize
  delay(500);

  pinMode(FAN, OUTPUT);
}

void loop() {
   Serial.print("Internal Temp = ");
   Serial.println(thermocouple.readInternal());

   int sensorValue = analogRead(TEMP);
   Serial.print("sensor value = ");
   Serial.println(sensorValue);

   int targetTemp = map(sensorValue, MIN_TEMP_VAL, MAX_TEMP_VAL, MIN_TEMP, MAX_TEMP);

   Serial.print("target temp = ");
   Serial.println(targetTemp);

   double c = thermocouple.readCelsius();
   double f = thermocouple.readFarenheit();
   
   if (isnan(c) || isnan(f)) {
     Serial.println("Something wrong with thermocouple!");
   } else {
     Serial.print("C = "); 
     Serial.println(c);
     Serial.print("F = ");
     Serial.println(f);

     if (f >= targetTemp) {
      pwmValue = MIN_PWM;
     } else if (f <= MIN_TEMP) {
      pwmValue = MAX_PWM;
     } else {
      pwmValue = map(f, targetTemp, MIN_TEMP, MIN_PWM, MAX_PWM);
     }
  
     Serial.print("PWM = ");
     Serial.println(pwmValue);

//////////////////////////////////
////////FOR SERIAL PLOTTER////////
//////////////////////////////////
//     Serial.print(f);
//     Serial.print(",");
//     Serial.println(pwmValue);
//////////////////////////////////

    Serial.println("-----------------------------------------");
  
     analogWrite(FAN, pwmValue);
   }

   delay(REFRESH_INTERVAL);
}

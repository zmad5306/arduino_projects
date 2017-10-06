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
//TEMP    A0    ORANGE
//////////////////////

#define MAXDO   3
#define MAXCS   4
#define MAXCLK  5
#define FAN     6
#define TEMP    A0

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

const int MIN_TEMP  = 200;
const int MAX_TEMP = 350;
const int MIN_PWM = 125;
const int MAX_PWM = 255;
const int MAX_TEMP_VAL = 1023;
const int MIN_TEMP_VAL = 0;

int readTargetTemp() {
  int ttv = analogRead(TEMP);
  return map(ttv, MIN_TEMP_VAL, MAX_TEMP_VAL, MIN_TEMP, MAX_TEMP);
}

void pwm(double currentTemp, int targetTemp) {
  int pwmValue = 0;
  if (isnan(currentTemp)) {
    Serial.println("Something wrong with thermocouple!");
  } else {
    if (currentTemp >= targetTemp) {
      pwmValue = MIN_PWM;
    } else if (currentTemp <= MIN_TEMP) {
      pwmValue = MAX_PWM;
    } else {
      pwmValue = map(currentTemp, targetTemp, MIN_TEMP, MIN_PWM, MAX_PWM);
    }
  }

  analogWrite(FAN, pwmValue);
}

void printSerial(double currentTemp, int targetTemp) {
//////////////////////////////////////////////////////////
///////////////////Serial Monitor/////////////////////////
//////////////////////////////////////////////////////////
//  Serial.print("target temp: ");
//  Serial.print(targetTemp);
//  Serial.println("°F");
//  Serial.print("current temp: ");
//  Serial.print(currentTemp);
//  Serial.println("°F");
//  Serial.println("--------------------------");

//////////////////////////////////////////////////////////
///////////////////Serial Plotter/////////////////////////
//////////////////////////////////////////////////////////
  Serial.print(targetTemp);
  Serial.print(",");
  Serial.println(currentTemp);
}

void setup() {
  Serial.begin(9600);

  while (!Serial) delay(1);
  delay(500);
  pinMode(FAN, OUTPUT);
}

void loop() {
  int targetTemp = readTargetTemp();
  double currentTemp = thermocouple.readFarenheit();
  pwm(currentTemp, targetTemp);
  printSerial(currentTemp, targetTemp);
}

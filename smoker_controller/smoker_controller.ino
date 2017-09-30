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
  int itc = thermocouple.readInternal();
  int internalTemp = itc * 9 / 5 + 32;

  int ttv = analogRead(TEMP);
  int targetTemp = map(ttv, MIN_TEMP_VAL, MAX_TEMP_VAL, MIN_TEMP, MAX_TEMP);
  
  double currentTemp = thermocouple.readFarenheit();

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

  Serial.print("target temp (analog): ");
  Serial.println(ttv);
  Serial.print("target pwm: ");
  Serial.println(pwmValue);
  Serial.print("target temp: ");
  Serial.print(targetTemp);
  Serial.println("°F");
  Serial.print("current temp: ");
  Serial.print(currentTemp);
  Serial.println("°F");
  Serial.print("internal temp: ");
  Serial.print(internalTemp);
  Serial.println("°F");
  Serial.println("------------------------------------------");

  delay(REFRESH_INTERVAL);
}

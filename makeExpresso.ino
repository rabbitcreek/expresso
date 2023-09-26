/***************************************************
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>
#include "Adafruit_MAX31855.h"
#define THRESHOLD   5000  /* Lower the value, more the sensitivity */
// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:
touch_pad_t touchPin;
// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   D3
#define MAXCS   D4
#define MAXCLK  D5
const int ledPin = D2;  // 16 corresponds to GPIO16
float fTemp = 0.0;
double fTime = 0.0;
// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
 
int change = 0;
// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS);

// Example creating a thermocouple instance with hardware SPI
// on SPI1 using specified CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS, SPI1);

void setup() {
  Serial.begin(9600);
  touchSleepWakeUpEnable(T2,THRESHOLD);
 // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  while (!Serial) delay(1); // wait for Serial on Leonardo/Zero, etc

  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);
  Serial.print("Initializing sensor...");
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }

  // OPTIONAL: Can configure fault checks as desired (default is ALL)
  // Multiple checks can be logically OR'd together.
  // thermocouple.setFaultChecks(MAX31855_FAULT_OPEN | MAX31855_FAULT_SHORT_VCC);  // short to GND fault is ignored

  Serial.println("DONE.");
  //pinMode(D2, OUTPUT);
  //digitalWrite(D2,LOW);
 fTime = millis();
}

void loop() {
  // basic readout test, just print the current temp
   //Serial.print("Internal Temp = ");
   //Serial.println(thermocouple.readInternal());

  // double c = thermocouple.readCelsius();
   fTemp = thermocouple.readFahrenheit();
   Serial.print("F = ");
   Serial.println(thermocouple.readFahrenheit());
   if (fTemp < 195.0){
   float val = (exp(sin(millis()/2000.0 * PI)) - 0.368) * 108.0;
  ledcWrite(ledChannel, val);  
   } else if (fTemp >= 195) {
     ledcWrite(ledChannel, 250);
   }
if( millis() - fTime > 1000 * 10 * 60 ){
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();
}
   delay(50);
}
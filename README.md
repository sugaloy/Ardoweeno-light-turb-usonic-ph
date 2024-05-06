This is a small code to fetch sensor data, separated by function and output it on Serial Monitor with 9600 rate

These are the includes you need : 
#include <Wire.h>
#include <BH1750.h>
#include<SoftwareSerial.h>
BH1750 lightMeter(0x23);
//=========================

//SENSOR : ULTRASONIC
#define TRIG 2 //DIGITAL PINOUT
#define ECHO 3
//=========================

//SENSOR : TURBIDITY
//Nothing
//=========================

//SENSOR : pH & Temp
#include<OneWire.h>
#include<DallasTemperature.h>

Within the loop() function there is simple algorithm to determine if water tank needs water flow from solenoid.

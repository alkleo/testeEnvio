#include "BluetoothSerial.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define BAT 34
#define DS18B20 32

float vbat = 0;
String temperature = "";
float offSetBat = -0.1; 


BluetoothSerial BT;
OneWire oneWire(DS18B20);
DallasTemperature sensors(&oneWire);


void setup() {
  pinMode(BAT, INPUT);
  Serial.begin(115200);
  BT.begin("ESP32-2-24-04-21");
}

void loop() {
 
  sensors.requestTemperatures(); 
  temperature = String(sensors.getTempCByIndex(0), 1) + "C | ";
  vbat = batteryReading();    
  BT.println(temperature + String(vbat, 2) + " V");
  delay(1000);
}

double batteryReading() {

  double x = 0;

  for (int i = 0; i < 50; i++) {
   x += analogRead(BAT) * 6.3 / 4095;
   delay(2);
 }

  x = x / 50;

  Serial.println(x);
  
  double terms[] = {
  5.4203694131221816e+001,
-8.1833298884089885e+001,
 4.6969966804441377e+001,
-1.1704519005988191e+001,
 1.0891834549235506e+000

};
  
  double t = 1;
  double r = 0;
  for (double c : terms) {
    r += c * t;
    t *= x;
  }

  r = r + offSetBat;
  
  return r;
  }

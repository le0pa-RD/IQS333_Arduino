//IQS333 to Arduino Test Code (Sample Code) to get started
/*
Connect IQS333 module to Arduino with following pinout
IQS333 Module     Arduino
GND               GND
VDDHI             3.3V
SDA               SDA
SCL               SCL
RDY               Pin 2
*/

#include <Wire.h>
#include "IQS333.h"

void setup() {
  // put your setup code here, to run once:
  //Setup PINS for IQS Device
  pinMode(RDY, INPUT);          // Set Pin2 (RDY) as Input

  // Begin Setup  
  Wire.begin();    
  Serial.begin(115200);
  Serial.println("Begin");     
  delay(10); 

  IQS333_setup();
}

void loop() {
  
 int *sensor_values = IQS333_read();

  // Start printing only if there is proximity detection
  if(sensor_values[0] != 0){ 
    Serial.print(sensor_values[0]);
    Serial.print(" : ");
    Serial.print(sensor_values[1]);
    Serial.print(" : ");
    Serial.print(sensor_values[2]);
    Serial.print(" : ");
    Serial.print(sensor_values[3]);
    Serial.print(" : ");
    Serial.print(sensor_values[4]);
    Serial.print(" : ");
    Serial.print(sensor_values[5]);
    Serial.print(" : ");
    Serial.print(sensor_values[6]);
    Serial.print(" : ");
    Serial.print(sensor_values[7]);    
    Serial.println(" ");
  }
}

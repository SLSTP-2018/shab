/*
  SHAB.ino runs two linear actuators based on altitude
  Copyright (C) 2018  Alex Hyer, Andrew Jong
 
  This file is part of SHAB.

  SHAB is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  SHAB is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with SHAB.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <Arduino.h>
#include "MS5xxx.h"
#include "SHAB.h"
#include <Wire.h>

MS5xxx sensor(&Wire);

// the pins must be next to each other for the powerLinearActuator to work
int LA_1_EXTEND_PIN = 2;
int LA_1_RETRACT_PIN = 3;
int LA_2_EXTEND_PIN = 4;
int LA_2_RETRACT_PIN = 5;

// time to power the linear actuator to fully perform its extend or retract action
int LA_POWER_MILLISECONDS = 5000;

void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);
  LinearActuator tropo (2, 3, 4, 5);
  LinearActuator strato (8, 9, 10, 11);

  if(sensor.connect() > 0) {
    Serial.println("Error connecting...");
    delay(500);
    setup();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor.ReadProm();
  sensor.Readout();
  //Serial.print("Temperature [0.01 C]: ");
  //Serial.println(sensor.GetTemp());
  //Serial.print("Pressure [Pa]: ");
  //Serial.println(sensor.GetPres());
  test_crc();
  //Serial.println(PascalToMeter(sensor.GetPres()));
  //Serial.println("---");
  delay(500);
}

void test_crc() {
  sensor.ReadProm();
  sensor.Readout(); 
  Serial.print("CRC=0x");
  Serial.print(sensor.Calc_CRC4(), HEX);
  Serial.print(" (should be 0x");
  Serial.print(sensor.Read_CRC4(), HEX);
  Serial.print(")\n");
  Serial.print("Test Code CRC=0x");
  Serial.print(sensor.CRCcodeTest(), HEX);
  Serial.println(" (should be 0xB)");
}

void powerLinearActuator(boolean useLinAct1, boolean isExtend){
  // choose which pin to use
  int pin = useLinAct1 ? LA_1_EXTEND_PIN : LA_2_EXTEND_PIN;
  // if isExtend, we already have the correct pin. if retract, it's the next pin
  pin = isExtend ? pin : pin + 1;
  // perform the pin action
  digitalWrite(pin, HIGH);
  delay(LA_POWER_MILLISECONDS);
}

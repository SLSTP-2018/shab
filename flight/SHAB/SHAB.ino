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

void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);

  // Intitialize LinearActuators
  LinearActuator tropo (2, 3, 4, 5);
  LinearActuator strato (8, 9, 10, 11);

  // Retract arms in case they are extended
  tropo.retract();
  strato.retract();

  // Run actuator tests
  tropo.extend();
  delay(1000);
  tropo.retract();
  delay(1000);
  strato.extend();
  delay(1000);
  strato.retract();

  // See if altimeter is responding with CRC
  if(sensor.connect() > 0) {
    Serial.println("Error connecting...");
    delay(500);
    setup();
  } else if(CRC_Valid(sensor) == false) {
    Serial.println("CRC failure...");
    delay(500);
    setup();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor.ReadProm();
  sensor.Readout();
  int altitude = PascalToMeter(sensor.GetPres());
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

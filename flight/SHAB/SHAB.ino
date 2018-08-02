/*
  SHAB.ino runs two linear actuators based on altitude
  Copyright (C) 2018  Alex Hyer
 
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
#include "libraries/SHAB.h"

// the pins must be next to each other for the powerLinearActuator to work
int LA_1_EXTEND_PIN = 2;
int LA_1_RETRACT_PIN = 3;
int LA_2_EXTEND_PIN = 4;
int LA_2_RETRACT_PIN = 5;

// time to power the linear actuator to fully perform its extend or retract action
int LA_POWER_MILLISECONDS = 5000;

void setup() {
  // put your setup code here, to run once:  
  pinMode(SDA, INPUT);  // This line may not be needed
  pinMode(SCL, OUTPUT);  // This line may not be needed

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    Serial.println(Serial.read());
  }
}

void powerLinearActuator(boolean useLinAct1, boolean isExtend){
  // choose which pin to use
  int pin = useLinAct1 ? LA_1_EXTEND_PIN : LA_2_EXTEND_PIN;
  // if isExtend, we already have the correct pin. if retract, it's the next pin
  pin = isExtend ? pin : pin + 1;
  // perform the pin action
  DigitalWrite(pin, HIGH);
  delay(LA_POWER_MILLISECONDS);
}


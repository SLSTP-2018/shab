/*
  LinearActuator.cpp defines LinearActuator class implementation details
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
#include "SHAB.h"

LinearActuator::LinearActuator(int f, int r) : fpin(f), rpin(r) {
  pinMode(fpin, OUTPUT);
  pinMode(rpin, OUTPUT);
}

void LinearActuator::extend() {
  if(extended == false and has_extended == false) {
    digitalWrite(fpin, HIGH);
    digitalWrite(rpin, LOW);

    delay(30000);

    digitalWrite(fpin, LOW);
    digitalWrite(rpin, LOW);

    extended = true;
    has_extended = true;
  };
}

bool LinearActuator::get_extended() {
  Serial.println(fpin);
  Serial.println(rpin);
  return true;
}

void LinearActuator::retract() {
  if(extended == true) {
    Serial.println(fpin);
    Serial.println(rpin);
    digitalWrite(fpin, LOW);
    digitalWrite(rpin, HIGH);

    delay(30000);

    digitalWrite(fpin, LOW);
    digitalWrite(rpin, LOW);

    extended = false;
  };
}

void LinearActuator::self_test() {
  // In-case they are extended
  extended = true;
  Serial.println(extended);
  Serial.println(has_extended);
  Serial.println("Retracting");
  retract();
  delay(1000);

  // Extension test
  Serial.println("Extending");
  extend();
  delay(3000);
  retract();
  has_extended = false;
}

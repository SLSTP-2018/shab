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

LinearActuator::LinearActuator (int f1, int f2, int r1, int r2){
  int fb_pin = f1;  // Forward Before Pin
  int fa_pin = f2;  // Forward After Pin
  int rb_pin = r1;  // Reverse Before Pin
  int ra_pin = r2;  // Reverse After Pin

  pinMode(fb_pin, OUTPUT);
  pinMode(fa_pin, OUTPUT);
  pinMode(rb_pin, OUTPUT);
  pinMode(ra_pin, OUTPUT);
}

void LinearActuator::extend(){
  if(extended == false) {
    digitalWrite(fb_pin, HIGH);
    digitalWrite(fa_pin, HIGH);
    digitalWrite(rb_pin, LOW);
    digitalWrite(ra_pin, LOW);

    delay(10000);

    digitalWrite(fb_pin, LOW);
    digitalWrite(fa_pin, LOW);
    digitalWrite(rb_pin, LOW);
    digitalWrite(ra_pin, LOW);

    extended = true;
  };
}

void LinearActuator::retract(){
  if(extended == true) {
    digitalWrite(fb_pin, LOW);
    digitalWrite(fa_pin, LOW);
    digitalWrite(rb_pin, HIGH);
    digitalWrite(ra_pin, HIGH);

    delay(10000);

    digitalWrite(fb_pin, LOW);
    digitalWrite(fa_pin, LOW);
    digitalWrite(rb_pin, LOW);
    digitalWrite(ra_pin, LOW);

    extended = false;
  };
}

bool LinearActuator::get_extended(){
  return extended;
}

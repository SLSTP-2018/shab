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
#include "RTClib.h"
#include "SHAB.h"

// Construct LinearActuator class by turning on forward and reverse pins.
LinearActuator::LinearActuator(int f, int r, RTC_DS1307 RTC)
                               : fpin(f), rpin(r), rtc(RTC) {
  pinMode(fpin, OUTPUT);
  pinMode(rpin, OUTPUT);
}

// Extend the linear actuator.
// This function will refuse to extend the arm if the actuator has ever been,
// or currently is, extended. If the actuator is currently extending, it will
// update the position of the arm.
void LinearActuator::extend() {
  if(has_extended == true or extended == true) {
    // pass
  } else if(is_extending == true) {
    update();
  } else {  // Actuator is retracted and has never been extended
    // Physically extend actuator
    digitalWrite(fpin, HIGH);
    digitalWrite(rpin, LOW);

    // Record time of extension and flag that actuator is moving
    extension_start = rtc.now().unixtime();
    is_extending = true;
  };
}

bool LinearActuator::get_extended() {
  return extended;
}

// Retract the linear actuator.
// This function will refuse to retract the arm unless the actuator is
// extended. If the actuator is currently extending, it will update the
// position of the arm.
void LinearActuator::retract() {
  if(extended == true and is_retracting == false) {
    // Physically retract actuator
    digitalWrite(fpin, LOW);
    digitalWrite(rpin, HIGH);

    // Record time of retraction and flag that actuator is moving
    retraction_start = rtc.now().unixtime();
    is_retracting = true;

  } else if(is_retracting == true) {
    update();
  };
}

// Runs a full self-test on the linear actuator.
// This function first retracts the linear reactor in case it is extended,
// then it extends and retracts the linear actuator. Finally, it resets the
// the variables to ensure the actuator functions in the future.
void LinearActuator::self_test() {
  // Fully retract the actuator.
  extended = true;
  retract();
  delay(31000);
  extended = false;
  is_retracting = false;

  // Extension test
  extend();
  delay(21000);
  extended = true;
  has_extended = true;
  is_extending = false;

  delay(1000);

  // Retraction test
  retract();
  delay(31000);
  extended = false;
  is_retracting = false;

  // Reset the extension switch
  has_extended = false;
}

// Updates LinearActuator's internal tracking variables for a moving arm.
// Briefly, this functions determines if enough time has passed for an
// extending or retracting arm to have fully extended or retracted. It then
// turns off the actuator if needed and updates tracking variables.
void LinearActuator::update() {
  int32_t now = rtc.now().unixtime();

  // Determines if actuator has been extending/retracting for long enough
  if(is_extending == true and now - extension_start >= 21) {
    // Stop actuator
    digitalWrite(fpin, LOW);
    digitalWrite(rpin, LOW);
    
    // Update tracking variables
    extended = true;
    has_extended = true;
    is_extending = false;
  } else if(is_retracting == true and now - retraction_start >= 30){
    // Stop actuator
    digitalWrite(fpin, LOW);
    digitalWrite(rpin, LOW);

    // Update tracking variables
    extended = false;
    is_retracting = false;
  };
}

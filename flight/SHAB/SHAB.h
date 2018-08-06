/*
  SHAB.h contains all declarations for in-house SHAB code
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

// Multiple-Inclusion Guard
#ifndef SHAB_SHAB_H_
#define SHAB_SHAB_H_

#include <Arduino.h>
#include "MS5xxx.h"
#include "RTClib.h"


// Controls a L12-100-210-6-S Actuonix linear actuator.
// The three member variables in the constructor are:
//   fpin: pin that extends the linear actuator when powered
//   rpin: pin that retracts the linear actuator when powered
//   rtc:  Real-Time Clock for timing the actuator
// This model of actuator does not report any information to the Arduino about
// it's position. This class determines the position of the linear actuator
// based on the known extension and retraction time of the actuator (~26s).
// As a result, this class must assume all operations work and use variables
// to record the last known position/operation. This class only permits
// extending the actuator once for use with high-altitude balloon sampling.
class LinearActuator {
  private:
    // Initialized members
    uint8_t fpin;    // Forward Pin: Extends linear actuator
    uint8_t rpin;    // Reverse Pin: Restracts linear actuator
    RTC_DS1307 rtc;  // Real-Time Clock device

    // Private variables for tracking the linear actuator's physical behavior
    bool extended = false;       // Is arm extended
    bool has_extended = false;   // Has arm ever extended
    bool is_extending = false;   // Is arm currently extending
    bool is_retracting = false;  // Is arm currently retracting
    uint32_t extension_start;    // Time arm started extending
    uint32_t retraction_start;   // Time arm started retracting
    
  public:
    LinearActuator(int f, int r, RTC_DS1307 rtc);
    void extend();        // Extends the linear actuator
    bool get_extended();  // Returns the extension state of linear actuator
    void retract();       // Retracts the linear actuator
    void self_test();     // Performs 90 second self-test

    // Updates the state of the linear actuator during movement. This function
    // is used internally by extend() and retract(), but is also meant to be
    // called directly. This function should be called at the end of each loop.
    void update();
};

// Performs a Cyclic Redundancy Check on the MS5xxx altimeter module and
// ensures the result is equal to a pre-determined value.
bool CRC_Valid(MS5xxx& sensor);

// Converts temperature-corrected pressure in pascals to altitude in meters.
double PascalToMeter(double  pressurePa);

#endif // SHAB_SHAB_H_

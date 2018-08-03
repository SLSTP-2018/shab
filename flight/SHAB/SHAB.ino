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
#include <SoftwareSerial.h>
#include <Wire.h>

MS5xxx sensor(&Wire);

// Altitude Ranges
const int tropo_lower = 8;
const int tropo_upper = 12;
const int strato_lower = 24;
const int strato_upper = 30;

// Intitialize LinearActuators
// Pin Order: fpin, rpin
LinearActuator tropo (3, 4);
LinearActuator strato (8, 9);

//Error LED pins
const int alt_com_err = 10;  // Altimeter Communications Error
const int alt_crc_err = 11;  // Altimeter CRC Error
int err_leds [2] = {alt_com_err, alt_crc_err};  // Array of error LEDs

void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);

  // Retract arms in case they are extended
  tropo.retract();
  strato.retract();

  //Light all error LEDs to ensure function
  flashErrorLEDs(err_leds, 3000);

  // Run actuator tests
  tropo.test();
  strato.test();

  // See if altimeter is responding with CRC
  if(sensor.connect() > 0) {
    digitalWrite(alt_com_err, HIGH);
    Serial.println("Error connecting...");
    delay(500);
    setup();
  } else if(CRC_Valid(sensor) == false) {
    digitalWrite(alt_crc_err, HIGH);
    Serial.println("CRC failure...");
    delay(500);
    setup();
  }

  // Turn off error LEDs
  digitalWrite(alt_com_err, LOW);
  digitalWrite(alt_crc_err, LOW);

  // Flash Error LEDs to signal end of setup
  for(int i = 0; i < 5; ++i) {
    delay(50);
    flashErrorLEDs(err_leds, 50);
  };
}

void loop() {
  // put your main code here, to run repeatedly:

  // Obtain altimeter data
  sensor.ReadProm();
  sensor.Readout();
  
  double altitude = PascalToMeter(sensor.GetPres());

  if(altitude >= tropo_lower and altitude <= tropo_upper) {
      tropo.extend();
      strato.retract();
  } else if(altitude >= strato_lower and altitude <= strato_upper) {
      tropo.retract();
      strato.extend();
  } else {  // Retract both arms in non-sampling altitudes
    tropo.retract();
    strato.retract();
  };
}

void flashErrorLEDs(int pins[], int seconds) {
  for(int pin = 0; pin < sizeof(int); ++pin) {
    digitalWrite(pin, HIGH);
  };

  delay(seconds);

  for(int pin = 0; pin < sizeof(int); ++pin) {
    digitalWrite(pin, LOW);
  };
}

//void powerLinearActuator(boolean useLinAct1, boolean isExtend){
  // choose which pin to use
//  int pin = useLinAct1 ? LA_1_EXTEND_PIN : LA_2_EXTEND_PIN;
  // if isExtend, we already have the correct pin. if retract, it's the next pin
//  pin = isExtend ? pin : pin + 1;
  // perform the pin action
//  digitalWrite(pin, HIGH);
//  delay(LA_POWER_MILLISECONDS);
//}

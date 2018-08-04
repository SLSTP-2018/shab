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
#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include "MS5xxx.h"
#include "RTClib.h"
#include "SHAB.h"

MS5xxx sensor(&Wire);

RTC_DS1307 RTC;
const int cs = 4;

// Intitialize LinearActuators
// Pin Order: fpin, rpin
LinearActuator tropo (2, 3, RTC);
LinearActuator strato (9, 8, RTC);

// Altitude Ranges
const int tropo_lower = 7;
const int tropo_upper = 9;
const int strato_lower = 11;
const int strato_upper = 13;

//Error LED pins
const int alt_com_err = 5;   // Altimeter Communications Error
const int alt_crc_err = 6;   // Altimeter CRC Error
const int rtc_run_err = 7;   // RTC Not Running Error
const int sdc_avl_err = 4;  // SD Card Not Available

// Array of error LEDs
const int num_err_leds = 4;
int err_leds [num_err_leds] = {alt_com_err,
                               alt_crc_err,
                               rtc_run_err,
                               sdc_avl_err};

void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  for(int pin = 0; pin < num_err_leds; ++pin) {
    pinMode(err_leds[pin], OUTPUT);
  };

  //Light all error LEDs to ensure function
  Serial.println("Flashing LEDs");
  flashErrorLEDs(err_leds, num_err_leds, 3000);

  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    digitalWrite(rtc_run_err, HIGH);
  };

  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);

  if (!SD.begin(cs)) {
    Serial.println("Card failed, or not present");
    digitalWrite(sdc_avl_err, HIGH);
    setup();
  }

  RTC.adjust(DateTime(__DATE__, __TIME__));

  DateTime now = RTC.now();

  // Run actuator tests
  //Serial.println("Running tropo test");
  //tropo.self_test();
  //Serial.println("Running strato test");
  //strato.self_test();

  // See if altimeter is responding with CRC
  if(sensor.connect() > 0) {
    digitalWrite(alt_com_err, HIGH);
    Serial.println("Error connecting...");
    delay(500);
    setup();
  }
  
  if(CRC_Valid(sensor) == false) {
    digitalWrite(alt_crc_err, HIGH);
    Serial.println("CRC failure...");
    delay(500);
    setup();
  }

  // Turn off error LEDs
  //Serial.println("Turning off LEDs");
  for(int pin = 0; pin < num_err_leds; ++pin) {
    digitalWrite(err_leds[pin], LOW);
  };

  // Flash Error LEDs to signal end of setup
  //Serial.println("Flashing LEDs");
  for(int i = 0; i < 15; ++i) {
    delay(50);
    flashErrorLEDs(err_leds, num_err_leds, 50);
  };
}

void loop() {
  uint32_t seconds = RTC.now().unixtime();
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println(seconds);
  dataFile.close();
  Serial.println(seconds);

  // Obtain altimeter data
  //Serial.println("Getting altimeter data");
  sensor.ReadProm();
  sensor.Readout();
  
  //Serial.println("Calculating altitude");
  double altitude = PascalToMeter(sensor.GetPres());
  //Serial.println(altitude);

  if(altitude >= tropo_lower and altitude <= tropo_upper) {
    //Serial.println("Extending tropo");
    tropo.extend();
    strato.retract();
  } else if(altitude >= strato_lower and altitude <= strato_upper) {
    //Serial.println("Extending strato");
    tropo.retract();
    strato.extend();
  } else {  // Retract both arms in non-sampling altitudes
    //Serial.println("Retracting both");
    tropo.retract();
    strato.retract();
  };

  //Serial.println("End loop");
  delay(1000);
}

void flashErrorLEDs(int pins[], const int pin_number, const int msecs) {
  for(int pin = 0; pin < pin_number; ++pin) {
    digitalWrite(pins[pin], HIGH);
  };

  delay(msecs);

  for(int pin = 0; pin < pin_number; ++pin) {
    digitalWrite(pins[pin], LOW);
  };
}

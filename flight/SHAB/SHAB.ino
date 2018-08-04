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


// Built-In Libraries
#include <Arduino.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>

// Local Libraries
#include "MS5xxx.h"
#include "RTClib.h"
#include "SHAB.h"

// Declare Future Hardware Interfaces
RTC_DS1307 RTC;        // Real-Time Clock
const uint8_t cs = 4;  // Chip Select for SD Card

// Intitialize Hardware Interfaces
MS5xxx sensor(&Wire);               // Altimeter
LinearActuator tropo (2, 3, RTC);   // Linear Actuator for Troposphere
LinearActuator strato (9, 8, RTC);  // Linear Actuator for Stratosphere

// Altitude Sample Ranges (meters)
const uint16_t tropo_lower  = 3000;
const uint16_t tropo_upper  = 3500;
const uint16_t strato_lower = 15000;
const uint16_t strato_upper = 15500;

void setup() {
  // Intialize declared hardware interfaces
  Wire.begin();
  RTC.begin();

  //Error LED pins
  const uint8_t alt_com_err = 5;   // Altimeter Communications Error (Red)
  const uint8_t alt_crc_err = 6;   // Altimeter CRC Error (Yellow)
  const uint8_t rtc_run_err = 7;   // RTC Not Running Error (Blue)
  const uint8_t sdc_avl_err = 10;  // SD Card Not Available (Green)

  // Array of error LEDs
  const uint8_t num_err_leds = 4;
  uint8_t err_leds [num_err_leds] = {alt_com_err,
                                     alt_crc_err,
                                     rtc_run_err,
                                     sdc_avl_err};

  // Enable error LED pins
  for(uint8_t pin = 0; pin < num_err_leds; ++pin) {
    pinMode(err_leds[pin], OUTPUT);
  };

  // Turn LEDs on for three seconds to ensure they function
  flashErrorLEDs(err_leds, num_err_leds, 3000, 1);

  // Ensure RTC is operating
  while(!RTC.isrunning()) {  // Stall execution until RTC is running
    digitalWrite(rtc_run_err, HIGH);  // Warn if RTC is not running
  };
  RTC.adjust(DateTime(__DATE__, __TIME__));  // Set time to compile time

  // Connect with SD card and ensure the connection is valid
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);
  while(!SD.begin(cs)) {  // Stall execution until SD card is available
    digitalWrite(sdc_avl_err, HIGH);  // Warn if SD card is not available
  };

  // Write column header to file
  File dataFile = SD.open("hardware_data.csv", FILE_WRITE);
  dataFile.println("Time(s),Altitude(m),Pressure(Pa),Temperature(C),Tropo_Extended,Strato_Extended,CRC(0xB)");
  dataFile.close();

  // Ensure altimeter is connected on I2C
  while(sensor.connect() > 0) {  // Stall execution til altimeter is connected
    digitalWrite(alt_com_err, HIGH);  // Warn if altimeter not connected
  };
  
  // Assert Cyclic Redundancy Check of altimeter
  while(!CRC_Valid(sensor)) {  // Stall execution until CRC completes
    digitalWrite(alt_crc_err, HIGH);  // Warn if CRC failure
  };

  // Run linear actuator self tests
  //tropo.self_test();
  //strato.self_test();

  // Turn off all error LEDs
  for(uint8_t pin = 0; pin < num_err_leds; ++pin) {
    digitalWrite(err_leds[pin], LOW);
  };

  // Flash Error LEDs for three seconds to signal end of setup
  flashErrorLEDs(err_leds, num_err_leds, 50, 30);
}

void loop() {
  // Get time at the start of the loop
  uint32_t seconds = RTC.now().unixtime();

  // Obtain altimeter data
  sensor.ReadProm();
  sensor.Readout();
  uint16_t altitude = PascalToMeter(sensor.GetPres());

  // Write data for time point to file
  File dataFile = SD.open("hardware_data.csv", FILE_WRITE);
  if(dataFile) {
    dataFile.print(seconds);
    dataFile.print(",");
    dataFile.print(altitude);
    dataFile.print(",");
    dataFile.print(sensor.GetPres());
    dataFile.print(",");
    dataFile.print(sensor.GetTemp());
    dataFile.print(",");
    dataFile.print(tropo.get_extended());
    dataFile.print(",");
    dataFile.print(strato.get_extended());
    dataFile.print(",");
    dataFile.println(sensor.CRCcodeTest());
    dataFile.close();
  };

  // Determine if in a sampling range and extend/retract actuators accordingly
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

  // Update position of linear actuators
  tropo.update();
  strato.update();

  // One second delay to save power and to keep each row of output unique
  delay(1000);
}

void flashErrorLEDs(uint8_t pins[], const uint8_t pin_number,
                    const uint8_t msecs, const uint8_t loop_num) {
  for(uint8_t i = 0; i < loop_num; ++i) {
    for(uint8_t pin = 0; pin < pin_number; ++pin) {
      digitalWrite(pins[pin], HIGH);
    };

    delay(msecs);

    for(uint8_t pin = 0; pin < pin_number; ++pin) {
      digitalWrite(pins[pin], LOW);
    };
  };
}

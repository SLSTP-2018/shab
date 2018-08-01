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

#ifndef SHAB_SHAB_ino_  // Prevent multiple inclusion
#define SHAB_SHAB_ino_  // Define if not defined previously

class LinearActuator {
  private:
    bool extended = false;
    int fpin, rpin;
    
  public:
    LinearActuator (int f, int r);
    bool get_extended();
};

LinearActuator::LinearActuator (int f, int r){
  fpin = f;
  rpin = r;
}

bool LinearActuator::get_extended(){
  return extended;
}

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

#endif // SHAB_SHAB_ino_

#ifndef SHAB_SHAB_ino_  // Prevent multiple inclusion
#define SHAB_SHAB_ino_  // Define if not defined previously

class LinearActuator {
  
} tla, sla;

// tla = Troposphere Linear Actuator
// sla = Stratosphere Linear Actuator

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

#endif // SHAB_SHAB_ino_vv

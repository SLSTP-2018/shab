void setup() {
  // put your setup code here, to run once:
  int SCL = 0;
  int SDA = 1;
  int VIN = 3;
  
  pinMode(SCL, OUTPUT);
  pinMode(SDA, INPUT);
  pinMode(VIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int SCL = 0;
  int SDA = 1;
  int VIN = 3;
  digitalWrite(VIN, HIGH);
  if (Serial.available() > 0) {
    Serial.println(Serial.read());
  }
}

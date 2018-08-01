void setup() {
  // put your setup code here, to run once:  
  pinMode(SDA, INPUT);
  pinMode(SCL, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    Serial.println(Serial.read());
  }
}

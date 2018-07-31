//Globaly set pin names
int SCL0 = 0;
int SDA1 = 1;
int VIN3 = 3;

void setup() {
  // put your setup code here, to run once:  
  pinMode(SCL0, OUTPUT);
  pinMode(SDA1, INPUT);
  pinMode(VIN3, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(VIN3, HIGH);
  if (Serial.available() > 0) {
    Serial.println(Serial.read());
  }
}

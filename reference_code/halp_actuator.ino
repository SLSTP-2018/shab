int motor_left[] = {5, 6};
int motor_right[] = {4, 3};

void setup() {
  // Setup motors
  int i;
    for(i = 0; i < 2; i++){
    pinMode(motor_left[i], OUTPUT);
    pinMode(motor_right[i], OUTPUT);
  }
}

void loop() {  // left = out and right = in
  digitalWrite(motor_left[0], HIGH); // 5 on
  digitalWrite(motor_left[1], LOW);  // 6 off
  digitalWrite(motor_right[0], HIGH);// 4 on
  digitalWrite(motor_right[1], LOW); // 3 off
  delay(30000);

  digitalWrite(motor_left[0], LOW); 
  digitalWrite(motor_left[1], LOW); 
  digitalWrite(motor_right[0], LOW); 
  digitalWrite(motor_right[1], LOW); 
  delay(25);

  digitalWrite(motor_left[0], LOW); 
  digitalWrite(motor_left[1], HIGH); 
  digitalWrite(motor_right[0], LOW); 
  digitalWrite(motor_right[1], HIGH);
  delay(30000);

  digitalWrite(motor_left[0], LOW); 
  digitalWrite(motor_left[1], LOW); 
  digitalWrite(motor_right[0], LOW); 
  digitalWrite(motor_right[1], LOW); 
  delay(25);
}

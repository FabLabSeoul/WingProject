#include <Servo.h>

Servo SimonKESC;
int pot1Pin = A2;

void setup() {
  SimonKESC.attach(5);
}

void loop() {
  int pot1 = analogRead(pot1Pin);
  pot1 = map(pot1, 0, 1023, 0, 179);
  SimonKESC.write(pot1);
}


#include <Servo.h>

Servo SimonKESC;
int pot1Pin = A2;
int pot1 = 0;

void setup() {
  SimonKESC.attach(5);
  Serial.begin(115200);
}

void loop() {
   
 if (Serial.available()) 
   { // If data is available to read,
     char val = Serial.read(); // read it and store it in val
     String str;
     str += val;
     pot1 = str.toInt();
     Serial.println(str);
   }
   
  //int pot1 = analogRead(pot1Pin);
  int out = map(pot1, 0, 9, 0, 179);
  SimonKESC.write(out);
}


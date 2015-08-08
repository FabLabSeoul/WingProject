// Fast Receive Serial Data
// read serial
// debug serial write, servoOut = xx
// output PWM D5 ping

#include <Servo.h>

Servo SimonKESC1;
Servo SimonKESC2;
Servo SimonKESC3;
int servoOut1 = 0;
int servoOut2 = 0;
int servoOut3 = 0;
int pot1Pin = A2;

void setup() 
{
   SimonKESC1.attach(5);
   SimonKESC2.attach(6);   
   SimonKESC3.attach(9);
}

void loop()
{
  int pot1 = analogRead(pot1Pin);
  int mapData = map(pot1, 0, 1023, 0, 179);

  SimonKESC1.write(mapData);
  SimonKESC2.write(mapData);
  SimonKESC3.write(mapData);      
}


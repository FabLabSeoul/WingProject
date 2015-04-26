// Fast Send Serial Data Version
//
// Read PotentialMeter from A2
// Write D5 pin, PWM 0~179, by A2 map(0~1023, to 0~179)
// Write Serial PotentialMeter 'S' map(0~1023, to 0~9) X 111  'E'
//   Ex) S000E, S111E, S999E
#include <Servo.h>

Servo SimonKESC;
int pot1Pin = A2;
void setup() {
  SimonKESC.attach(5);
   Serial.begin(9600);
}
void loop() {
  int pot1 = analogRead(pot1Pin);
  int mapData = map(pot1, 0, 1023, 0, 179);
  SimonKESC.write(mapData);

  // mapping 0~1023 to 0 ~ 9  
  int sendData = map(pot1, 0, 1023, 0, 255);
  char buffer[6];
  buffer[0] = 'S';
  buffer[1] = sendData;
  buffer[2] = sendData;
  buffer[3] = sendData;
  buffer[4] = 'E';
  Serial.write(buffer,5);
  
  delay(40);
}


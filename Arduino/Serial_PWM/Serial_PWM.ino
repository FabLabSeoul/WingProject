#include <Servo.h>

Servo SimonKESC;
int servoOut = 0;

void setup() 
{
   SimonKESC.attach(5);
  Serial.begin(115200);
}

void loop()
{
  if (Serial.available())
  {
    char data[8];
    if (Serial.readBytesUntil('\0', data, 8) > 0)
    {
      int servo = (int)data[2] - '0';
      servoOut = map(servo, 0, 9, 0, 179);
      SimonKESC.write(servoOut);
      
      Serial.println( servo );
     }    
  }
}


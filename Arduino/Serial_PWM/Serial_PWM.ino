// Fast Receive Serial Data

#include <Servo.h>

Servo SimonKESC;
int servoOut = 0;

void setup() 
{
   SimonKESC.attach(5);
  Serial.begin(9600);
}

unsigned char buffer[8];
int readIdx = 0;
void loop()
{
  if (!Serial.available())
    return;
  
  unsigned char data;
  if (Serial.readBytes(&data, 1) > 0)
  {
    if (data == 'S')
    {
      readIdx = 0;
    }
    else if (data == 'E')
    {
      int servo = (int)buffer[0];
      servoOut = map(servo, 0, 255, 0, 179);
      SimonKESC.write(servoOut);      
      Serial.print( "servoOut = ");
      Serial.println( servoOut );
    }
    else
    {
      if (readIdx < 3)
      {
        buffer[ readIdx++] = (int)data;
      }
    }
   }
}


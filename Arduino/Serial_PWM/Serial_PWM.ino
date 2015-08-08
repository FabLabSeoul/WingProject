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
int boardNumber = 4;

void setup() 
{
   SimonKESC1.attach(5);
   SimonKESC2.attach(6);   
   SimonKESC3.attach(9);
  Serial.begin(9600);
}

unsigned char buffer[32];
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
      const int index = boardNumber * 3;
      int servo1 = (int)buffer[index];
      int servo2 = (int)buffer[index+1];
      int servo3 = (int)buffer[index+2];
      int CRC = (int)buffer[24];
      int totalNumber = 0;
      for (int i=0; i < 24; ++i)
        {
          totalNumber += buffer[i];
        }
        
      if (CRC != ((totalNumber) % 255))
      {
         Serial.println( "CRC~");
         return;
      }
      
      servoOut1 = map(servo1, 0, 255, 28, 156);
      servoOut2 = map(servo2, 0, 255, 28, 156);
      servoOut3 = map(servo3, 0, 255, 28, 156);     
      SimonKESC1.write(servoOut1);
      SimonKESC2.write(servoOut2);
      SimonKESC3.write(servoOut3);      
    }
    else
    {
      if (readIdx < 25)
      {
        buffer[ readIdx++] = (int)data;
      }
    }
   }
}


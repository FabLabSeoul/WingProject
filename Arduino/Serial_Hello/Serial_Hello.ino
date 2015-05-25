void setup() 
{
//initialize serial communications at a 9600 baud rate
  Serial.begin(9600);
}

void loop()
{
//send 'Hello, world!' over the serial port
Serial.println("Hello, world!");
//wait 100 milliseconds so we don't drive ourselves crazy
delay(300);
}


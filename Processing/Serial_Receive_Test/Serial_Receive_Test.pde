//
// Test Serial Comminication

import processing.serial.*;

Serial myPort;  // Create object from Serial class

void setup()
{
  size(200,200);
  println(Serial.list());
  
  //change the 0 to a 1 or 2 etc. to match your port
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 19200); 
  
  println( "link portName = " + portName );
  myPort.clear();  
}

void draw()
{
  // nothing~
}

void serialEvent(Serial p) 
{
  print( (int)p.readChar() );
  print( ' ' );
}


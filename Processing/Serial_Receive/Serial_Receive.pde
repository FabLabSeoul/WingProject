import processing.serial.*;

Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port

void setup()
{
  // I know that the first port in the serial list on my mac
  // is Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  size(200,200);
  String portName = Serial.list()[1]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600); 
}

void draw()
{
  if ( myPort.available() > 0) 
  {
    // If data is available,
    val = myPort.readStringUntil('\n');         // read it and store it in val
    println(val); //print it out in the console  
  } 
  
  
}



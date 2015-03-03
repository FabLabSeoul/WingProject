/*
   HMC5884L Magnetormeter Sensor Test
 - serial input
 - 'S'
 - magnetormeter x
 - magnetormeter y
 - magnetormeter z
 
 display heading sensor
 */

import processing.serial.*;

Serial  myPort;
int     lf = 10;       
String  inString;      
int     calibrating;

float   x_mag; // magnetometer
float   y_mag;
float   z_mag;
float heading = 0;



void setup() { 
  //P3D
  size(1024, 768, P3D);
  noStroke();
  colorMode(RGB, 256); 

  String portName = Serial.list()[1];
  println(Serial.list());

  println( "link portName = " + portName );
  myPort = new Serial(this, portName, 19200);
  myPort.clear();
} 

//draw box
void draw_box(int r, int g, int b) {

//  fill(r, g, b);
  scale(90);

  beginShape(QUADS);

  fill(255, 0, 0);
  vertex(-1, 0, 0.25);
  vertex( 1, 0, 0.25);
  vertex( 1, -1.5, 0.25);
  vertex(-1, -1.5, 0.25);

  fill(r, g, b);
  vertex(-1, 1.5, 0.25);
  vertex( 1, 1.5, 0.25);
  vertex( 1, 0, 0.25);
  vertex(-1, 0, 0.25);

  vertex( 1, 1.5, 0.25);
  vertex( 1, 1.5, -0.25);
  vertex( 1, -1.5, -0.25);
  vertex( 1, -1.5, 0.25);

  vertex( 1, 1.5, -0.25);
  vertex(-1, 1.5, -0.25);
  vertex(-1, -1.5, -0.25);
  vertex( 1, -1.5, -0.25);

  vertex(-1, 1.5, -0.25);
  vertex(-1, 1.5, 0.25);
  vertex(-1, -1.5, 0.25);
  vertex(-1, -1.5, -0.25);

  vertex(-1, 1.5, -0.25);
  vertex( 1, 1.5, -0.25);
  vertex( 1, 1.5, 0.25);
  vertex(-1, 1.5, 0.25);

  vertex(-1, -1.5, -0.25);
  vertex( 1, -1.5, -0.25);
  vertex( 1, -1.5, 0.25);
  vertex(-1, -1.5, 0.25);

  endShape();
}

void draw() { 

  background(0);
  lights();

  int distance = 50;
  int x_rotation = 130;

  //gyro scope
  pushMatrix(); 
  translate(width/2, height/2, -50); 
  rotateX(radians(30));
  rotateZ(heading);
  draw_box(249, 250, 50);
  popMatrix(); 


  textSize(24);
  String magStr = "(" + (int)x_mag + ", " + (int)y_mag + ", " + (int)z_mag + ")";
  String magStr2 = "(" + heading + ")";

  //magnetometer
  fill(56, 140, 206);
  text("Magnetometer", (int) width/2.0 - 50, 25);
  text(magStr, (int) (width/2.0) - 30, 50);
  text(magStr2, (int) (width/2.0) - 30, 75);
} 



boolean startParse = false;
int sensingCount = 0;
int count = 0;
short v1 = 0, v2 = 0;

void serialEvent(Serial p) 
{
  if (myPort.available() <= 0)
  {
    return;
  }

  if (startParse)
  {
    ++count;

    float value = 0;
    if ((count % 2) == 1)
    {
      v1 = (short)myPort.readChar();
      v1 <<= 8;
    } else
    {
      v2 = (short)myPort.readChar();

      short v  = (short)(v1 | v2);
      value = float(v);

      switch (count)
      {
      case 2: 
        x_mag = value; 
        break;
      case 4: 
        y_mag = value; 
        break;
      case 6: 
        z_mag = value; 
        startParse = false; 
        break;
      }

      if (count == 6)
      {
        heading = atan2(y_mag, x_mag);
      }
    }
  } else
  {
    char head = myPort.readChar();
    if ('S' == head)
    {
      startParse = true;
      count = 0;
      ++sensingCount;
      println( "detect S " + sensingCount );
    }
  }
}


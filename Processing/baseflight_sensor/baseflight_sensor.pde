/*
 baseflight sensor
 */

import processing.serial.*;

Serial  myPort;
 
float   x_gyr;  // Gyroscope data
float   y_gyr;
float   z_gyr;
float heading = 0;
 
 
void setup()  { 
  size(800, 600, P3D);
  noStroke();
  colorMode(RGB, 256); 
 
  String portName = Serial.list()[1];
  println(Serial.list());

  println( "link portName = " + portName );
  myPort = new Serial(this, portName, 115200);
  myPort.clear();
} 


void draw_rect(int r, int g, int b) 
{
  fill(r, g, b);
  scale(90);
  
  beginShape(QUADS);

  vertex(-1, 1.5, 0.25);
  vertex( 1, 1.5, 0.25);
  vertex( 1, -1.5, 0.25);
  vertex(-1, -1.5, 0.25);

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


//draw compass
void draw_compass(int r, int g, int b) 
{
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

void draw()  
{ 
  background(0);
  lights();
    
  int distance = 50;
  int x_rotation = 90;

  //accelerometer
  pushMatrix();
  translate(width/2, height/2, -50);
  rotateX((-x_gyr - x_rotation));
  rotateY((-y_gyr));
  rotateZ((-z_gyr));
  draw_rect(56, 140, 206);
  popMatrix();
   
  //Display Information
  textSize(24);
  String gyrStr = "(" + (int) x_gyr + ", " + (int) y_gyr + ", " + (int) z_gyr + ")";

  fill(56, 140, 206);
  text("Accelerometer", (int) width/2.0 - 50, 25);
  text(gyrStr, (int) (width/2.0) - 30, 50); 



  processMessage();
} 


// serial data -> 'Radian', millis, roll, pitch, heading
void processMessage()
{
  String message = myPort.readStringUntil(10);
  if (message == null)
    return;

  println(message);
  String data[] = message.split(",");
  println(data[0]);
  if (data[0].equals("Radian"))
  {
    try 
    {
      x_gyr = Float.parseFloat(data[2]);
      y_gyr = Float.parseFloat(data[3]);
      z_gyr = Float.parseFloat(data[4]);
    }
    catch (Throwable t)
    {
      println(".");
    }
  }
}


/*
 Show GY521 + GY271 Data
 */

import processing.serial.*;

Serial  myPort;
 
float   dt;
float   x_gyr;  //Gyroscope data
float   y_gyr;
float   z_gyr;
float   x_acc;  //Accelerometer data
float   y_acc;
float   z_acc;
float   x_fil;  //Filtered data
float   y_fil;
float   z_fil;

float   x_mag; // magnetometer
float   y_mag;
float   z_mag;
float heading = 0;
 
 
void setup()  { 
  size(1024, 768, P3D);
  noStroke();
  colorMode(RGB, 256); 
 
  String portName = Serial.list()[1];
  println(Serial.list());

  println( "link portName = " + portName );
  myPort = new Serial(this, portName, 9600);
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
  
  //gyro scope
  pushMatrix(); 
  translate(width/6, height/3, -50); 
  rotateX(radians(-x_gyr - x_rotation));
  rotateY(radians(-y_gyr));
  draw_rect(249, 250, 50);  
  popMatrix(); 

  //accelerometer
  pushMatrix();
  translate(width/2, height/3, -50);
  rotateX(radians(-x_acc - x_rotation));
  rotateY(radians(-y_acc));
  draw_rect(56, 140, 206);
  popMatrix();
  
  //combination
  pushMatrix();
  translate(5*width/6, height/3, -50);
  rotateX(radians(-x_fil - x_rotation));
  rotateY(radians(-y_fil));
  draw_rect(93, 175, 83);
  popMatrix();
  
  //magnetometer
  pushMatrix(); 
  translate(width/2, height*2.5/3, -50); 
  rotateX(radians(x_rotation));
  rotateZ(heading);
  draw_compass(249, 250, 50);
  popMatrix();  
 
 
  //Display Gyroscope + Accelermoeter + Combination Information
  textSize(24);
  String accStr = "(" + (int) x_acc + ", " + (int) y_acc + ")";
  String gyrStr = "(" + (int) x_gyr + ", " + (int) y_gyr + ")";
  String filStr = "(" + (int) x_fil + ", " + (int) y_fil + ")";
 
  fill(249, 250, 50);
  text("Gyroscope", (int) width/6.0 - 60, 25);
  text(gyrStr, (int) (width/6.0) - 40, 50);

  fill(56, 140, 206);
  text("Accelerometer", (int) width/2.0 - 50, 25);
  text(accStr, (int) (width/2.0) - 30, 50); 
  
  fill(83, 175, 93);
  text("Combination", (int) (5.0*width/6.0) - 40, 25);
  text(filStr, (int) (5.0*width/6.0) - 20, 50);
  
  
  // Dispay Magnetometer Information
  textSize(24);
  String magStr = "(" + (int)x_mag + ", " + (int)y_mag + ", " + (int)z_mag + ")";
  String magStr2 = "(" + heading + ")";

  fill(56, 140, 206);
  text("Magnetometer", (int) width/2.0 - 50, 445);
  text(magStr, (int) (width/2.0) - 30, 470);
  text(magStr2, (int) (width/2.0) - 30, 495);
} 


boolean startParse = false;
int sensingCount = 0;
int count = 0;
short v1 = 0, v2 = 0;

void serialEvent(Serial p) 
{
  if (p.available() > 0) 
  {
    if (startParse)
    {
      ++count;
      if (count == 1)
      {
        dt  = float(p.readChar());
        println( "dt = " + dt );
      }
      else
      {
        float value = 0;
        if ((count % 2) == 0)
        {
          v1 = (short)p.readChar();
          v1 <<= 8;
        }
        else
        {
          v2 = (short)p.readChar();
          
          short v  = (short)(v1 | v2);
          value = float(v) / 100.f;

          switch (count)
          {
            case 3: x_acc = value; break;
            case 5: y_acc = value; break;
            case 7: z_acc = value; break;

            case 9:  x_gyr = value; break;
            case 11: y_gyr = value; break;
            case 13: z_gyr = value; break;

            case 15: x_fil = value; break;
            case 17: y_fil = value; break;
            case 19: z_fil = value; break;
            
            case 21: x_mag = value; break;
            case 23: y_mag = value; break;
            case 25: 
              z_mag = value; 
              heading = atan2(y_mag, x_mag); 
              startParse = false; 
              break;
          }
        }
        
      }
      
    }
    else
    {
      char head = p.readChar();
      if ('S' == head)
      {
        startParse = true;
        count = 0;
        ++sensingCount;
        println( "detect S " + sensingCount );
      }      
    }
  }
}

import processing.serial.*;

Serial myPort;
int lf = 10;

float x_acc;
float y_acc;
float z_acc;
int x_pos;
int y_pos;
boolean btnZ;
boolean btnC;

void setup()
{
  size(800, 600, P3D);
  noStroke();
  colorMode(RGB, 256); 

  print(Serial.list());
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
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


void draw()
{
  background(0);
  lights();

  int distance = 50;
  int x_rotation = 90;

  pushMatrix(); 
  translate(width/2, height/2, -50); 
  //  rotateX(radians(-x_gyr - x_rotation));
  rotateX(radians(y_acc));
  rotateY(radians(x_acc));
  draw_rect(249, 250, 50);  
  popMatrix(); 

  // Dispay Nunchuk Information
  textSize(24);
  String str1 = "(" + x_pos + ", " + y_pos + ")";
  String str2 = "(" + (int)x_acc + ", " + (int)y_acc + ", " + (int)z_acc + ")";
  String str3 = "(btnZ = " + btnZ + ")";
  String str4 = "(btnC = " + btnC + ")";

  fill(56, 140, 206);
  text(str1, (int) (width/2.0) - 30, 25);
  text(str2, (int) (width/2.0) - 30, 50);
  text(str3, (int) (width/2.0) - 30, 75);
  text(str4, (int) (width/2.0) - 30, 100);

  // check serial
  processMessage();
}

// serial data -> 'Start',joyX,joyY,accelX,accelY,accelZ,btnZ,btnC
void processMessage()
{
  String message = myPort.readStringUntil(lf);
  if (message == null)
    return;

  //println(message);
  String data[] = message.split(",");
  if (data[0].equals("Start"))
  {
    try 
    {
      x_pos = Integer.parseInt(data[1]);
      y_pos = Integer.parseInt(data[2]);

      x_acc = Float.parseFloat(data[3]);
      y_acc = Float.parseFloat(data[4]);
      z_acc = Float.parseFloat(data[5]);

      btnZ = (Integer.parseInt(data[6])==0)? false : true;
      btnC = (Integer.parseInt(data[7])==0)? false : true;
    }
    catch (Throwable t)
    {
      println(".");
    }
  }
}


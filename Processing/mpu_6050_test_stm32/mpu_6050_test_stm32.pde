/*
 Show GY521 Data.
 본 스케치는 http://www.geekmomprojects.com/에서 참고 하였습니다.
 */

import processing.serial.*;

Serial  myPort;
int     lf = 10;       
String  inString;      
int     calibrating;
 
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

 
void setup()  { 
  //P3D 사용 3D표현
  size(1400, 800, P3D);
  //size(800, 600, P3D);
  stroke(0,0,0);
  colorMode(RGB, 256); 
 
  String portName = Serial.list()[1];  //포트 번호 지정
  println(Serial.list());                      //포트 리스트 출력

  println( "link portName = " + portName );
  myPort = new Serial(this, portName, 19200);
  //myPort = new Serial(this, portName, 9600);
  //myPort = new Serial(this, portName, 115200);
  myPort.clear();
  //myPort.bufferUntil(lf);
} 

//도형 색깔 및 위치 지정
void draw_rect(int r, int g, int b) {
  scale(90);
  beginShape(QUADS);
  
  fill(r, g, b);
  vertex(-1,  1.5,  0.25);
  vertex( 1,  1.5,  0.25);
  vertex( 1, -1.5,  0.25);
  vertex(-1, -1.5,  0.25);

  endShape();  
}

void draw()  { 
  
  background(0);
    
  int distance = 50;
  int x_rotation = 90;
  
  //gyro 값과 사각형
  pushMatrix(); 
  translate(width/6, height/2, -50); 
  rotateX(radians(-x_gyr - x_rotation));
  rotateY(radians(-y_gyr));
  draw_rect(249, 250, 50);
  
  popMatrix(); 

  //accel 값과 사각형
  pushMatrix();
  translate(width/2, height/2, -50);
  rotateX(radians(-x_acc - x_rotation));
  rotateY(radians(-y_acc));
  draw_rect(56, 140, 206);
  popMatrix();
  
  //둘을 합친 값과 사각형
  pushMatrix();
  translate(5*width/6, height/2, -50);
  rotateX(radians(-x_fil - x_rotation));
  rotateY(radians(-y_fil));
  draw_rect(93, 175, 83);
  popMatrix();
 
  //값 스트링 형태로 저장
  textSize(24);
  String accStr = "(" + (int) x_acc + ", " + (int) y_acc + ")";
  String gyrStr = "(" + (int) x_gyr + ", " + (int) y_gyr + ")";
  String filStr = "(" + (int) x_fil + ", " + (int) y_fil + ")";
 
  //값 표시
  fill(249, 250, 50);
  text("Gyroscope", (int) width/6.0 - 60, 25);
  text(gyrStr, (int) (width/6.0) - 40, 50);

  fill(56, 140, 206);
  text("Accelerometer", (int) width/2.0 - 50, 25);
  text(accStr, (int) (width/2.0) - 30, 50); 
  
  fill(83, 175, 93);
  text("Combination", (int) (5.0*width/6.0) - 40, 25);
  text(filStr, (int) (5.0*width/6.0) - 20, 50);

} 

int ReadInt(Serial p)
{
  int c1 = myPort.readChar();
  int c2 = myPort.readChar();
  int v1 = (c1 << 8) + c2;  
  return v1;
}

//데이터 받아오기
boolean startParse = false;
int sensingCount = 0;
int count = 0;
short v1 = 0, v2 = 0;

void serialEvent(Serial p) 
{
  if (myPort.available() > 0) 
  {
    if (startParse)
    {
      ++count;
      if (count == 1)
      {
        dt  = float(myPort.readChar());
        println( "dt = " + dt );
      }
      else
      {
        float value = 0;
        if ((count % 2) == 0)
        {
          v1 = (short)myPort.readChar();
          v1 <<= 8;
        }
        else
        {
          v2 = (short)myPort.readChar();
          
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
            case 19: z_fil = value; startParse = false; break;
          }
        }
        
      }
      
    }
    else
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
  

/*  
  inString = (myPort.readString());
  
  try {
   
    String[] dataStrings = split(inString, '#');
    for (int i = 0; i < dataStrings.length; i++) {
      String type = dataStrings[i].substring(0, 4);
      String dataval = dataStrings[i].substring(4);
    if (type.equals("DEL:")) {
        dt = float(dataval);
        
      } else if (type.equals("ACC:")) {   //Accelerometer값 가져오기
        String data[] = split(dataval, ',');
        x_acc = float(data[0]);
        y_acc = float(data[1]);
        z_acc = float(data[2]);
        
      } else if (type.equals("GYR:")) {   //자이로 값 가져오기
        String data[] = split(dataval, ',');
        x_gyr = float(data[0]);
        y_gyr = float(data[1]);
        z_gyr = float(data[2]);
      } else if (type.equals("FIL:")) {    //최종 값 가져오기
        String data[] = split(dataval, ',');
        x_fil = float(data[0]);
        y_fil = float(data[1]);
        z_fil = float(data[2]);
      }
    }
  } catch (Exception e) {
      println("Caught Exception");
  }
/**/

}

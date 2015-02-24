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
  //size(1400, 800, P3D);
  size(800, 600, P3D);
  stroke(0,0,0);
  colorMode(RGB, 256); 
 
  String portName = Serial.list()[1];  //포트 번호 지정
  println(Serial.list());                      //포트 리스트 출력

  myPort = new Serial(this, portName, 19200);
  myPort.clear();
  myPort.bufferUntil(lf);
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

//데이터 받아오기
void serialEvent(Serial p) {

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
}

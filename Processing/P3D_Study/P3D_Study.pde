


void setup() { 
  size(800, 600, P3D); 
  //size(1400, 800, P3D);

  noStroke();
  colorMode(RGB, 256);
}


void draw_rect(int r, int g, int b) {
  scale(90);
  beginShape(QUADS);

  fill(r, g, b);
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


float rad = 0;
float rad2 = 0;

void draw() { 

  background(0);
  lights();
  
  camera(mouseX, height/2, (height/2) / tan(PI/6), width/2, height/2, 0, 0, 1, 0);

  //Show accel data
  pushMatrix();
  translate(width/2, height/2, -50);
  //rotateY(rad += 0.01f );
  //rotateX(rad += 0.02f);
  //draw_rect(56, 140, 206);
  //rect(x,y,w,h);

  //stroke(255);
  //noFill();
  rectMode(CENTER);
  //rect(0,0, 100, 100);
  box(100);  
  popMatrix();
}



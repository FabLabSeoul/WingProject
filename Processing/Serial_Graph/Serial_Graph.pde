import processing.serial.*;

Serial myPort;  // Create object from Serial class
int SIZE = 390;
int WIDTH = 400;
int HEIGHT = 620;
double Graph_Max_H = 3.3f;
double Graph_Min_H = 0.f;
double[] data1 = new double[ SIZE];
double[] data2 = new double[ SIZE];
double[] data3 = new double[ SIZE];
int dataIdx1 = 0;
int dataIdx2 = 0;
int dataIdx3 = 0;

void setup()
{
  // I know that the first port in the serial list on my mac
  // is Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  size(WIDTH, HEIGHT);
  String portName = Serial.list()[1]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600); 
}

void Graph(int left, int top, int w, int h,
  double[] graph, int size)
{
  int offsetX = 2;
  stroke(0,0,0);
  strokeWeight(2);
  //line(left, h/2 + top, left+w, h/2 + top);
  line(left+offsetX, top, left+offsetX, top + h);
  strokeWeight(1);
  line(left, top, left+w, top);
  line(left, top+h, left+w, top+h);
  
  strokeWeight(1);
  stroke(255,0,0);
  for (int i=1; i < size; ++i)
  {
    int x1 = i - 1;
    int x2 = i;
    int y1 = h - (int)(graph[ i-1] * (double)h);
    int y2 = h - (int)(graph[ i] * (double)h);
    
    line(x1 + left + offsetX, y1 + top, 
      x2 + left + offsetX, y2 + top);    
  }  
}

boolean SetGraph( int idx, int val, double minValue, double maxValue, 
  double[] graph, int size)
{
  int index = (size <= idx)? 0 : idx;
  double v = (double)val - minValue;
  v = v / (maxValue - minValue);
  graph[ index] =  v;
  
  return (size <= idx+1);
}

void ClearGraph(double[] graph, int size)
{
  for (int i=0; i < size; ++i)
    graph[ i] = 0;
}

void draw()
{
  background(128, 128, 128);

  if (myPort.available() > 0) 
  {
    char head = myPort.readChar();
    if ('R' == head)
    {
      delay(100);
      int c1 = myPort.readChar();
      int c2 = myPort.readChar();
      int c3 = myPort.readChar();
      int c4 = myPort.readChar();
      int c5 = myPort.readChar();
      int c6 = myPort.readChar();

      int v1 = (c1 << 8) + c2;
      int v2 = (c3 << 8) + c4;
      int v3 = (c5 << 8) + c6;
      
      if (SetGraph(dataIdx1++, v1, 0, 4095, data1, SIZE))
       {
         dataIdx1 = 0;
         ClearGraph(data1, SIZE);
       }
       
      if (SetGraph(dataIdx2++, v2, 0, 4095, data2, SIZE))
       {
         dataIdx2 = 0;
         ClearGraph(data2, SIZE);
       }      
       
      if (SetGraph(dataIdx3++, v3, 0, 4095, data3, SIZE))
       {
         dataIdx3 = 0;
         ClearGraph(data3, SIZE);
      }             
       
      println(v1);
      println(v2);
      println(v3);
    }
    else
    {
      println( "not R" );
    }
    
  }
  
  Graph(0, 000, 400, 200, data1, dataIdx1);
  Graph(0, 210, 400, 200, data2, dataIdx2);
  Graph(0, 420, 400, 200, data3, dataIdx3);
}



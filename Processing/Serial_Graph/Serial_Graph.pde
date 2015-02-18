import processing.serial.*;

Serial myPort;  // Create object from Serial class
int SIZE = 390;
int WIDTH = 400;
int HEIGHT = 400;
double Graph_Max_H = 3.3f;
double Graph_Min_H = 0.f;
double[] data1 = new double[ SIZE];
int dataIdx = 0;

void setup()
{
  // I know that the first port in the serial list on my mac
  // is Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  size(400,400);
  String portName = Serial.list()[1]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600); 
}

void Graph(int left, int top, int w, int h,
  double[] graph, int size)
{
  int offsetX = 2;
  strokeWeight(1);
  line(left, h/2 + top, left+w, h/2 + top);
  line(left+offsetX, top, left+offsetX, top + h);
  
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
  //background(255,255,255);

  
  if ( myPort.available() > 0) 
  {
    // If data is available,
    int c = myPort.read();
    if (SetGraph(dataIdx++, c, 0, 256, data1, SIZE))
     {
       dataIdx = 0;
       ClearGraph(data1, SIZE);
     }

    println(c);
  }
  
  Graph(0, 200, 400, 200, data1, dataIdx);  
}



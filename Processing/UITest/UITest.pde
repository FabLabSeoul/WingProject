
boolean btn1 = false;
boolean btn2 = false;
int x = 0, y = 0, w = 50, h=50;

class Button
{
  public int x,y,w,h;
  public String name;
  public int keyCode;
  public boolean clicked;
  Button() {}
  Button(int id, int key0, int x0, int y0, int w0, int h0) {
    x=x0; y=y0; w=w0; h=h0;
    clicked = false;
    name = "  ";
    name += char(key0);
    keyCode = key0;
  }
}

Button[] btns = {
  new Button(0, 'S', 100, 500, w, h),
  new Button(1, 'F', 200, 500, w, h),
  new Button(2, 'E', 150, 450, w, h),
  new Button(3, 'D', 150, 550, w, h),

  new Button(0, 'J', 600, 500, w, h),
  new Button(1, 'L', 700, 500, w, h),
  new Button(2, 'I', 650, 450, w, h),
  new Button(3, 'K', 650, 550, w, h),
};

  
void setup()
{
  size(800,600);  
}

void draw()
{
  background(0);
  
  textSize(24);

  for (int i=0; i < btns.length; ++i)
  {
    fill( btns[i].clicked? 100:255);
    rect(btns[i].x, btns[i].y, btns[i].w, btns[i].h);
    
    fill(100);
    text(btns[i].name, btns[i].x, btns[i].y+(btns[i].h*2/3));        
  }

}


void mousePressed() 
{ 

  for (int i=0; i < btns.length; ++i)
  {
    if (mouseX > btns[i].x && mouseX < btns[i].x+w && 
          mouseY > btns[i].y && mouseY < btns[i].y+h)
    {
       btns[i].clicked = true;
    }
  }
  
}


void mouseReleased()
{
  btn1 = false;
  
  for (int i=0; i < btns.length; ++i)
  {
    btns[i].clicked = false;
  }
  
}

void keyPressed(event e) 
{
  int curKeyIdx = -1;
  if (key >= 'a' && key <= 'z')
    curKeyIdx = key - 'a'; 
  if (key >= 'A' && key <= 'Z')
    curKeyIdx = key - 'A';   

  for (int i=0; i < btns.length; ++i)
  {
    int keyIdx = -1;
    if (btns[i].keyCode >= 'a' && btns[i].keyCode <= 'z')
      keyIdx = btns[i].keyCode - 'a'; 
    if (btns[i].keyCode >= 'A' && btns[i].keyCode <= 'Z')
      keyIdx = btns[i].keyCode - 'A'; 
    
    if (keyIdx == -1)
    {
      btns[i].clicked = key == btns[i].keyCode;
    }
    else
    {
      btns[i].clicked = keyIdx == curKeyIdx;
    }    
  }  
}

void keyReleased()
{
  int curKeyIdx = -1;
  if (key >= 'a' && key <= 'z')
    curKeyIdx = key - 'a'; 
  if (key >= 'A' && key <= 'Z')
    curKeyIdx = key - 'A';   

  for (int i=0; i < btns.length; ++i)
  {
    int keyIdx = -1;
    if (btns[i].keyCode >= 'a' && btns[i].keyCode <= 'z')
      keyIdx = btns[i].keyCode - 'a'; 
    if (btns[i].keyCode >= 'A' && btns[i].keyCode <= 'Z')
      keyIdx = btns[i].keyCode - 'A'; 
    
    if (keyIdx == -1)
    {
      btns[i].clicked = (key == btns[i].keyCode)? false : btns[i].clicked;
    }
    else
    {
      btns[i].clicked = (keyIdx == curKeyIdx)? false : btns[i].clicked;
    }
    
  }  
  
}



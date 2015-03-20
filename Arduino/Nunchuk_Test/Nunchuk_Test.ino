/*출처 : 아두이노 쿡북 
  최종수정 : www.kocoafab.cc */
#include <Wire.h> // Wire 라이브러리를 초기화 합니다.
const int vccPin = A3;// 아날로그 3 핀이 눈차크의 전원을 제공합니다.
const int gndPin = A2;// 아날로그 2 핀이 그라운드 를 제공합니다.


const int dataLength =6;// 요청할 데이터의 길이(바이트의 수)를 정합니다.
static byte rawData[dataLength];// 눈차크 데이터를 저장할 배열을 설정합니다.
int offsetX, offsetY, offsetZ;

enum nunchuckItems { JOY_X, JOY_Y, ACCEL_X, ACCEL_Y, ACCEL_Z, BTN_Z, BTN_C }; 
// 각각 조이스틱 x축, 조이스틱 y축, 가속도 y, 가속도 z , 버튼 z , 버튼 c 를 배열합니다.

void setup(){ //전원핀의 초기상태를 안정화 하기 위해 아래의 설정을 합니다.
  pinMode(gndPin, OUTPUT);//그라운드핀을 출력으로 설정합니다
  pinMode(vccPin, OUTPUT);// 전원핀을 출력으로 설정합니다.
  digitalWrite(gndPin,LOW);//그라운드 핀을 low상태로 설정합니다.
  digitalWrite(vccPin,HIGH);// 전원핀을 high상태로 설정합니다.
  delay(100);//100밀리 초동안 대기합니다.
  
  Serial.begin(9600); //시리얼 통신을 준비합니다.
  nunchuckInit();// 눈차크와의 I2C 통신을 설정합니다.
  
}

void loop()
{
  nunchuckRead();
 /*
  int acceleration = getValue(accelX);
  if((acceleration >= 75) && (acceleration <= 185))
  {
    byte x = map(acceleration, 75,185,0,63); 
    //map함수는 75부터 185 사이의 값에 대해 0부터 63 사이의 값을 반환 합니다.
    Serial.write(x);
  }
 */
  
  
  delay(6);
  boolean btnC = nunchuckGetValue(BTN_C);
  boolean btnZ = nunchuckGetValue(BTN_Z);
  
  if(btnC) {
    offsetX = 127 - nunchuckGetValue(ACCEL_X); 
    offsetY = 127 - nunchuckGetValue(ACCEL_Y);
    offsetZ = 127 - nunchuckGetValue(ACCEL_Z);    
  }
  
  Serial.print("Start,");
  printJoy(nunchuckGetValue(JOY_X)); 
  printJoy(nunchuckGetValue(JOY_Y)); 
  printAccel(nunchuckGetValue(ACCEL_X), offsetX);
  printAccel(nunchuckGetValue(ACCEL_Y), offsetY);
  printAccel(nunchuckGetValue(ACCEL_Z), offsetZ);  
  printButton(nunchuckGetValue(BTN_Z));
  printButton(nunchuckGetValue(BTN_C));
  
  // 프로세싱에서 자료를 받아서 분석 하기 위해 "Data, 조이패드 X값, 조이패드 Y값, Z버튼, 가속도 Y값, 가속도 X값" 순서로 보낸다.
  // 프로세싱에서는 아두이노에서 보낸 값을 " , " 로 나눠서 값을 저장한다. 

  Serial.println();
  
  delay(20);
}

void nunchuckInit()
{
  Wire.begin(); //i2c 버스에 연결합니다.
  Wire.beginTransmission(0x52);// 0x52로 전송합니다.
  Wire.write((byte)0x40);//메모리 주소를 전송합니다.
  Wire.write((byte)0x00);//0을 전송합니다.
  Wire.endTransmission();//전송종료
}

// 눈차크에게 데이터를 요청합니다.
static void nunchuckRequest()
{
  Wire.beginTransmission(0x52);//0x52로 전송합니다.
  Wire.write((byte)0x00);//1을 전송합니다.
  Wire.endTransmission();//전송종료  
}

//눈차크로부터 데이터를 수신합니다.
boolean nunchuckRead()
{
  int cnt = 0;
  Wire.requestFrom (0x52,dataLength);//데이터 요청
  while(Wire.available()){
    rawData[cnt] = nunchuckDecode(Wire.read());
    cnt++;
  }
  nunchuckRequest();//다음 데이터 요청
  if (cnt >= dataLength)
  return true;//6바이트가 수신되면 성공
  else
  return false;
}

static char nunchuckDecode (byte x){// 데이터를 인코딩 합니다.
  return (x ^ 0x17) + 0x17;
}
int nunchuckGetValue(int item)
{
  if(item <=ACCEL_Z)
  return (int)rawData[item];
  else if (item == BTN_Z)
  return bitRead(rawData[5],0) ? 0:1;
  else if (item ==BTN_C)
  return bitRead(rawData[5],1)? 0: 1;
}

void printAccel(int value, int offset){ // 가속도 값을 출력 하는 함수
  Serial.print(adjReading(value, 127-50, 127+50, offset));
  Serial.print(",");
}

void printJoy(int value){ // 조이패드 값을 출력하는 함수
  Serial.print(adjReading(value,0, 255, 0));
  Serial.print(",");
}

void printButton(int value){ // 버튼 값을 출력하는 함수
  if( value != 0)
     value = 127;
  Serial.print(value,DEC);
  Serial.print(",");
}

int adjReading( int value, int min, int max, int offset){ // 받은 값의 범위를 재설정 해주는 함수
   value = constrain(value + offset, min, max);
   // 범위 한정(min값 보다 낮을경우 min값으로, max값보다 클 경우 max값으로 바꿔준다.)
   value = map(value, min, max, -127, 127); // 값의 범위 재설정
   return value;  
}


//
// MPU-6050 test project
//
//
// Hari's 라이브러리를 이용함.
// MPU-6050 과 I2C 통신하는 부분 이용
// https://harinadha.wordpress.com/2012/05/23/mpu6050lib/
//
//
// MPU-6050으로 부터 받은 정보를 실제 사용할 값으로 변환하는 부분을 참조.
// http://m.kocoafab.cc/tutorial/view/239
//
//

#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "mpu6050.h"
#include <math.h>
#include "tick.h"



//센서에서 바로 읽어온 값을 저장할 변수
float    base_x_accel;
float    base_y_accel;
float    base_z_accel;

float    base_x_gyro;
float    base_y_gyro;
float    base_z_gyro;


//변경된 각도와 최종 각도를 저장할 변수
uint32_t last_read_time;
float         last_x_angle;  
float         last_y_angle;
float         last_z_angle;  
float         last_gyro_x_angle;  
float         last_gyro_y_angle;
float         last_gyro_z_angle;


uint32_t get_last_time(void) {return last_read_time;}
void calibrate_sensors(void);
float get_last_x_angle(void) {return last_x_angle;}
float get_last_y_angle(void) {return last_y_angle;}
float get_last_z_angle(void) {return last_z_angle;}
float get_last_gyro_x_angle(void) {return last_gyro_x_angle;}
float get_last_gyro_y_angle(void) {return last_gyro_y_angle;}
float get_last_gyro_z_angle(void) {return last_gyro_z_angle;}



//각도를 바꾸는 함수
void set_last_read_angle_data(uint32_t time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) 
{
  last_read_time = time;
  last_x_angle = x;
  last_y_angle = y;
  last_z_angle = z;
  last_gyro_x_angle = x_gyro;
  last_gyro_y_angle = y_gyro;
  last_gyro_z_angle = z_gyro;
}


void SerialSetup(void);
void print_byte(short  c);
void print_short(short  c);
void __attribute__((weak)) osa_mdelay(unsigned int msec);
void delay(unsigned int msec);
void SendSerialAccelGryro( int16_t accelgyro[6] );
void InitSysTick(void);


int main()
{
	int16_t  AccelGyro[6]={0};
	int curTick=0, oldTick=0;
	
	InitSysTick();
	
	SerialSetup();
	
	MPU6050_I2C_Init();
	MPU6050_Initialize();

	calibrate_sensors();
	set_last_read_angle_data(GetTickCount(), 0, 0, 0, 0, 0, 0);

	
	if (MPU6050_TestConnection() != 0)
	{
		 // connection success
	}
	else
	{
		 // connection failed
		return 0;
	}
 	
	while(1)
	{
			curTick = GetTickCount();
		
			if (curTick - oldTick > 10)
			{
				MPU6050_GetRawAccelGyro(AccelGyro);
		
				SendSerialAccelGryro(  AccelGyro );
				
				oldTick = curTick;
			}
			
	}
}



 void SerialSetup(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	

	// Port 설정
	// PA9  - Tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PA10  - Rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// UART Port 설정
	USART_InitStructure.USART_BaudRate    = 19200;
	USART_InitStructure.USART_WordLength   = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits    = USART_StopBits_1;
	USART_InitStructure.USART_Parity    = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode    = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}


void USART_SendData2(USART_TypeDef* USARTx, int16_t Data)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* Transmit Data */
  USARTx->DR = (Data & (int16_t)0x01FF);	
}

void print_byte(short  c)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	USART_SendData2(USART1, c);
}


void print_short(short  c)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	USART_SendData2(USART1, c >> 8);
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
	USART_SendData2(USART1, c & 0xFF);	
}


void __attribute__((weak)) osa_mdelay(unsigned int msec)
{
	uint32_t temp;
	SysTick->LOAD=(uint32_t)msec*(HSE_VALUE/1000);
	SysTick->VAL =0x00;		// clear Count flag
	SysTick->CTRL=0x01;
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));	// wait Count flag set
	SysTick->CTRL=0x00;
	SysTick->VAL =0X00;
}

void delay(unsigned int msec)
{
	while (msec-->0) {}
}



//움직임이 없을 때 일차적으로 값을 읽어와 전역변수에 저장
void calibrate_sensors() 
{
	int i = 0;
  int                   num_readings = 10;
  float                 x_accel = 0;
  float                 y_accel = 0;
  float                 z_accel = 0;
  float                 x_gyro = 0;
  float                 y_gyro = 0;
  float                 z_gyro = 0;
	int16_t  AccelGyro[6]={0};
	
   
  //원시 데이터들의 평균 읽기
  for (i = 0; i < num_readings; i++) 
	{
		MPU6050_GetRawAccelGyro(AccelGyro);
    
    x_accel += AccelGyro[0];
    y_accel += AccelGyro[1];
    z_accel += AccelGyro[2];
    x_gyro += AccelGyro[3];
    y_gyro += AccelGyro[4];
    z_gyro += AccelGyro[5];
		
    delay(10000);
  }
	
  x_accel /= num_readings;
  y_accel /= num_readings;
  z_accel /= num_readings;
  x_gyro /= num_readings;
  y_gyro /= num_readings;
  z_gyro /= num_readings;
  
  //전역 변수에 저장
  base_x_accel = x_accel;
  base_y_accel = y_accel;
  base_z_accel = z_accel;
  base_x_gyro = x_gyro;
  base_y_gyro = y_gyro;
  base_z_gyro = z_gyro;
}



//
// 소스 참조. 아두이노 소스를 stm32 소스로 바꿨다.
// http://m.kocoafab.cc/tutorial/view/239
//
void SendSerialAccelGryro( int16_t accelgyro[6] )
{
	//  250 degree/s 범위를 가지고, 이 값을 16 bit 분해능으로 표현하고 있으므로, 
	// mpu-6050에서 보내는 값은 -32766 ~ +32766 값이다.
	// 그러므로 이 값을 실제 250 degree/s 로 변환하려면 131로 나눠줘야 한다. 범위가
	// 다르면 이 값도 같이 바껴야한다.
	float FS_SEL = 131;
	
	  //회전을 했을 떄 시간 알기
  unsigned long t_now = GetTickCount();

	
  float gyro_x = (accelgyro[3] - base_x_gyro)/FS_SEL;
  float gyro_y = (accelgyro[4] - base_y_gyro)/FS_SEL;
  float gyro_z = (accelgyro[5] - base_z_gyro)/FS_SEL;

	  
// 가속도 값 범위는?
// 16bit 니 -32766 ~ +32766 범위이고,
//	 +-2g 범위라면 mpu-6050으로부터 넘어온 값을 실제 g단위로 환산하려면
//	  scale factor(16384)로 나눠줘야 한다. +-2g 범위는 16,384값이다. 즉 32766값이면 2가 된다.	
	
  //acceleration 원시 데이터 저장
  float accel_x = accelgyro[0];
  float accel_y = accelgyro[1];
  float accel_z = accelgyro[2];
	
	 
  //accelerometer로 부터 각도 얻기
  float RADIANS_TO_DEGREES = 180/3.14159;

  // float accel_vector_length = sqrt(pow(accel_x,2) + pow(accel_y,2) + pow(accel_z,2));
  float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
  float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
  float accel_angle_z = 0;
	

  //gyro angles 계산1
  float dt =(t_now - get_last_time())/1000.0;
  float gyro_angle_x = gyro_x*dt + get_last_x_angle();
  float gyro_angle_y = gyro_y*dt + get_last_y_angle();
  float gyro_angle_z = gyro_z*dt + get_last_z_angle();
  
  //gyro angles 계산2
  float unfiltered_gyro_angle_x = gyro_x*dt + get_last_gyro_x_angle();
  float unfiltered_gyro_angle_y = gyro_y*dt + get_last_gyro_y_angle();
  float unfiltered_gyro_angle_z = gyro_z*dt + get_last_gyro_z_angle();
  
  //알파를 이용해서 최종 각도 계산3
  float alpha = 0.96;
  float angle_x = alpha*gyro_angle_x + (1.0 - alpha)*accel_angle_x;
  float angle_y = alpha*gyro_angle_y + (1.0 - alpha)*accel_angle_y;
  float angle_z = gyro_angle_z;  //Accelerometer는 z-angle 없음	
	
	
 //최종 각도 저장
  set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
 	
	// 시작 문자 전송
	print_byte('S');	
	
	print_byte( (int16_t)dt );
	
	// 2 바이트 정수로 보내기 위해 100을 곱하고, 받을 때, 100을 나눠준다. 
	print_short( (int16_t)(accel_angle_x*100) );
	print_short( (int16_t)(accel_angle_y*100) );
	print_short( (int16_t)(accel_angle_z*100) );

	print_short( (int16_t)(unfiltered_gyro_angle_x*100) );
	print_short( (int16_t)(unfiltered_gyro_angle_y*100) );
	print_short( (int16_t)(unfiltered_gyro_angle_z*100) );
	
	print_short( (int16_t)(angle_x*100) );
	print_short( (int16_t)(angle_y*100) );
	print_short( (int16_t)(angle_z*100) );
}


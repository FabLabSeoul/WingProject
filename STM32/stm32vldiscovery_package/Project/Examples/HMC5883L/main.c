//
//
// HMC5883L 지자기 센서 정보를 인식해서 시리얼통신으로 보낸다.
//
//

#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "HMC5883L.h"
#include "tick.h"


void SerialSetup(void);
void print_byte(short  c);
void print_short(short  c);
void __attribute__((weak)) osa_mdelay(unsigned int msec);
void delay(unsigned int msec);
void SendSerialMagnetometer( int16_t accelgyro[3] );


void setup()
{
	InitSysTick();
	SerialSetup();
	
	HMC5883L_I2C_Init();
	HMC5883L_Initialize();	
}


int main()
{
	int curTick=0, oldTick=0;
	int16_t  MagneticHeading[3]={0};
	
	setup();
	
	if( HMC5883L_TestConnection())
	{
		 // connection success
	}
	else
	{
		 // connection failed
		return 1;
	}
	 
	
	while(1)
	{
		curTick = GetTickCount();

		if (curTick - oldTick > 50)
		{
			HMC5883L_GetHeading(MagneticHeading);
			
			SendSerialMagnetometer(MagneticHeading);
		
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


// 지자기 센서 정보를 시리얼통신으로 보낸다.
void SendSerialMagnetometer( int16_t magnetometer[3] )
{
	int16_t mag_x = magnetometer[0];
	int16_t mag_y = magnetometer[2];
	int16_t mag_z = magnetometer[1];	
	
	print_byte( 'S' );
	print_short( mag_x );
	print_short( mag_y );
	print_short( mag_z );	
}

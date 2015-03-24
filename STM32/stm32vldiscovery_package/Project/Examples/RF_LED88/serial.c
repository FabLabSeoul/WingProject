
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "serial.h"


uint8_t TxBuffer1[TxBufferSize1];
uint8_t RxBuffer1[TxBufferSize1];



 void SerialSetup(uint16_t baudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// RCC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

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
	USART_InitStructure.USART_BaudRate    = baudRate;
	USART_InitStructure.USART_WordLength   = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits    = USART_StopBits_1;
	USART_InitStructure.USART_Parity    = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode    = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}



void DMA_SerialSetup(uint16_t baudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
	

	// RCC Configuration
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

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
	USART_InitStructure.USART_BaudRate    = baudRate;
	USART_InitStructure.USART_WordLength   = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits    = USART_StopBits_1;
	USART_InitStructure.USART_Parity    = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode    = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);



	// Init DMA
  // USART1_Tx_DMA_Channel (triggered by USART1 Tx event) Config
  DMA_DeInit(DMA1_Channel4);	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  
  // USART1_Rx_DMA_Channel (triggered by USART1 Rx event) Config
	DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);


	// Enable USART1 DMA TX request
  USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);

  // Enable USART1
  USART_Cmd(USART1, ENABLE);

	// Enable USART1 DMA TX, RX Channel
  //DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);

	USART_Cmd(USART1, ENABLE);	
}


void sendData()
{
	uartStartTxDMA(DMA1_Channel4);	
}


void uartStartTxDMA(DMA_Channel_TypeDef *txDMAChannel)
{
    txDMAChannel->CMAR = (uint32_t)TxBuffer1;
    txDMAChannel->CNDTR = TxBufferSize1;
    DMA_Cmd(txDMAChannel, ENABLE);
}


bool isReceiveData(void)
{
	if (DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET)
		return FALSE;

		return TRUE;
}

void uartStartRxDMA(void)
{
	DMA_ClearITPendingBit(DMA1_IT_TC5);
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

void __attribute__((weak)) delay_us(unsigned int usec)
{
	uint32_t temp;
	SysTick->LOAD=(uint32_t)usec*(HSE_VALUE/1000000);
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


uint16_t receiveData(void)
{
		//while (!(USART1->SR & USART_FLAG_RXNE));
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{
		}
	
		return USART_ReceiveData(USART1);	
}

char receiveData2(void)
{
	uint16_t data;
	
		//while (!(USART1->SR & USART_FLAG_RXNE));
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{
		}
	
		data = USART_ReceiveData(USART1);
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		return *(char*)&data;
}


void putc ( void* p, char c)
{
	//while (!SERIAL_PORT_EMPTY) ;
	//SERIAL_PORT_TX_REGISTER = c;
	print_byte(c);
}
	

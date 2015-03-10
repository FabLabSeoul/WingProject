
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_dma.h"

//
// DMA를 이용해 시리얼통신을 하는 예제다.
// TxBuffer1 에 있는 내용을 시리얼로 보내고, 받은 정보는 RxBuffer1에 저장된다.
// 받은 정보가 TxBufferSize1 만큼 받아야 DMA_GetFlagStatus(DMA1_FLAG_TC5) == SET 이 된다.
// 
// 


/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define TxBufferSize2   (countof(TxBuffer2) - 1)

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
uint8_t TxBuffer1[] = "USART DMA Interrupt: USARTy -> USARTz using DMA Tx and Rx Flag";
//uint8_t TxBuffer2[] = "USART DMA Interrupt: USARTz -> USARTy using DMA Tx and Rx Interrupt";
uint8_t RxBuffer1[TxBufferSize1];
//uint8_t RxBuffer2[TxBufferSize1];
uint8_t NbrOfDataToRead = TxBufferSize1;
uint8_t index = 0;
int nnn = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void DMA_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void uartStartTxDMA(DMA_Channel_TypeDef *txDMAChannel);
	

int main()
{
  /* System Clocks Configuration */
  RCC_Configuration();
 
  /* NVIC configuration */
  //NVIC_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();

  /* Configure the DMA */
  DMA_Configuration();	
	

  // USART1 초기화	
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	
  USART_Init(USART1, &USART_InitStructure);
	
  // Enable USART1 DMA TX request
  USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);

  // Enable USART1
  USART_Cmd(USART1, ENABLE);

	// Enable USART1 DMA TX, RX Channel
  DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	
	while(1)
	{
		int i=0;

		while (DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET)
		{
		}

		// Wait until USARTy RX DMA1 Channel Transfer Complete 
		while (DMA_GetFlagStatus(DMA1_FLAG_TC5) == RESET)
		{
			
		}

		// TxBufferSize1 길이 만큼 시리얼 입력을 받은 후에 똑같은 문자열을 다시 보낸다.
		// 그리고, 다시 시리얼입력을 들어 올 때 까지 대기한다.
		
		for (i=0; i < TxBufferSize1; ++i)
		{
			TxBuffer1[ i] = RxBuffer1[ i];
		}

		
    DMA_ClearITPendingBit(DMA1_IT_TC4);
		DMA_ClearITPendingBit(DMA1_IT_TC5);
    DMA_Cmd(DMA1_Channel4, DISABLE);
		
		uartStartTxDMA(DMA1_Channel4);
	}
	
}


void uartStartTxDMA(DMA_Channel_TypeDef *txDMAChannel)
{
    txDMAChannel->CMAR = (uint32_t)TxBuffer1;
    txDMAChannel->CNDTR = TxBufferSize1;
    DMA_Cmd(txDMAChannel, ENABLE);
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  // DMA clock enable
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  // Enable GPIO clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
}


/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}


/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTz Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the DMA.
  * @param  None
  * @retval None
  */
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  // USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config
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
  
  // USARTy_Rx_DMA_Channel (triggered by USARTy Rx event) Config
	DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer1;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *   FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}



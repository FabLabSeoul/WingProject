
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_dma.h"

//
// 인터럽트를 이용해 시리얼통신을 하는 예제다.
// Serial Port Terminal 프로그램과 정보를 주고 받는다.
//
// USART1 을 이용한다. PA10,9 (Rx,Tx)
//
// 1. TxBuffer 에 있는 내용을 시리얼로 보낸다.
// 2. 받은 정보는 RxBuffer에 저장된다.
// 3. 일정 이상의 정보를 받으면, 그대로 다시 보낸다.
// 4. 2번으로 다시 돌아간다. (계속 반복)
//


/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize   (countof(TxBuffer) - 1)

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
uint8_t TxBuffer[] = "USART Interrupt: USART1 Transmitt Data";
uint8_t RxBuffer[TxBufferSize];
uint8_t NbrOfDataToRead = TxBufferSize;
uint8_t NbrOfDataToTransfer = TxBufferSize;
uint8_t TxCounter = 0;
uint8_t RxCounter = 0;


/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void SendSerial(uint8_t buffer[], uint8_t size);


int main()
{
  /* System Clocks Configuration */
  RCC_Configuration();
 
  /* NVIC configuration */
  NVIC_Configuration();

  /* Configure the GPIO ports */
  GPIO_Configuration();
	

  // USART1 초기화	
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
	
  USART_Init(USART1, &USART_InitStructure);
	
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	
  // Enable USART1
  USART_Cmd(USART1, ENABLE);


	while (1)
	{
	}	
}



/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  // Enable GPIO clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
}


/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  /* Configure USART1 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  
  /* Configure USART1 Tx as alternate function push-pull */
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

  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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


void SendSerial(uint8_t buffer[], uint8_t size)
{
	int i=0;
	
	TxCounter = 0;
	NbrOfDataToTransfer = size;

	for (i=0; i < size; ++i)
	{
		TxBuffer[ i] = buffer[ i];
	}			

	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}


/**
  * @brief  This function handles USARTy global interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
	
	// 시리얼로 부터 받은 정보를 계속 저장한다.
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    RxBuffer[RxCounter++] = USART_ReceiveData(USART1);

	
    if(RxCounter == NbrOfDataToRead)
    {
			// NbrOfDataToRead 만큼 정보를 받았다면, 그대로 전송한다.
			SendSerial(RxBuffer, RxCounter);
			
			RxCounter = 0;			
    }
  }
  
	// 시리얼로 데이타를 모두 보내면, 인터럽트를 끈다.
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    USART_SendData(USART1, TxBuffer[TxCounter++]);

		
    if(TxCounter == NbrOfDataToTransfer)
    {
      /* Disable the USART1 Transmit interrupt */
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }    
  }
}


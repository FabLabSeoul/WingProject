//
// I2C Interrupt mode test
//
// 이 소스는 STM32F10x_StdPeriph_Lib_V3.3.0\Project\STM32F10x_StdPeriph_Examples\I2C\Interrupt  
// 프로젝트를 참조했다.
//


#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_i2c.h"
#include "serial.h"
#include "printf.h"


#define Tx1BufferSize           4 
#define Tx2BufferSize           4 
#define I2C2_SLAVE_ADDRESS7     0x30
#define Transmitter             0x00
#define Receiver                0x01

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

#define I2C1_SLAVE_ADDRESS7    0x30
#define I2C2_SLAVE1_ADDRESS7   0x30
#define BufferSize             4 
#define ClockSpeed             200000

I2C_InitTypeDef  I2C_InitStructure;
NVIC_InitTypeDef  NVIC_InitStructure;

uint8_t I2C1_Buffer_Tx[BufferSize] = {1,2,3,4}, I2C1_Buffer_Rx[BufferSize];
uint8_t I2C2_Buffer_Tx[BufferSize] = {5,6,7,8}, I2C2_Buffer_Rx[BufferSize+1];

uint8_t Tx_Idx = 0, Rx_Idx = 0;
__IO uint8_t Tx1_Idx = 0, Rx1_Idx = 0, Tx2_Idx = 0, Rx2_Idx = 0;
__IO uint8_t Direction = Transmitter;

volatile TestStatus TransferStatus1 = FAILED;
volatile TestStatus TransferStatus2 = FAILED;
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void putc ( void* p, char c);


// I2C1, I2C initialize
void setup()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// Initialize SerialPort, PA10,9 - Rx,Tx 
	SerialSetup();

	init_printf(0, putc);
	printf( "init_printf()\n" );

	
	// Clock Enable
  // GPIOB Periph clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  // I2C1 and I2C2 Periph clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2, ENABLE);
	
	// GPIO Enable
	// Configure I2C1 pins: SCL and SDA
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // Configure I2C2 pins: SCL and SDA
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Configuration Setting
  // Enable I2C1 and I2C2 
  I2C_Cmd(I2C1, ENABLE);
  I2C_Cmd(I2C2, ENABLE);

  // I2C1 configuration
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
  I2C_Init(I2C1, &I2C_InitStructure);
  // I2C2 configuration
  I2C_InitStructure.I2C_OwnAddress1 = I2C2_SLAVE1_ADDRESS7;
  I2C_Init(I2C2, &I2C_InitStructure);	
	
	I2C_CalculatePEC(I2C1, ENABLE);
	I2C_CalculatePEC(I2C2, ENABLE);
	
	// NVIC Configuration
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* Configure and enable I2C1 event interrupt -------------------------------*/
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C2 event interrupt -------------------------------*/  
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C2 error interrupt -------------------------------*/  
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_Init(&NVIC_InitStructure);	
	
  /* Enable I2C1 event and buffer interrupts */
  I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
  /* Enable I2C1 event and buffer interrupts */
  I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
	
}



int main()
{
	
	setup();
	
	
	
	  /*----- Transmission Phase -------------------------------------------------*/
  /* Set data direction to transmitter */
  Direction = Transmitter;
  /* Send I2C1 START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Wait until all data and the PEC value are received */
  /* I2C2_Buffer_Rx buffer will contain the data plus the PEC value */
  while(Rx2_Idx < Tx1BufferSize)
  {
  }
  
  /* Check the corectness of the I2C1 transmitted data */
  TransferStatus1 = Buffercmp(I2C1_Buffer_Tx, I2C2_Buffer_Rx, Tx1BufferSize);
  /* TransferStatus1 = PASSED, if the transmitted and received data
     are equal */
  /* TransferStatus1 = FAILED, if the transmitted and received data 
     are different */     

	
  /*----- Reception Phase --------------------------------------------------*/
  /* Re-configure and enable I2C1 event interrupt to have the higher priority */
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
 
  /* Wait until end of Slave transmission */
  while(Rx1_Idx < Tx2BufferSize)
  {
  }

  /* Check the corectness of the I2C1 received data */
  TransferStatus2 = Buffercmp(I2C2_Buffer_Tx, I2C1_Buffer_Rx, Tx2BufferSize);
  /* TransferStatus2 = PASSED, if the transmitted and received data
     are equal */
  /* TransferStatus2 = FAILED, if the transmitted and received data 
     are different */
   
	printf("TransferStatus1 = %d\n", TransferStatus1 );	
	printf("TransferStatus2 = %d\n", TransferStatus2 );
	
  while(1)
  {
		
	}	
	
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


void putc ( void* p, char c)
{
	//while (!SERIAL_PORT_EMPTY) ;
	//SERIAL_PORT_TX_REGISTER = c;
	print_byte(c);
}
	





/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles I2C1 Event interrupt request.
  * @param  None
  * @retval None
  */
void I2C1_EV_IRQHandler(void)
{
	
  switch (I2C_GetLastEvent(I2C1))
  {
    case I2C_EVENT_MASTER_MODE_SELECT:                 /* EV5 */
      if(Direction == Transmitter)
      {
        /* Master Transmitter ----------------------------------------------*/
        /* Send slave Address for write */
        I2C_Send7bitAddress(I2C1, I2C2_SLAVE_ADDRESS7, I2C_Direction_Transmitter);
      }
      else
      {
        /* Master Receiver -------------------------------------------------*/
        /* Send slave Address for read */
        I2C_Send7bitAddress(I2C1, I2C2_SLAVE_ADDRESS7, I2C_Direction_Receiver);
       
      }
      break;
        
    /* Master Transmitter --------------------------------------------------*/
    /* Test on I2C1 EV6 and first EV8 and clear them */
    case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:  
      
      /* Send the first data */
      I2C_SendData(I2C1, I2C1_Buffer_Tx[Tx1_Idx++]); 
      break;

    /* Test on I2C1 EV8 and clear it */
  case I2C_EVENT_MASTER_BYTE_TRANSMITTING:  /* Without BTF, EV8 */     
      if(Tx1_Idx < (Tx1BufferSize))
      {
        /* Transmit I2C1 data */
        I2C_SendData(I2C1, I2C1_Buffer_Tx[Tx1_Idx++]);
      
      }
      else
      {
        I2C_TransmitPEC(I2C1, ENABLE); 
        I2C_ITConfig(I2C1, I2C_IT_BUF, DISABLE);
      }            
      break;

    case I2C_EVENT_MASTER_BYTE_TRANSMITTED: /* With BTF EV8-2 */
        I2C_ITConfig(I2C1, I2C_IT_BUF, ENABLE);
        /* I2C1 Re-START Condition */
        I2C_GenerateSTART(I2C1, ENABLE);
      break;

    /* Master Receiver -------------------------------------------------------*/
    case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:
      if(Tx2BufferSize == 1)
      {
        /* Disable I2C1 acknowledgement */
        I2C_AcknowledgeConfig(I2C1, DISABLE);
        /* Send I2C1 STOP Condition */
        I2C_GenerateSTOP(I2C1, ENABLE);
      }
      break;

   /* Test on I2C1 EV7 and clear it */
   case I2C_EVENT_MASTER_BYTE_RECEIVED:
     /* Store I2C1 received data */
     I2C1_Buffer_Rx[Rx1_Idx++] = I2C_ReceiveData (I2C1);
     /* Disable ACK and send I2C1 STOP condition before receiving the last data */
     if(Rx1_Idx == (Tx2BufferSize - 1))
     {
       /* Disable I2C1 acknowledgement */
       I2C_AcknowledgeConfig(I2C1, DISABLE);
       /* Send I2C1 STOP Condition */
       I2C_GenerateSTOP(I2C1, ENABLE);
     }
     break;

    default:
      break;
  }
}

/**
  * @brief  This function handles I2C2 Event interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_EV_IRQHandler(void)
{
  switch (I2C_GetLastEvent(I2C2))
  {
    /* Slave Transmitter ---------------------------------------------------*/
    case I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED:  /* EV1 */
    
      /* Transmit I2C2 data */
      I2C_SendData(I2C2, I2C2_Buffer_Tx[Tx2_Idx++]);
      break;

   case I2C_EVENT_SLAVE_BYTE_TRANSMITTED:             /* EV3 */
      /* Transmit I2C2 data */
      I2C_SendData(I2C2, I2C2_Buffer_Tx[Tx2_Idx++]);
      break; 
  

    /* Slave Receiver ------------------------------------------------------*/
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:     /* EV1 */
      break;

    case I2C_EVENT_SLAVE_BYTE_RECEIVED:                /* EV2 */
      /* Store I2C2 received data */
      I2C2_Buffer_Rx[Rx2_Idx++] = I2C_ReceiveData(I2C2);

      if(Rx2_Idx == Tx2BufferSize)
      { 
        I2C_TransmitPEC(I2C2, ENABLE);  
        Direction = Receiver;
      }
      break; 

    case I2C_EVENT_SLAVE_STOP_DETECTED:                /* EV4 */
      /* Clear I2C2 STOPF flag: read of I2C_SR1 followed by a write on I2C_CR1 */
      (void)(I2C_GetITStatus(I2C2, I2C_IT_STOPF));
      I2C_Cmd(I2C2, ENABLE);
      break;
   
    default:
      break;
  }
}

/**
  * @brief  This function handles I2C2 Error interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_ER_IRQHandler(void)
{
  /* Check on I2C2 AF flag and clear it */
  if (I2C_GetITStatus(I2C2, I2C_IT_AF)) 
  {
    I2C_ClearITPendingBit(I2C2, I2C_IT_AF);
  }
}


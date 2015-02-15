//
// ADC Channel 2 Example
//


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"


double x = 0, y = 0;
GPIO_InitTypeDef myGPIO;
ADC_InitTypeDef myADC;
DMA_InitTypeDef DMA_InitStructure;
 __IO uint32_t ADC_DualConvertedValueTab[2];
 

//Configure other I/O pins
void GPIO_config(void)
{
	// initialize LED C8, 9 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_StructInit(&myGPIO);
    myGPIO.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    myGPIO.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &myGPIO);
}

 
void adc_config()
{ 
  // PC1, PC2를 analog input 모드로 설정한다.  
	myGPIO.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2; //set to pin 1, 2
	myGPIO.GPIO_Mode = GPIO_Mode_AIN; //set as analog input
	GPIO_Init(GPIOC, &myGPIO); //set to C1, C2


  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //clock for ADC (max 14MHz, 72/6=12MHz)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //enable ADC clock


	// DMA1 channel1 configuration ----------------------------------------------
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // 32bit
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; // 32bit
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	// Enable DMA1 Channel1
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	
	//configure ADC1 parameters
	myADC.ADC_Mode = ADC_Mode_Independent;
	myADC.ADC_ScanConvMode = ENABLE;
	myADC.ADC_ContinuousConvMode = ENABLE;
	myADC.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	myADC.ADC_DataAlign = ADC_DataAlign_Right;
	myADC.ADC_NbrOfChannel  = 2;
	ADC_Init(ADC1, &myADC);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5); //PC1 as Input
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_55Cycles5); //PC2 as Input
	ADC_DMACmd(ADC1, ENABLE);
	

	//enable
	ADC_Cmd(ADC1, ENABLE);

	//Calibrate ADC *optional?
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

  
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    GPIO_config(); //configure pins
    adc_config(); //configure ADC
	
    while(1)
    {
				x = ADC_DualConvertedValueTab[0] * 3.3/4095; //get analog value and convert to volts, 12bit ADC
 
        if(x > 2)
        {
          GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);//turn LED on 1
        }
        else 
        {
          GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);//turn LED off 0
        }
				
				y = ADC_DualConvertedValueTab[1] * 3.3/4095; //get analog value and convert to volts, 12bit ADC
 
        if(y > 2)
        {
          GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);//turn LED on 1
        }
        else 
        {
          GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);//turn LED off 0
        }
    }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    Examples/GPIOToggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    09/13/2010
  * @brief   Main program body.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"


/** @addtogroup Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//GPIO_InitTypeDef GPIO_InitStructure;

double x = 0;
GPIO_InitTypeDef myGPIO;
ADC_InitTypeDef myADC;


/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);

/* Private functions ---------------------------------------------------------*/

 
void adc_config()
{ 
	// ADC1 setup
  // PA6를 analog input 모드로 설정한다.  
	myGPIO.GPIO_Pin = GPIO_Pin_6; //set to pin 6
	myGPIO.GPIO_Mode = GPIO_Mode_AIN; //set as analog input
	GPIO_Init(GPIOA, &myGPIO); //set to A6

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //clock for ADC (max 14MHz, 72/6=12MHz)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //enable ADC clock

	//configure ADC parameters
	myADC.ADC_Mode = ADC_Mode_Independent;
	myADC.ADC_ScanConvMode = DISABLE;
	myADC.ADC_ContinuousConvMode = ENABLE;
	myADC.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	myADC.ADC_DataAlign = ADC_DataAlign_Right;
	myADC.ADC_NbrOfChannel  = 1;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5); //PA6 as Input
	ADC_Init(ADC1, &myADC);

	//enable
	ADC_Cmd(ADC1, ENABLE);

	//Calibrate ADC *optional?
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	//enable ADC to work
	ADC_Cmd(ADC1, ENABLE);
}
 
//get Analog Value at pin
int getPot(void)
{
    return ADC_GetConversionValue(ADC1);
}
 
//Configure other I/O pins
void GPIO_config(void)
{
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    //LED-pinC9
    GPIO_StructInit(&myGPIO);
    myGPIO.GPIO_Pin = GPIO_Pin_9;
    myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    myGPIO.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &myGPIO);
}
 

// The second example is an analog read example. 
// Here we have connected a potentiometer to pin PA6 and are reading the voltage. 
// If it exceeds 2V the LED PC9 turns on, otherwise it’s off.
// https://sourcelion.wordpress.com/2014/09/14/stm32-discovery-get-started-tutorial/
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
        x = getPot()*3.3/4095; //get analog value and convert to volts, 12bit ADC
 
        if(x > 2)
        {
          GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);//turn LED on 1
        }
        else 
        {
          GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);//turn LED off 0
        }
    }
}


/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
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

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

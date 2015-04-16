
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"


// PA6 ������ PWM ��ȣ�� ��µȴ�.
//
// PA7 ������ ������ �Է� �޾� PWM ��ȣ�� �����Ѵ�.
//        - ���������� �޾� 0 ~ 3.3V �� ������ ������.
//
// �������ʹ� 5V �� �޾���� �� �����δ�.
//
// PB7 �ɿ��� PWM ��ȣ�� �޾� duty cycle�� ����Ѵ�.
//			- PA6 - PB7 ���� �����ϸ�, PA6 �ɿ��� ���� PWM ���� ������ �� �ִ�.
//

double x = 0;
GPIO_InitTypeDef myGPIO;
ADC_InitTypeDef myADC;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;

__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint32_t Frequency = 0;


void RCC_Configuration(void);
void Init_ADC(void);
double getPot(void);
void SetPWM(double value);
void NVIC_Configuration(void);
void GPIO_Configuration(void);

	
int main()
{
	// pwm duty ���
	// -90 degree : 3.5% duty cycle
	// 0 degree : 7.5% duty cycle
	// +90 degree: 11.5% duty cycle
	
	// -90 degree : 20000 * 3.5 / 100 = 700
	// 0 degree : 20000 * 7.5 / 100 = 1500
	// +90 degree : 20000 * 11.5 / 100 = 2300
	
	
	RCC_Configuration();
	NVIC_Configuration();
	GPIO_Configuration();
	Init_ADC();


	TIM_TimeBaseStructure.TIM_Prescaler = 24 - 1;  // 24MHz / 24 -> 1MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1; // 50 Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // �ǹ� ����.

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	

	
	// PWM1 Mode configuration: Channel1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1500; // duty cycle 7.5%
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	// pwm channel 1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);	
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_Cmd(TIM3, ENABLE);




  /* TIM3 configuration: PWM Input mode ------------------------
     The external signal is connected to TIM4 CH2 pin (PA.01), 
     The Rising edge is used as active edge,
     The TIM4 CCR2 is used to compute the frequency value 
     The TIM4 CCR1 is used to compute the duty cycle value
  ------------------------------------------------------------ */
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	
  
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  
  TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);

  /* Select the TIM4 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);

  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);

  /* Enable the Master/Slave Mode */
  TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);

  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
  
  
  

	STM32vldiscovery_LEDInit(LED3);
	STM32vldiscovery_LEDInit(LED4);	
	
	STM32vldiscovery_LEDOff(LED3);
	STM32vldiscovery_LEDOff(LED4);	
	
	
	while (1)
	{
		SetPWM( getPot() / 3.3 );
		
		if (getPot() > 1)
		{
			STM32vldiscovery_LEDOn(LED3);
		}
		else
		{
			STM32vldiscovery_LEDOff(LED3);
		}
		
		if (getPot() > 2)
		{
			STM32vldiscovery_LEDOn(LED4);
		}
		else
		{
			STM32vldiscovery_LEDOff(LED4);
		}
	
	}
	

}


void Init_ADC(void)
{
	// ADC1 setup
  // PA7�� analog input ���� �����Ѵ�.  
	myGPIO.GPIO_Pin = GPIO_Pin_7; //set to pin 7
	myGPIO.GPIO_Mode = GPIO_Mode_AIN; //set as analog input
	GPIO_Init(GPIOA, &myGPIO); //set to A7

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //clock for ADC (max 14MHz, 24/6=4MHz)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //enable ADC clock

	//configure ADC parameters
	myADC.ADC_Mode = ADC_Mode_Independent;
	myADC.ADC_ScanConvMode = DISABLE;
	myADC.ADC_ContinuousConvMode = ENABLE;
	myADC.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	myADC.ADC_DataAlign = ADC_DataAlign_Right;
	myADC.ADC_NbrOfChannel  = 1;
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_55Cycles5); //PA7 as Input
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

double absf(double v)
{
	return (v < 0)? -v : v;
}


// value 	: 0 -> -90 degree
// 			: 0.5 -> 0 degree
// 			: 1 -> _90 degree
void SetPWM(double value)
{
	// -90 degree : 20000 * 3.5 / 100 = 700
	// 0 degree : 20000 * 7.5 / 100 = 1500
	// +90 degree : 20000 * 11.5 / 100 = 2300
	static int realPulse = 0;
	double rate = (value * 8) + 3.5;
	int pulse = (int)(20000.0 * rate / 100.0);
	
	static double oldValue = 0;
	if (absf(value - oldValue) < 0.05)
		return;	
	oldValue = value;	
	
	
	realPulse = pulse; // debug ��.
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = realPulse;	

	// pwm channel 1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	
}



//get Analog Value at pin
double getPot(void)
{
    return ADC_GetConversionValue(ADC1) * 3.3/4095;
}
 

void RCC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
  // TIM1, GPIOA and GPIOB clock enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

  // TIM3 and TIM4 clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
	
	
	// TIM3 PWM channel1,2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

	// TIM3 PWM channel3,4
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
}


void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM4 channel 2 pin (PB.07) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void TIM4_IRQHandler(void)
{
	int count = 0;
  /* Clear TIM3 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);

  /* Get the Input Capture value */
  IC2Value = TIM_GetCapture2(TIM4);

  if (IC2Value != 0)
  {
	 count = TIM_GetCapture1(TIM4);
	  
    /* Duty cycle computation */
    DutyCycle = (TIM_GetCapture1(TIM4) * 100) / IC2Value;

    /* Frequency computation */
    Frequency = SystemCoreClock / (IC2Value + 1);
  }
  else
  {
    DutyCycle = 0;
    Frequency = 0;
  }
}


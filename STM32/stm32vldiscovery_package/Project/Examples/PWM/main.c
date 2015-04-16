
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"


// PA6 핀으로 PWM 신호가 출력된다.
//
// PA7 핀으로 입력을 받아 PWM 신호를 조절한다.
//        - 가변저항을 달아 0 ~ 3.3V 를 핀으로 보내자.
//
// 서보모터는 5V 를 달아줘야 잘 움직인다.
//

double x = 0;
GPIO_InitTypeDef myGPIO;
ADC_InitTypeDef myADC;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

void RCC_Configuration(void);
void Init_ADC(void);
double getPot(void);
void SetPWM(double value);

	
int main()
{
	// pwm duty 계산
	// -90 degree : 3.5% duty cycle
	// 0 degree : 7.5% duty cycle
	// +90 degree: 11.5% duty cycle
	
	// -90 degree : 20000 * 3.5 / 100 = 700
	// 0 degree : 20000 * 7.5 / 100 = 1500
	// +90 degree : 20000 * 11.5 / 100 = 2300
	
	
	RCC_Configuration();	
	Init_ADC();


	TIM_TimeBaseStructure.TIM_Prescaler = 24 - 1;  // 24MHz / 24 -> 1MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1; // 50 Hz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 5; // 의미 없음.

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
  // PA7를 analog input 모드로 설정한다.  
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
	double rate = (value * 8) + 3.5;
	int pulse = (int)(20000.0 * rate / 100.0);

	
	static double oldValue = 0;
	if (absf(value - oldValue) < 0.05)
		return;	
	oldValue = value;	
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = pulse;	

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



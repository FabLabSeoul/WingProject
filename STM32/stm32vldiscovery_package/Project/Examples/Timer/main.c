
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_tim.h"


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;

void RCC_Configuration(void);

int g_timerCount = 0;
bool g_ledToggle = FALSE;

int main()
{
	
	RCC_Configuration();
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Prescaler = 24000 - 1; // 1 milli second 마다 clock 이 발생한다. (24MHz / 24K) 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1000; // 1000 millisecond 가 되면 인터럽트를 발생하게 한다.
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 5; // 의미 없음.

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	


	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	
	TIM_Cmd(TIM3, ENABLE);


	STM32vldiscovery_LEDInit(LED3);
	STM32vldiscovery_LEDInit(LED4);	
	
	STM32vldiscovery_LEDOn(LED3);
	STM32vldiscovery_LEDOn(LED4);	
	
	while (1)
	{
		if (g_ledToggle)
		{
			STM32vldiscovery_LEDOff(LED3);
			STM32vldiscovery_LEDOff(LED4);			
		}
		else
		{
			STM32vldiscovery_LEDOn(LED3);
			STM32vldiscovery_LEDOn(LED4);
		}
		
		if (g_timerCount > 1000)
		{
			g_timerCount = 0;
		}
		
	}
}


void TIM3_IRQHandler(void)
{
	// TIM3 이벤트 호출.
	
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
    {
		g_timerCount++;
		g_ledToggle = !g_ledToggle;
		
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); // Clear the interrupt flag
    }	
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* TIM1, GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

  /* TIM3 and TIM4 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
}

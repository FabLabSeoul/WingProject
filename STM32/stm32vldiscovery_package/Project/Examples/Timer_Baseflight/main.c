
#include "global.h"
#include "stm32f10x_tim.h"


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
int g_timerCount = 0;


#ifdef USE_LAME_PRINTF
// gcc/GNU version
static void _putc(void *p, char c)
{
    (void)p;
    serialWrite(g_mainport, c);
}
#else
// keil/armcc version
int fputc(int c, FILE *f)
{
    // let DMA catch up a bit when using set or dump, we're too fast.
   while (!isSerialTransmitBufferEmpty(g_mainport));
   serialWrite(g_mainport, c);
   return c;
}
#endif



int main()
{
	// 클럭, 타이머 초기화
	systemInit();
	
	// 시리얼 통신 초기화.
	serialInit(115200);
	

	TIM_TimeBaseStructure.TIM_Prescaler = 24000 - 1; // 1 millisecond 마다 타이머 카운터가 1씩 올라간다.
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	
	// 루프
	while (1)
	{
		// 시리얼 통신 처리.
		serialCom();
		
		printf( "time %d %d %d\n", g_timerCount , TIM_GetCounter(TIM2), millis() );
		
		delay(10);
	}
		
}


void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
    {
        g_timerCount++;

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // Clear the interrupt flag
    }  	
//	printf( "tim2\n");
}


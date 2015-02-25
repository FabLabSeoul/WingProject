
#include "stm32f10x.h"


uint32_t g_currentTick = 0;


void InitSysTick(void)
{
  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }	
}


void SysTick_Process(void)
{
		++g_currentTick;
}


uint32_t GetTickCount(void)
{
	return g_currentTick;
}

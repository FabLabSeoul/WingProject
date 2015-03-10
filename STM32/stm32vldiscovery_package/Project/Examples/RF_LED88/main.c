
//#include "stm32f10x.h"
//#include "STM32vldiscovery.h"
#include "global.h"



bool setup()
{
	gpio_config_t gpio;
	
	systemInit();

	gpio.pin = Pin_0 | Pin_1 | Pin_2 | Pin_3 | Pin_4 | Pin_5 | Pin_6 | Pin_7;
	gpio.speed = Speed_2MHz;
	gpio.mode = Mode_Out_PP;
	gpioInit(GPIOA, &gpio);	
	
	gpio.pin = Pin_8 | Pin_9;
	gpioInit(GPIOC, &gpio);
	
	SerialSetup(9600);	
	
	delay(100);

	digitalHi(GPIOA, Pin_0);
	digitalHi(GPIOA, Pin_1);
	digitalHi(GPIOA, Pin_2);
	digitalHi(GPIOA, Pin_3);

	digitalHi(GPIOA, Pin_4);
	digitalHi(GPIOA, Pin_5);
	digitalHi(GPIOA, Pin_6);
	digitalHi(GPIOA, Pin_7);	

	delay(100);
	
	digitalLo(GPIOA, Pin_0);
	digitalLo(GPIOA, Pin_1);
	digitalLo(GPIOA, Pin_2);
	digitalLo(GPIOA, Pin_3);

	digitalLo(GPIOA, Pin_4);
	digitalLo(GPIOA, Pin_5);
	digitalLo(GPIOA, Pin_6);
	digitalLo(GPIOA, Pin_7);	
	
	delay(100);
	
	digitalHi(GPIOA, Pin_0);
	digitalHi(GPIOA, Pin_1);
	digitalHi(GPIOA, Pin_2);
	digitalHi(GPIOA, Pin_3);

	digitalHi(GPIOA, Pin_4);
	digitalHi(GPIOA, Pin_5);
	digitalHi(GPIOA, Pin_6);
	digitalHi(GPIOA, Pin_7);		
	
	return TRUE;
}


int main()
{
	if (!setup())
		return 0;
	
	while (1)
	{
		char data;
		
//		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }

		
		//while (!(USART1->SR & USART_FLAG_RXNE));
//    if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)		
		{
			data = receiveData2();
			if (data == 'S')
			{
				char buff[4];
//				buff[ 0] = receiveData2();
//				buff[ 1] = receiveData2();
//				buff[ 2] = receiveData2();
//				buff[ 3] = receiveData2();
				

				digitalHi(GPIOA, Pin_0);
				digitalHi(GPIOA, Pin_1);
				digitalHi(GPIOA, Pin_2);
				digitalHi(GPIOA, Pin_3);

				digitalHi(GPIOA, Pin_4);
				digitalHi(GPIOA, Pin_5);
				digitalHi(GPIOA, Pin_6);
				digitalHi(GPIOA, Pin_7);			

				if (buff[0] <= -100)
				{
					digitalLo(GPIOA, Pin_1);
					digitalLo(GPIOA, Pin_4);				
				}
				if (buff[0] >= 100)
				{
					digitalLo(GPIOA, Pin_3);
					digitalLo(GPIOA, Pin_6);
				}
				
				if (buff[1] <= -100)
				{
					digitalLo(GPIOA, Pin_2);
				}
				if (buff[1] >= 100)
				{
					digitalLo(GPIOA, Pin_5);
				}
				
			
				
				if (buff[2] <= -100)
				{
					digitalLo(GPIOA, Pin_1);
					digitalLo(GPIOA, Pin_4);				
				}
				if (buff[2] >= 100)
				{
					digitalLo(GPIOA, Pin_3);
					digitalLo(GPIOA, Pin_6);
				}
				
				if (buff[3] <= -100)
				{
					digitalLo(GPIOA, Pin_2);
				}
				if (buff[3] >= 100)
				{
					digitalLo(GPIOA, Pin_5);
				}

				
				LED0_ON;
			}
		}

 	  delay(10);		
		LED0_OFF;
		//LED1_TOGGLE;
		
		//print_byte('A');
		
		//delay(50);
	}		
	
}


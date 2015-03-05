
#include "global.h"


static volatile uint32_t usTicks = 0;
static volatile uint32_t sysTickUptime = 0;


static void cycleCounterInit(void)
{
    RCC_ClocksTypeDef clocks;
    RCC_GetClocksFreq(&clocks);
    usTicks = clocks.SYSCLK_Frequency / 1000000;
}

// SysTick
void SysTick_Handler(void)
{
    sysTickUptime++;
}



void systemInit(void)
{
/*	
    struct {
        GPIO_TypeDef *gpio;
        gpio_config_t cfg;
    } gpio_setup[] = {
#ifdef LED0
        {
            .gpio = LED0_GPIO,
            .cfg = { LED0_PIN, Mode_Out_PP, Speed_2MHz }
        },
#endif
#ifdef LED1

        {
            .gpio = LED1_GPIO,
            .cfg = { LED1_PIN, Mode_Out_PP, Speed_2MHz }
        }
#endif
    };
	*/
    gpio_config_t gpio;
    //int i, gpio_count = sizeof(gpio_setup) / sizeof(gpio_setup[0]);
		

    // Configure NVIC preempt/priority groups
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // Turn on clocks for stuff we use
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_TIM1 | RCC_APB2Periph_ADC1 | RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_ClearFlag();

    // Make all GPIO in by default to save power and reduce noise
    gpio.pin = Pin_All;
    gpio.mode = Mode_AIN;
    gpioInit(GPIOA, &gpio);
    gpioInit(GPIOB, &gpio);
    gpioInit(GPIOC, &gpio);

    // Turn off JTAG port 'cause we're using the GPIO for leds
#define AFIO_MAPR_SWJ_CFG_NO_JTAG_SW            (0x2 << 24)
    AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_NO_JTAG_SW;

//    LED0_OFF;
//    LED1_OFF;

    // Hack - rev4 and below used opendrain to PNP for buzzer. Rev5 and above use PP to NPN.
//    for (i = 0; i < gpio_count; i++) {
 //       if (hw_revision >= NAZE32_REV5 && gpio_setup[i].cfg.mode == Mode_Out_OD)
 //           gpio_setup[i].cfg.mode = Mode_Out_PP;
//        gpioInit(gpio_setup[i].gpio, &gpio_setup[i].cfg);
//    }

    // Init cycle counter
    cycleCounterInit();

    // SysTick
    SysTick_Config(SystemCoreClock / 1000);
}



// Return system uptime in microseconds (rollover in 70minutes)
uint32_t micros(void)
{
    register uint32_t ms, cycle_cnt;
    do {
        ms = sysTickUptime;
        cycle_cnt = SysTick->VAL;
    } while (ms != sysTickUptime);
    return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}


// Return system uptime in milliseconds (rollover in 49 days)
uint32_t millis(void)
{
    return sysTickUptime;
}


void delayMicroseconds(uint32_t us)
{
    uint32_t now = micros();
    while (micros() - now < us);
}


void delay(uint32_t ms)
{
    while (ms--)
        delayMicroseconds(1000);
}



#include "global.h"

// Baseflight �ҽ��� �̿��ؼ� PWM ��ȣ�� ����ϴ� ������.
// 
// USART1 ��Ʈ =  PA10,9 (Rx,Tx)
// PA8 �� PWM ��ȣ�� ���´�.
// Servo ���͸� -90 ~ +90 �� ���̸� ������ �Ѵ�.
//


TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1_Val = 333;
uint16_t CCR2_Val = 249;
uint16_t CCR3_Val = 166;
uint16_t CCR4_Val = 83;
uint16_t PrescalerValue = 0;
int16_t failsafeCnt = 0;



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
//	 GPIO_InitTypeDef GPIO_InitStructure;
	drv_pwm_config_t pwm_params;
	int servoOut = 700;

	
	// Ŭ��, Ÿ�̸� �ʱ�ȭ
	systemInit();
	
	// �ø��� ��� �ʱ�ȭ.
	// USART1 ��Ʈ =  PA10,9 (Rx,Tx)
	serialInit(9600);
	
	
	
    pwm_params.airplane = false;
    pwm_params.useUART = false;
    pwm_params.useSoftSerial = false;
    pwm_params.usePPM = false;
    pwm_params.enableInput = false;
    pwm_params.useServos = true;
    pwm_params.extraServos = false;
    pwm_params.motorPwmRate = 400;
    pwm_params.servoPwmRate = 50;
    pwm_params.pwmFilter = NULL;
    pwm_params.idlePulse = PULSE_1MS; // standard PWM for brushless ESC (default, overridden below)
    if (pwm_params.motorPwmRate > 500)
        pwm_params.idlePulse = 0; // brushed motors
    pwm_params.syncPWM = false;
    pwm_params.fastPWM = false;
    pwm_params.servoCenterPulse = 1500;
    pwm_params.failsafeThreshold = 985;
    pwm_params.adcChannel = 0;
	
	pwmInit( &pwm_params );
	
	
	// PA8 �� PWM ��ȣ�� ���´�.
	pwmWriteServo(0, servoOut);
	
	
	// ����
	while (1)
	{
		// 700 ~ 2300  ���̸� ������ PWM��ȣ�� ������.
		// -90 ~ +90 ���̸� ȸ���Ѵ�.
		servoOut += 10;		
		pwmWriteServo(0, servoOut);		
		if (servoOut > 2300)
			servoOut = 700;
		
		
		// �ø��� ��� ó��.
		//serialCom();		
		//printf( "test\n" );		

		
		delay(10);
	}
		
}


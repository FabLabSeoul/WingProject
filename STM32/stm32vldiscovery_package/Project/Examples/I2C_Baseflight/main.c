//
// base flight i2c ���̺귯���� �̿��ؼ� hmc5883 ������ ����Ѵ�.
//
//

#include "global.h"
#include "stm32f10x.h"
#include "STM32vldiscovery.h"


sensor_t mag;                       // mag access functions
//static uint8_t magInit = 0;

void Mag_init(void)
{
    // initialize and calibration. turn on led during mag calibration (calibration routine blinks it)
    LED1_ON;
    mag.init(ALIGN_DEFAULT);
    LED1_OFF;
    //magInit = 1;
}


void setup(void)
{
  gpio_config_t gpio;
	
	// Ŭ���� �ʱ�ȭ�ϰ�, delay, tick ����� ����� �� �ְ� �ʱ�ȭ�Ѵ�.
	systemInit();
	
	// PA10,9(Rx,Tx)��Ʈ�� �ø�����Ʈ�� �ʱ�ȭ �Ѵ�.
	SerialSetup();
	
	// printf()�Լ��� ����� �� �ְ� �ʱ�ȭ�Ѵ�.
	init_printf(NULL, putc);
	printf( "Init baseflight i2c\n" );
	
	// Stm32 ���忡 �ִ� LED3,4�� Ȱ��ȭ �Ѵ�.
	gpio.pin = Pin_All;
	gpio.speed = Speed_2MHz;
	gpio.mode = Mode_Out_PP;
	gpioInit(GPIOC, &gpio);
	
	// HMC5883L�� ����� i2c�� �ʱ�ȭ �Ѵ�.
	i2cInit(I2CDEV_1, 0x00); // Init Master i2c 
	printf( "init i2c\n");
}


int main()
{

	setup();
	
	
  // HMC5883L�� ����Ǿ����� Ȯ��.
	if (!hmc5883lDetect(&mag))
	{
		return 1;
	}
	
	
  // HMC5883L ���ڱ⼾�� �ʱ�ȭ
	Mag_init();
	printf( "Init Magnetometer\n" );
	
	
	while(1)
	{
		int16_t Mag[3];
		
		// ���ڱ� ���������� i2c����� ���� �о�´�.
		mag.read(Mag);

		
		print_byte('S');
		print_short( Mag[ 0] );
		print_short( Mag[ 1] );
		print_short( Mag[ 2] );
		
		delayMicroseconds(10000);		
	}
}


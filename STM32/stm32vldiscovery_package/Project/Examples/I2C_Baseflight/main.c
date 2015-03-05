//
// base flight i2c 라이브러리를 이용해서 hmc5883 센서와 통신한다.
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
	
	systemInit();
	
	SerialSetup();
	init_printf(NULL, putc);
	printf( "Init baseflight i2c\n" );
	
	gpio.pin = Pin_All;
	gpio.speed = Speed_2MHz;
	gpio.mode = Mode_Out_PP;
	gpioInit(GPIOC, &gpio);
  //STM32vldiscovery_LEDInit(LED3);
  //STM32vldiscovery_LEDInit(LED4);
	
	i2cInit(I2CDEV_1, 0x00); // Init Master i2c 
	printf( "init i2c\n");
}


int main()
{

	setup();
	
	
	if (!hmc5883lDetect(&mag))
	{
		return 1;
	}
	
	Mag_init();
	printf( "Init Magnetometer\n" );
	
	
	while(1)
	{
		int16_t Mag[3];
		mag.read(Mag);

		
		print_byte('S');
		print_short( Mag[ 0] );
		print_short( Mag[ 1] );
		print_short( Mag[ 2] );
		
		delayMicroseconds(10000);		
	}
}


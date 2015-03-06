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
	
	// 클럭을 초기화하고, delay, tick 기능이 수행될 수 있게 초기화한다.
	systemInit();
	
	// PA10,9(Rx,Tx)포트를 시리얼포트로 초기화 한다.
	SerialSetup();
	
	// printf()함수를 사용할 수 있게 초기화한다.
	init_printf(NULL, putc);
	printf( "Init baseflight i2c\n" );
	
	// Stm32 보드에 있는 LED3,4를 활성화 한다.
	gpio.pin = Pin_All;
	gpio.speed = Speed_2MHz;
	gpio.mode = Mode_Out_PP;
	gpioInit(GPIOC, &gpio);
	
	// HMC5883L과 통신할 i2c를 초기화 한다.
	i2cInit(I2CDEV_1, 0x00); // Init Master i2c 
	printf( "init i2c\n");
}


int main()
{

	setup();
	
	
  // HMC5883L과 연결되었는지 확인.
	if (!hmc5883lDetect(&mag))
	{
		return 1;
	}
	
	
  // HMC5883L 지자기센서 초기화
	Mag_init();
	printf( "Init Magnetometer\n" );
	
	
	while(1)
	{
		int16_t Mag[3];
		
		// 지자기 센서정보를 i2c통신을 통해 읽어온다.
		mag.read(Mag);

		
		print_byte('S');
		print_short( Mag[ 0] );
		print_short( Mag[ 1] );
		print_short( Mag[ 2] );
		
		delayMicroseconds(10000);		
	}
}


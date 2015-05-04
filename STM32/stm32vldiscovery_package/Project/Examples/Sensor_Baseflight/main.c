//
//
// baseflight 의 Sensor 라이브러리를 이용해 자이로+가속도+지자기 센서를 테스트 한다.
//
// I2C1 -> PB6,7 (SCL, SDA)
// USART1 -> PA10,9 (Rx,Tx)
//
//

#include "global.h"
#include "system_stm32f10x.h"


// Automatic ACC Offset Calibration
bool AccInflightCalibrationArmed = false;
bool AccInflightCalibrationMeasurementDone = false;
bool AccInflightCalibrationSavetoEEProm = false;
bool AccInflightCalibrationActive = false;
uint16_t InflightcalibratingA = 0;

// Battery monitoring stuff
uint8_t batteryCellCount = 3;       // cell count
uint16_t batteryWarningVoltage;     // slow buzzer after this one, recommended 80% of battery used. Time to land.
uint16_t batteryCriticalVoltage;    // annoying buzzer after this one, battery is going to be dead.


core_t core;
config_t cfg;
master_t mcfg;  // master config struct with data independent from profiles
flags_t f;
uint32_t currentTime = 0;


void SetSysClock(bool overclock);



void setup()
{
	gpio_config_t gpio;
	bool sensorsOK = false;

	mcfg.emf_avoidance = 0;
	mcfg.gyro_cmpf_factor = 600;    // default MWC	
//	mcfg.gyro_cmpf_factor = 25;    // default MWC	
	mcfg.gyro_cmpfm_factor = 250;   // default MWC
	mcfg.gyro_lpf = 42;             // supported by all gyro drivers now. In case of ST gyro, will default to 32Hz instead
	mcfg.gyro_align = ALIGN_DEFAULT;
	mcfg.acc_align = ALIGN_DEFAULT;
	mcfg.mag_align = ALIGN_DEFAULT;
	mcfg.vbatscale = 110;
	mcfg.currentscale = 400; // for Allegro ACS758LCB-100U (40mV/A)
	mcfg.moron_threshold = 32;


	cfg.mag_declination = 0;    // For example, -6deg 37min, = -637 Japan, format is [sign]dddmm (degreesminutes) default is zero.
	cfg.small_angle = 25;
	cfg.throttle_correction_value = 0;      // could 10 with althold or 40 for fpv
	cfg.throttle_correction_angle = 800;    // could be 80.0 deg with atlhold or 45.0 for fpv
	cfg.accz_lpf_cutoff = 5.0f;
	cfg.acc_lpf_factor = 4;
	cfg.accz_deadband = 40;
	cfg.accxy_deadband = 40;
	
	// RCC 초기화.
	SystemInit();
	 
	 // Configure clock, this figures out HSE for hardware autodetect
    //SetSysClock(mcfg.emf_avoidance);
	
	
	//Baseflight 클럭, 타이머 초기화
	systemInit();


	// sleep for 100ms
	delay(100);


	// Stm32 보드에 있는 LED3,4를 활성화 한다.
	gpio.pin = Pin_All;
	gpio.speed = Speed_2MHz;
	gpio.mode = Mode_Out_PP;
	gpioInit(GPIOC, &gpio);


	// MPU-6050, HMC5883L과 통신할 i2c를 초기화 한다.
	i2cInit(I2CDEV_1, 0x00); // Init Master i2c 


	// MPU-6050, HMC6553L 센서 체크, 
	// 센서 초기화.
	sensorsOK = sensorsAutodetect();
	
	// if gyro was not detected due to whatever reason, we give up now.
	if (!sensorsOK)
			failureMode(3);

	// IMU 초기화
	// HMC6553L 지자기 센서 초기화
	imuInit();

	// 시리얼 통신 초기화.
	// PA10,9 (Rx,Tx)
	serialInit(115200);
	

}



int main()
{
	setup();
	
	
	// 루프
	while (1)
	{
		uint32_t curT = millis();
		static uint32_t oldT = 0;
		uint32_t elapseT = curT - oldT;
		
		currentTime = micros();
		
		// 시리얼 통신 처리.
		serialCom();
		
		Mag_getADC();
		
		// 센서 처리
		computeIMU();
				
		//printf( "Sensor,%d, %d\n", angle[ ROLL], angle[PITCH]);
		
		if (elapseT > 30)
		{
			printf( "Radian, %d, %f, %f, %f \n", millis(), anglerad[0], anglerad[1], (float)heading * RAD);
			
			LED0_TOGGLE;
			oldT = curT;
		}		
		
		
	}
		
}



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


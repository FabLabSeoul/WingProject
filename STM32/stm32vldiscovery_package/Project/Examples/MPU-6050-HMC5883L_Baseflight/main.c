
#include "global.h"
//#include "stm32f10x.h"
//#include "STM32vldiscovery.h"


sensor_t acc; // acc access functions
sensor_t gyro; // gyro access functions
sensor_t mag; // mag access functions
uint8_t mpu6050_scale;


//센서에서 바로 읽어온 값을 저장할 변수
float    base_x_accel;
float    base_y_accel;
float    base_z_accel;

float    base_x_gyro;
float    base_y_gyro;
float    base_z_gyro;


//변경된 각도와 최종 각도를 저장할 변수
uint32_t last_read_time;
float         last_x_angle;  
float         last_y_angle;
float         last_z_angle;  
float         last_gyro_x_angle;  
float         last_gyro_y_angle;
float         last_gyro_z_angle;

uint32_t get_last_time(void) {return last_read_time;}
void calibrate_sensors(void);
float get_last_x_angle(void) {return last_x_angle;}
float get_last_y_angle(void) {return last_y_angle;}
float get_last_z_angle(void) {return last_z_angle;}
float get_last_gyro_x_angle(void) {return last_gyro_x_angle;}
float get_last_gyro_y_angle(void) {return last_gyro_y_angle;}
float get_last_gyro_z_angle(void) {return last_gyro_z_angle;}


//각도를 바꾸는 함수
void set_last_read_angle_data(uint32_t time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) 
{
  last_read_time = time;
  last_x_angle = x;
  last_y_angle = y;
  last_z_angle = z;
  last_gyro_x_angle = x_gyro;
  last_gyro_y_angle = y_gyro;
  last_gyro_z_angle = z_gyro;
}

void SendSerialAccelGryro( int16_t accelgyro[6] );



// 각종 초기화
bool setup()
{
  gpio_config_t gpio;
	
	// 클럭을 초기화하고, delay, tick 기능이 수행될 수 있게 초기화한다.
	systemInit();
	
	// PA10,9(Rx,Tx)포트를 시리얼포트로 초기화 한다.
	SerialSetup();
	
	// printf()함수를 사용할 수 있게 초기화한다.
	init_printf(NULL, putc);

	
	// Stm32 보드에 있는 LED3,4를 활성화 한다.
	gpio.pin = Pin_All;
	gpio.speed = Speed_2MHz;
	gpio.mode = Mode_Out_PP;
	gpioInit(GPIOC, &gpio);
	
	// HMC5883L과 통신할 i2c를 초기화 한다.
	i2cInit(I2CDEV_1, 0x00); // Init Master i2c 
	printf( "init i2c\n");
	
	
	// MPU-6050 초기화
	if (!mpu6050Detect(&acc, &gyro, 42, &mpu6050_scale)) // yes, i'm rerunning it again.  re-fill acc struct	
		return false;
	
	acc.init(ALIGN_DEFAULT);
	gyro.init(ALIGN_DEFAULT);
	
	
  // HMC5883L과 연결되었는지 확인.
	if (!hmc5883lDetect(&mag))
		return false;
	
  // HMC5883L 지자기센서 초기화
	LED1_ON;
	mag.init(ALIGN_DEFAULT);
	LED1_OFF;	
	printf( "Init Magnetometer\n" );
	
	return true;
}


int main()
{
		if (!setup())
			return 0;

	calibrate_sensors();
	set_last_read_angle_data(millis(), 0, 0, 0, 0, 0, 0);
		

	while(1)
	{
		int16_t accelgyro[6];	
		int16_t Mag[3];
		
		// Accel, Gyro
		acc.read(accelgyro);
		gyro.read(accelgyro+3);		
		
		// 지자기 센서정보를 i2c통신을 통해 읽어온다.
		mag.read(Mag);

		
		SendSerialAccelGryro(accelgyro);
		print_short( Mag[ 0] );
		print_short( Mag[ 1] );
		print_short( Mag[ 2] );

		
		delay(10);
	}
}


//
// 소스 참조. 아두이노 소스를 stm32 소스로 바꿨다.
// http://m.kocoafab.cc/tutorial/view/239
//
void SendSerialAccelGryro( int16_t accelgyro[6] )
{
	//  250 degree/s 범위를 가지고, 이 값을 16 bit 분해능으로 표현하고 있으므로, 
	// mpu-6050에서 보내는 값은 -32766 ~ +32766 값이다.
	// 그러므로 이 값을 실제 250 degree/s 로 변환하려면 131로 나눠줘야 한다. 범위가
	// 다르면 이 값도 같이 바껴야한다.
	float FS_SEL = 131;
	
	  //회전을 했을 떄 시간 알기
  unsigned long t_now = millis();

	
  float gyro_x = (accelgyro[3] - base_x_gyro)/FS_SEL;
  float gyro_y = (accelgyro[4] - base_y_gyro)/FS_SEL;
  float gyro_z = (accelgyro[5] - base_z_gyro)/FS_SEL;

	  
// 가속도 값 범위는?
// 16bit 니 -32766 ~ +32766 범위이고,
//	 +-2g 범위라면 mpu-6050으로부터 넘어온 값을 실제 g단위로 환산하려면
//	  scale factor(16384)로 나눠줘야 한다. +-2g 범위는 16,384값이다. 즉 32766값이면 2가 된다.	
	
  //acceleration 원시 데이터 저장
  float accel_x = accelgyro[0];
  float accel_y = accelgyro[1];
  float accel_z = accelgyro[2];
	
	 
  //accelerometer로 부터 각도 얻기
  float RADIANS_TO_DEGREES = 180/3.14159;

  // float accel_vector_length = sqrt(pow(accel_x,2) + pow(accel_y,2) + pow(accel_z,2));
  float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
  float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
  float accel_angle_z = 0;
	

  //gyro angles 계산1
  float dt =(t_now - get_last_time())/1000.0;
  float gyro_angle_x = gyro_x*dt + get_last_x_angle();
  float gyro_angle_y = gyro_y*dt + get_last_y_angle();
  float gyro_angle_z = gyro_z*dt + get_last_z_angle();
  
  //gyro angles 계산2
  float unfiltered_gyro_angle_x = gyro_x*dt + get_last_gyro_x_angle();
  float unfiltered_gyro_angle_y = gyro_y*dt + get_last_gyro_y_angle();
  float unfiltered_gyro_angle_z = gyro_z*dt + get_last_gyro_z_angle();
  
  //알파를 이용해서 최종 각도 계산3
  float alpha = 0.96;
  float angle_x = alpha*gyro_angle_x + (1.0 - alpha)*accel_angle_x;
  float angle_y = alpha*gyro_angle_y + (1.0 - alpha)*accel_angle_y;
  float angle_z = gyro_angle_z;  //Accelerometer는 z-angle 없음	
	
	
 //최종 각도 저장
  set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
 	
	// 시작 문자 전송
	print_byte('S');	
	
	print_byte( (int16_t)dt );
	
	// 2 바이트 정수로 보내기 위해 100을 곱하고, 받을 때, 100을 나눠준다. 
	print_short( (int16_t)(accel_angle_x*100) );
	print_short( (int16_t)(accel_angle_y*100) );
	print_short( (int16_t)(accel_angle_z*100) );

	print_short( (int16_t)(unfiltered_gyro_angle_x*100) );
	print_short( (int16_t)(unfiltered_gyro_angle_y*100) );
	print_short( (int16_t)(unfiltered_gyro_angle_z*100) );
	
	print_short( (int16_t)(angle_x*100) );
	print_short( (int16_t)(angle_y*100) );
	print_short( (int16_t)(angle_z*100) );
}



//움직임이 없을 때 일차적으로 값을 읽어와 전역변수에 저장
void calibrate_sensors() 
{
	int i = 0;
  int                   num_readings = 10;
  float                 x_accel = 0;
  float                 y_accel = 0;
  float                 z_accel = 0;
  float                 x_gyro = 0;
  float                 y_gyro = 0;
  float                 z_gyro = 0;
	int16_t  AccelGyro[6]={0};
	
   
  //원시 데이터들의 평균 읽기
  for (i = 0; i < num_readings; i++) 
	{
		//MPU6050_GetRawAccelGyro(AccelGyro);
		acc.read(AccelGyro);
		gyro.read(AccelGyro+3);

    
    x_accel += AccelGyro[0];
    y_accel += AccelGyro[1];
    z_accel += AccelGyro[2];
    x_gyro += AccelGyro[3];
    y_gyro += AccelGyro[4];
    z_gyro += AccelGyro[5];
		
    delay(10);
  }
	
  x_accel /= num_readings;
  y_accel /= num_readings;
  z_accel /= num_readings;
  x_gyro /= num_readings;
  y_gyro /= num_readings;
  z_gyro /= num_readings;
  
  //전역 변수에 저장
  base_x_accel = x_accel;
  base_y_accel = y_accel;
  base_z_accel = z_accel;
  base_x_gyro = x_gyro;
  base_y_gyro = y_gyro;
  base_z_gyro = z_gyro;
}


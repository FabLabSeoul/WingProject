
#include "global.h"
//#include "stm32f10x.h"
//#include "STM32vldiscovery.h"


sensor_t acc; // acc access functions
sensor_t gyro; // gyro access functions
sensor_t mag; // mag access functions
uint8_t mpu6050_scale;


//�������� �ٷ� �о�� ���� ������ ����
float    base_x_accel;
float    base_y_accel;
float    base_z_accel;

float    base_x_gyro;
float    base_y_gyro;
float    base_z_gyro;


//����� ������ ���� ������ ������ ����
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


//������ �ٲٴ� �Լ�
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



// ���� �ʱ�ȭ
bool setup()
{
  gpio_config_t gpio;
	
	// Ŭ���� �ʱ�ȭ�ϰ�, delay, tick ����� ����� �� �ְ� �ʱ�ȭ�Ѵ�.
	systemInit();
	
	// PA10,9(Rx,Tx)��Ʈ�� �ø�����Ʈ�� �ʱ�ȭ �Ѵ�.
	SerialSetup();
	
	// printf()�Լ��� ����� �� �ְ� �ʱ�ȭ�Ѵ�.
	init_printf(NULL, putc);

	
	// Stm32 ���忡 �ִ� LED3,4�� Ȱ��ȭ �Ѵ�.
	gpio.pin = Pin_All;
	gpio.speed = Speed_2MHz;
	gpio.mode = Mode_Out_PP;
	gpioInit(GPIOC, &gpio);
	
	// HMC5883L�� ����� i2c�� �ʱ�ȭ �Ѵ�.
	i2cInit(I2CDEV_1, 0x00); // Init Master i2c 
	printf( "init i2c\n");
	
	
	// MPU-6050 �ʱ�ȭ
	if (!mpu6050Detect(&acc, &gyro, 42, &mpu6050_scale)) // yes, i'm rerunning it again.  re-fill acc struct	
		return false;
	
	acc.init(ALIGN_DEFAULT);
	gyro.init(ALIGN_DEFAULT);
	
	
  // HMC5883L�� ����Ǿ����� Ȯ��.
	if (!hmc5883lDetect(&mag))
		return false;
	
  // HMC5883L ���ڱ⼾�� �ʱ�ȭ
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
		
		// ���ڱ� ���������� i2c����� ���� �о�´�.
		mag.read(Mag);

		
		SendSerialAccelGryro(accelgyro);
		print_short( Mag[ 0] );
		print_short( Mag[ 1] );
		print_short( Mag[ 2] );

		
		delay(10);
	}
}


//
// �ҽ� ����. �Ƶ��̳� �ҽ��� stm32 �ҽ��� �ٲ��.
// http://m.kocoafab.cc/tutorial/view/239
//
void SendSerialAccelGryro( int16_t accelgyro[6] )
{
	//  250 degree/s ������ ������, �� ���� 16 bit ���ش����� ǥ���ϰ� �����Ƿ�, 
	// mpu-6050���� ������ ���� -32766 ~ +32766 ���̴�.
	// �׷��Ƿ� �� ���� ���� 250 degree/s �� ��ȯ�Ϸ��� 131�� ������� �Ѵ�. ������
	// �ٸ��� �� ���� ���� �ٲ����Ѵ�.
	float FS_SEL = 131;
	
	  //ȸ���� ���� �� �ð� �˱�
  unsigned long t_now = millis();

	
  float gyro_x = (accelgyro[3] - base_x_gyro)/FS_SEL;
  float gyro_y = (accelgyro[4] - base_y_gyro)/FS_SEL;
  float gyro_z = (accelgyro[5] - base_z_gyro)/FS_SEL;

	  
// ���ӵ� �� ������?
// 16bit �� -32766 ~ +32766 �����̰�,
//	 +-2g ������� mpu-6050���κ��� �Ѿ�� ���� ���� g������ ȯ���Ϸ���
//	  scale factor(16384)�� ������� �Ѵ�. +-2g ������ 16,384���̴�. �� 32766���̸� 2�� �ȴ�.	
	
  //acceleration ���� ������ ����
  float accel_x = accelgyro[0];
  float accel_y = accelgyro[1];
  float accel_z = accelgyro[2];
	
	 
  //accelerometer�� ���� ���� ���
  float RADIANS_TO_DEGREES = 180/3.14159;

  // float accel_vector_length = sqrt(pow(accel_x,2) + pow(accel_y,2) + pow(accel_z,2));
  float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
  float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
  float accel_angle_z = 0;
	

  //gyro angles ���1
  float dt =(t_now - get_last_time())/1000.0;
  float gyro_angle_x = gyro_x*dt + get_last_x_angle();
  float gyro_angle_y = gyro_y*dt + get_last_y_angle();
  float gyro_angle_z = gyro_z*dt + get_last_z_angle();
  
  //gyro angles ���2
  float unfiltered_gyro_angle_x = gyro_x*dt + get_last_gyro_x_angle();
  float unfiltered_gyro_angle_y = gyro_y*dt + get_last_gyro_y_angle();
  float unfiltered_gyro_angle_z = gyro_z*dt + get_last_gyro_z_angle();
  
  //���ĸ� �̿��ؼ� ���� ���� ���3
  float alpha = 0.96;
  float angle_x = alpha*gyro_angle_x + (1.0 - alpha)*accel_angle_x;
  float angle_y = alpha*gyro_angle_y + (1.0 - alpha)*accel_angle_y;
  float angle_z = gyro_angle_z;  //Accelerometer�� z-angle ����	
	
	
 //���� ���� ����
  set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
 	
	// ���� ���� ����
	print_byte('S');	
	
	print_byte( (int16_t)dt );
	
	// 2 ����Ʈ ������ ������ ���� 100�� ���ϰ�, ���� ��, 100�� �����ش�. 
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



//�������� ���� �� ���������� ���� �о�� ���������� ����
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
	
   
  //���� �����͵��� ��� �б�
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
  
  //���� ������ ����
  base_x_accel = x_accel;
  base_y_accel = y_accel;
  base_z_accel = z_accel;
  base_x_gyro = x_gyro;
  base_y_gyro = y_gyro;
  base_z_gyro = z_gyro;
}


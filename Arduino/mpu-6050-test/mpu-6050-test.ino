/*GY-521 모듈 사용하기 
  본 스케치는 arduino.cc의 Krodal님의 스케치를 바탕으로 작성되었습니다.
*/

#include <Wire.h>    //아두이노 기본 라이브러리 Wire를 포함해야 합니다.

// MPU-6050 레지스터 map
#define MPU6050_AUX_VDDIO          0x01   
#define MPU6050_SMPLRT_DIV         0x19   
#define MPU6050_CONFIG             0x1A   
#define MPU6050_GYRO_CONFIG        0x1B   
#define MPU6050_ACCEL_CONFIG       0x1C   
#define MPU6050_FF_THR             0x1D   
#define MPU6050_FF_DUR             0x1E  
#define MPU6050_MOT_THR            0x1F 
#define MPU6050_MOT_DUR            0x20 
#define MPU6050_ZRMOT_THR          0x21
#define MPU6050_ZRMOT_DUR          0x22
#define MPU6050_FIFO_EN            0x23   
#define MPU6050_I2C_MST_CTRL       0x24  
#define MPU6050_I2C_SLV0_ADDR      0x25 
#define MPU6050_I2C_SLV0_REG       0x26 
#define MPU6050_I2C_SLV0_CTRL      0x27   
#define MPU6050_I2C_SLV1_ADDR      0x28   
#define MPU6050_I2C_SLV1_REG       0x29   
#define MPU6050_I2C_SLV1_CTRL      0x2A   
#define MPU6050_I2C_SLV2_ADDR      0x2B   
#define MPU6050_I2C_SLV2_REG       0x2C
#define MPU6050_I2C_SLV2_CTRL      0x2D   
#define MPU6050_I2C_SLV3_ADDR      0x2E   
#define MPU6050_I2C_SLV3_REG       0x2F   
#define MPU6050_I2C_SLV3_CTRL      0x30   
#define MPU6050_I2C_SLV4_ADDR      0x31   
#define MPU6050_I2C_SLV4_REG       0x32  
#define MPU6050_I2C_SLV4_DO        0x33  
#define MPU6050_I2C_SLV4_CTRL      0x34  
#define MPU6050_I2C_SLV4_DI        0x35   
#define MPU6050_I2C_MST_STATUS     0x36   
#define MPU6050_INT_PIN_CFG        0x37   
#define MPU6050_INT_ENABLE         0x38  
#define MPU6050_INT_STATUS         0x3A  
#define MPU6050_ACCEL_XOUT_H       0x3B  
#define MPU6050_ACCEL_XOUT_L       0x3C   
#define MPU6050_ACCEL_YOUT_H       0x3D     
#define MPU6050_ACCEL_YOUT_L       0x3E   
#define MPU6050_ACCEL_ZOUT_H       0x3F   
#define MPU6050_ACCEL_ZOUT_L       0x40   
#define MPU6050_TEMP_OUT_H         0x41  
#define MPU6050_TEMP_OUT_L         0x42    
#define MPU6050_GYRO_XOUT_H        0x43    
#define MPU6050_GYRO_XOUT_L        0x44   
#define MPU6050_GYRO_YOUT_H        0x45    
#define MPU6050_GYRO_YOUT_L        0x46   
#define MPU6050_GYRO_ZOUT_H        0x47   
#define MPU6050_GYRO_ZOUT_L        0x48   
#define MPU6050_EXT_SENS_DATA_00   0x49     
#define MPU6050_EXT_SENS_DATA_01   0x4A    
#define MPU6050_EXT_SENS_DATA_02   0x4B   
#define MPU6050_EXT_SENS_DATA_03   0x4C    
#define MPU6050_EXT_SENS_DATA_04   0x4D   
#define MPU6050_EXT_SENS_DATA_05   0x4E    
#define MPU6050_EXT_SENS_DATA_06   0x4F    
#define MPU6050_EXT_SENS_DATA_07   0x50    
#define MPU6050_EXT_SENS_DATA_08   0x51   
#define MPU6050_EXT_SENS_DATA_09   0x52  
#define MPU6050_EXT_SENS_DATA_10   0x53     
#define MPU6050_EXT_SENS_DATA_11   0x54    
#define MPU6050_EXT_SENS_DATA_12   0x55  
#define MPU6050_EXT_SENS_DATA_13   0x56 
#define MPU6050_EXT_SENS_DATA_14   0x57   
#define MPU6050_EXT_SENS_DATA_15   0x58  
#define MPU6050_EXT_SENS_DATA_16   0x59    
#define MPU6050_EXT_SENS_DATA_17   0x5A  
#define MPU6050_EXT_SENS_DATA_18   0x5B    
#define MPU6050_EXT_SENS_DATA_19   0x5C  
#define MPU6050_EXT_SENS_DATA_20   0x5D    
#define MPU6050_EXT_SENS_DATA_21   0x5E   
#define MPU6050_EXT_SENS_DATA_22   0x5F     
#define MPU6050_EXT_SENS_DATA_23   0x60  
#define MPU6050_MOT_DETECT_STATUS  0x61   
#define MPU6050_I2C_SLV0_DO        0x63  
#define MPU6050_I2C_SLV1_DO        0x64  
#define MPU6050_I2C_SLV2_DO        0x65  
#define MPU6050_I2C_SLV3_DO        0x66 
#define MPU6050_I2C_MST_DELAY_CTRL 0x67 
#define MPU6050_SIGNAL_PATH_RESET  0x68 
#define MPU6050_MOT_DETECT_CTRL    0x69  
#define MPU6050_USER_CTRL          0x6A  
#define MPU6050_PWR_MGMT_1         0x6B   
#define MPU6050_PWR_MGMT_2         0x6C  
#define MPU6050_FIFO_COUNTH        0x72   
#define MPU6050_FIFO_COUNTL        0x73   
#define MPU6050_FIFO_R_W           0x74  
#define MPU6050_WHO_AM_I           0x75  


// 센서 사용시 사용할 변수
#define MPU6050_D0 0
#define MPU6050_D1 1
#define MPU6050_D2 2
#define MPU6050_D3 3
#define MPU6050_D4 4
#define MPU6050_D5 5
#define MPU6050_D6 6
#define MPU6050_D7 7

// AUX_VDDIO Register
#define MPU6050_AUX_VDDIO MPU6050_D7  // I2C high: 1=VDD, 0=VLOGIC

// CONFIG Register
// DLPF은 Digital Low Pass Filter
#define MPU6050_DLPF_CFG0     MPU6050_D0
#define MPU6050_DLPF_CFG1     MPU6050_D1
#define MPU6050_DLPF_CFG2     MPU6050_D2
#define MPU6050_EXT_SYNC_SET0 MPU6050_D3
#define MPU6050_EXT_SYNC_SET1 MPU6050_D4
#define MPU6050_EXT_SYNC_SET2 MPU6050_D5

//EXT_SYNC_SET 변수 
#define MPU6050_EXT_SYNC_SET_0 (0)
#define MPU6050_EXT_SYNC_SET_1 (bit(MPU6050_EXT_SYNC_SET0))
#define MPU6050_EXT_SYNC_SET_2 (bit(MPU6050_EXT_SYNC_SET1))
#define MPU6050_EXT_SYNC_SET_3 (bit(MPU6050_EXT_SYNC_SET1)|bit(MPU6050_EXT_SYNC_SET0))
#define MPU6050_EXT_SYNC_SET_4 (bit(MPU6050_EXT_SYNC_SET2))
#define MPU6050_EXT_SYNC_SET_5 (bit(MPU6050_EXT_SYNC_SET2)|bit(MPU6050_EXT_SYNC_SET0))
#define MPU6050_EXT_SYNC_SET_6 (bit(MPU6050_EXT_SYNC_SET2)|bit(MPU6050_EXT_SYNC_SET1))
#define MPU6050_EXT_SYNC_SET_7 (bit(MPU6050_EXT_SYNC_SET2)|bit(MPU6050_EXT_SYNC_SET1)|bit(MPU6050_EXT_SYNC_SET0))
#define MPU6050_EXT_SYNC_DISABLED     MPU6050_EXT_SYNC_SET_0
#define MPU6050_EXT_SYNC_TEMP_OUT_L   MPU6050_EXT_SYNC_SET_1
#define MPU6050_EXT_SYNC_GYRO_XOUT_L  MPU6050_EXT_SYNC_SET_2
#define MPU6050_EXT_SYNC_GYRO_YOUT_L  MPU6050_EXT_SYNC_SET_3
#define MPU6050_EXT_SYNC_GYRO_ZOUT_L  MPU6050_EXT_SYNC_SET_4
#define MPU6050_EXT_SYNC_ACCEL_XOUT_L MPU6050_EXT_SYNC_SET_5
#define MPU6050_EXT_SYNC_ACCEL_YOUT_L MPU6050_EXT_SYNC_SET_6
#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L MPU6050_EXT_SYNC_SET_7

//DLPF_CFG 변수
#define MPU6050_DLPF_CFG_0 (0)
#define MPU6050_DLPF_CFG_1 (bit(MPU6050_DLPF_CFG0))
#define MPU6050_DLPF_CFG_2 (bit(MPU6050_DLPF_CFG1))
#define MPU6050_DLPF_CFG_3 (bit(MPU6050_DLPF_CFG1)|bit(MPU6050_DLPF_CFG0))
#define MPU6050_DLPF_CFG_4 (bit(MPU6050_DLPF_CFG2))
#define MPU6050_DLPF_CFG_5 (bit(MPU6050_DLPF_CFG2)|bit(MPU6050_DLPF_CFG0))
#define MPU6050_DLPF_CFG_6 (bit(MPU6050_DLPF_CFG2)|bit(MPU6050_DLPF_CFG1))
#define MPU6050_DLPF_CFG_7 (bit(MPU6050_DLPF_CFG2)|bit(MPU6050_DLPF_CFG1)|bit(MPU6050_DLPF_CFG0))
#define MPU6050_DLPF_260HZ    MPU6050_DLPF_CFG_0
#define MPU6050_DLPF_184HZ    MPU6050_DLPF_CFG_1
#define MPU6050_DLPF_94HZ     MPU6050_DLPF_CFG_2
#define MPU6050_DLPF_44HZ     MPU6050_DLPF_CFG_3
#define MPU6050_DLPF_21HZ     MPU6050_DLPF_CFG_4
#define MPU6050_DLPF_10HZ     MPU6050_DLPF_CFG_5
#define MPU6050_DLPF_5HZ      MPU6050_DLPF_CFG_6
#define MPU6050_DLPF_RESERVED MPU6050_DLPF_CFG_7

// GYRO_CONFIG Register
#define MPU6050_FS_SEL0 MPU6050_D3
#define MPU6050_FS_SEL1 MPU6050_D4
#define MPU6050_ZG_ST   MPU6050_D5
#define MPU6050_YG_ST   MPU6050_D6
#define MPU6050_XG_ST   MPU6050_D7

//the FS_SEL 변수
#define MPU6050_FS_SEL_0 (0)
#define MPU6050_FS_SEL_1 (bit(MPU6050_FS_SEL0))
#define MPU6050_FS_SEL_2 (bit(MPU6050_FS_SEL1))
#define MPU6050_FS_SEL_3 (bit(MPU6050_FS_SEL1)|bit(MPU6050_FS_SEL0))
#define MPU6050_FS_SEL_250  MPU6050_FS_SEL_0
#define MPU6050_FS_SEL_500  MPU6050_FS_SEL_1
#define MPU6050_FS_SEL_1000 MPU6050_FS_SEL_2
#define MPU6050_FS_SEL_2000 MPU6050_FS_SEL_3

// ACCEL_CONFIG Register
#define MPU6050_ACCEL_HPF0 MPU6050_D0
#define MPU6050_ACCEL_HPF1 MPU6050_D1
#define MPU6050_ACCEL_HPF2 MPU6050_D2
#define MPU6050_AFS_SEL0   MPU6050_D3
#define MPU6050_AFS_SEL1   MPU6050_D4
#define MPU6050_ZA_ST      MPU6050_D5
#define MPU6050_YA_ST      MPU6050_D6
#define MPU6050_XA_ST      MPU6050_D7

//ACCEL_HPF 변수
#define MPU6050_ACCEL_HPF_0 (0)
#define MPU6050_ACCEL_HPF_1 (bit(MPU6050_ACCEL_HPF0))
#define MPU6050_ACCEL_HPF_2 (bit(MPU6050_ACCEL_HPF1))
#define MPU6050_ACCEL_HPF_3 (bit(MPU6050_ACCEL_HPF1)|bit(MPU6050_ACCEL_HPF0))
#define MPU6050_ACCEL_HPF_4 (bit(MPU6050_ACCEL_HPF2))
#define MPU6050_ACCEL_HPF_7 (bit(MPU6050_ACCEL_HPF2)|bit(MPU6050_ACCEL_HPF1)|bit(MPU6050_ACCEL_HPF0))
#define MPU6050_ACCEL_HPF_RESET  MPU6050_ACCEL_HPF_0
#define MPU6050_ACCEL_HPF_5HZ    MPU6050_ACCEL_HPF_1
#define MPU6050_ACCEL_HPF_2_5HZ  MPU6050_ACCEL_HPF_2
#define MPU6050_ACCEL_HPF_1_25HZ MPU6050_ACCEL_HPF_3
#define MPU6050_ACCEL_HPF_0_63HZ MPU6050_ACCEL_HPF_4
#define MPU6050_ACCEL_HPF_HOLD   MPU6050_ACCEL_HPF_7

//AFS_SEL 변수
#define MPU6050_AFS_SEL_0 (0)
#define MPU6050_AFS_SEL_1 (bit(MPU6050_AFS_SEL0))
#define MPU6050_AFS_SEL_2 (bit(MPU6050_AFS_SEL1))
#define MPU6050_AFS_SEL_3 (bit(MPU6050_AFS_SEL1)|bit(MPU6050_AFS_SEL0))
#define MPU6050_AFS_SEL_2G  MPU6050_AFS_SEL_0
#define MPU6050_AFS_SEL_4G  MPU6050_AFS_SEL_1
#define MPU6050_AFS_SEL_8G  MPU6050_AFS_SEL_2
#define MPU6050_AFS_SEL_16G MPU6050_AFS_SEL_3

// FIFO_EN Register
#define MPU6050_SLV0_FIFO_EN  MPU6050_D0
#define MPU6050_SLV1_FIFO_EN  MPU6050_D1
#define MPU6050_SLV2_FIFO_EN  MPU6050_D2
#define MPU6050_ACCEL_FIFO_EN MPU6050_D3
#define MPU6050_ZG_FIFO_EN    MPU6050_D4
#define MPU6050_YG_FIFO_EN    MPU6050_D5
#define MPU6050_XG_FIFO_EN    MPU6050_D6
#define MPU6050_TEMP_FIFO_EN  MPU6050_D7

// I2C_MST_CTRL Register
#define MPU6050_I2C_MST_CLK0  MPU6050_D0
#define MPU6050_I2C_MST_CLK1  MPU6050_D1
#define MPU6050_I2C_MST_CLK2  MPU6050_D2
#define MPU6050_I2C_MST_CLK3  MPU6050_D3
#define MPU6050_I2C_MST_P_NSR MPU6050_D4
#define MPU6050_SLV_3_FIFO_EN MPU6050_D5
#define MPU6050_WAIT_FOR_ES   MPU6050_D6
#define MPU6050_MULT_MST_EN   MPU6050_D7

//I2C_MST_CLK
#define MPU6050_I2C_MST_CLK_0 (0)
#define MPU6050_I2C_MST_CLK_1  (bit(MPU6050_I2C_MST_CLK0))
#define MPU6050_I2C_MST_CLK_2  (bit(MPU6050_I2C_MST_CLK1))
#define MPU6050_I2C_MST_CLK_3  (bit(MPU6050_I2C_MST_CLK1)|bit(MPU6050_I2C_MST_CLK0))
#define MPU6050_I2C_MST_CLK_4  (bit(MPU6050_I2C_MST_CLK2))
#define MPU6050_I2C_MST_CLK_5  (bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK0))
#define MPU6050_I2C_MST_CLK_6  (bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK1))
#define MPU6050_I2C_MST_CLK_7  (bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK1)|bit(MPU6050_I2C_MST_CLK0))
#define MPU6050_I2C_MST_CLK_8  (bit(MPU6050_I2C_MST_CLK3))
#define MPU6050_I2C_MST_CLK_9  (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK0))
#define MPU6050_I2C_MST_CLK_10 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK1))
#define MPU6050_I2C_MST_CLK_11 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK1)|bit(MPU6050_I2C_MST_CLK0))
#define MPU6050_I2C_MST_CLK_12 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK2))
#define MPU6050_I2C_MST_CLK_13 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK0))
#define MPU6050_I2C_MST_CLK_14 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK1))
#define MPU6050_I2C_MST_CLK_15 (bit(MPU6050_I2C_MST_CLK3)|bit(MPU6050_I2C_MST_CLK2)|bit(MPU6050_I2C_MST_CLK1)|bit(MPU6050_I2C_MST_CLK0))
#define MPU6050_I2C_MST_CLK_348KHZ MPU6050_I2C_MST_CLK_0
#define MPU6050_I2C_MST_CLK_333KHZ MPU6050_I2C_MST_CLK_1
#define MPU6050_I2C_MST_CLK_320KHZ MPU6050_I2C_MST_CLK_2
#define MPU6050_I2C_MST_CLK_308KHZ MPU6050_I2C_MST_CLK_3
#define MPU6050_I2C_MST_CLK_296KHZ MPU6050_I2C_MST_CLK_4
#define MPU6050_I2C_MST_CLK_286KHZ MPU6050_I2C_MST_CLK_5
#define MPU6050_I2C_MST_CLK_276KHZ MPU6050_I2C_MST_CLK_6
#define MPU6050_I2C_MST_CLK_267KHZ MPU6050_I2C_MST_CLK_7
#define MPU6050_I2C_MST_CLK_258KHZ MPU6050_I2C_MST_CLK_8
#define MPU6050_I2C_MST_CLK_500KHZ MPU6050_I2C_MST_CLK_9
#define MPU6050_I2C_MST_CLK_471KHZ MPU6050_I2C_MST_CLK_10
#define MPU6050_I2C_MST_CLK_444KHZ MPU6050_I2C_MST_CLK_11
#define MPU6050_I2C_MST_CLK_421KHZ MPU6050_I2C_MST_CLK_12
#define MPU6050_I2C_MST_CLK_400KHZ MPU6050_I2C_MST_CLK_13
#define MPU6050_I2C_MST_CLK_381KHZ MPU6050_I2C_MST_CLK_14
#define MPU6050_I2C_MST_CLK_364KHZ MPU6050_I2C_MST_CLK_15

// I2C_SLV0_ADDR Register
#define MPU6050_I2C_SLV0_RW MPU6050_D7

// I2C_SLV0_CTRL Register
#define MPU6050_I2C_SLV0_LEN0    MPU6050_D0
#define MPU6050_I2C_SLV0_LEN1    MPU6050_D1
#define MPU6050_I2C_SLV0_LEN2    MPU6050_D2
#define MPU6050_I2C_SLV0_LEN3    MPU6050_D3
#define MPU6050_I2C_SLV0_GRP     MPU6050_D4
#define MPU6050_I2C_SLV0_REG_DIS MPU6050_D5
#define MPU6050_I2C_SLV0_BYTE_SW MPU6050_D6
#define MPU6050_I2C_SLV0_EN      MPU6050_D7

// A mask for the length
#define MPU6050_I2C_SLV0_LEN_MASK 0x0F

// I2C_SLV1_ADDR Register
#define MPU6050_I2C_SLV1_RW MPU6050_D7

// I2C_SLV1_CTRL Register
#define MPU6050_I2C_SLV1_LEN0    MPU6050_D0
#define MPU6050_I2C_SLV1_LEN1    MPU6050_D1
#define MPU6050_I2C_SLV1_LEN2    MPU6050_D2
#define MPU6050_I2C_SLV1_LEN3    MPU6050_D3
#define MPU6050_I2C_SLV1_GRP     MPU6050_D4
#define MPU6050_I2C_SLV1_REG_DIS MPU6050_D5
#define MPU6050_I2C_SLV1_BYTE_SW MPU6050_D6
#define MPU6050_I2C_SLV1_EN      MPU6050_D7

// A mask for the length
#define MPU6050_I2C_SLV1_LEN_MASK 0x0F

// I2C_SLV2_ADDR Register
#define MPU6050_I2C_SLV2_RW MPU6050_D7

// I2C_SLV2_CTRL Register
#define MPU6050_I2C_SLV2_LEN0    MPU6050_D0
#define MPU6050_I2C_SLV2_LEN1    MPU6050_D1
#define MPU6050_I2C_SLV2_LEN2    MPU6050_D2
#define MPU6050_I2C_SLV2_LEN3    MPU6050_D3
#define MPU6050_I2C_SLV2_GRP     MPU6050_D4
#define MPU6050_I2C_SLV2_REG_DIS MPU6050_D5
#define MPU6050_I2C_SLV2_BYTE_SW MPU6050_D6
#define MPU6050_I2C_SLV2_EN      MPU6050_D7

// A mask for the length
#define MPU6050_I2C_SLV2_LEN_MASK 0x0F

// I2C_SLV3_ADDR Register
#define MPU6050_I2C_SLV3_RW MPU6050_D7

// I2C_SLV3_CTRL Register
#define MPU6050_I2C_SLV3_LEN0    MPU6050_D0
#define MPU6050_I2C_SLV3_LEN1    MPU6050_D1
#define MPU6050_I2C_SLV3_LEN2    MPU6050_D2
#define MPU6050_I2C_SLV3_LEN3    MPU6050_D3
#define MPU6050_I2C_SLV3_GRP     MPU6050_D4
#define MPU6050_I2C_SLV3_REG_DIS MPU6050_D5
#define MPU6050_I2C_SLV3_BYTE_SW MPU6050_D6
#define MPU6050_I2C_SLV3_EN      MPU6050_D7

// A mask for the length
#define MPU6050_I2C_SLV3_LEN_MASK 0x0F

// I2C_SLV4_ADDR Register
#define MPU6050_I2C_SLV4_RW MPU6050_D7

// I2C_SLV4_CTRL Register
#define MPU6050_I2C_MST_DLY0     MPU6050_D0
#define MPU6050_I2C_MST_DLY1     MPU6050_D1
#define MPU6050_I2C_MST_DLY2     MPU6050_D2
#define MPU6050_I2C_MST_DLY3     MPU6050_D3
#define MPU6050_I2C_MST_DLY4     MPU6050_D4
#define MPU6050_I2C_SLV4_REG_DIS MPU6050_D5
#define MPU6050_I2C_SLV4_INT_EN  MPU6050_D6
#define MPU6050_I2C_SLV4_EN      MPU6050_D7

// A mask for the delay
#define MPU6050_I2C_MST_DLY_MASK 0x1F

// I2C_MST_STATUS Register
#define MPU6050_I2C_SLV0_NACK MPU6050_D0
#define MPU6050_I2C_SLV1_NACK MPU6050_D1
#define MPU6050_I2C_SLV2_NACK MPU6050_D2
#define MPU6050_I2C_SLV3_NACK MPU6050_D3
#define MPU6050_I2C_SLV4_NACK MPU6050_D4
#define MPU6050_I2C_LOST_ARB  MPU6050_D5
#define MPU6050_I2C_SLV4_DONE MPU6050_D6
#define MPU6050_PASS_THROUGH  MPU6050_D7

// I2C_PIN_CFG Register
#define MPU6050_CLKOUT_EN       MPU6050_D0
#define MPU6050_I2C_BYPASS_EN   MPU6050_D1
#define MPU6050_FSYNC_INT_EN    MPU6050_D2
#define MPU6050_FSYNC_INT_LEVEL MPU6050_D3
#define MPU6050_INT_RD_CLEAR    MPU6050_D4
#define MPU6050_LATCH_INT_EN    MPU6050_D5
#define MPU6050_INT_OPEN        MPU6050_D6
#define MPU6050_INT_LEVEL       MPU6050_D7

// INT_ENABLE Register
#define MPU6050_DATA_RDY_EN    MPU6050_D0
#define MPU6050_I2C_MST_INT_EN MPU6050_D3
#define MPU6050_FIFO_OFLOW_EN  MPU6050_D4
#define MPU6050_ZMOT_EN        MPU6050_D5
#define MPU6050_MOT_EN         MPU6050_D6
#define MPU6050_FF_EN          MPU6050_D7

// INT_STATUS Register
#define MPU6050_DATA_RDY_INT   MPU6050_D0
#define MPU6050_I2C_MST_INT    MPU6050_D3
#define MPU6050_FIFO_OFLOW_INT MPU6050_D4
#define MPU6050_ZMOT_INT       MPU6050_D5
#define MPU6050_MOT_INT        MPU6050_D6
#define MPU6050_FF_INT         MPU6050_D7

// MOT_DETECT_STATUS Register
#define MPU6050_MOT_ZRMOT MPU6050_D0
#define MPU6050_MOT_ZPOS  MPU6050_D2
#define MPU6050_MOT_ZNEG  MPU6050_D3
#define MPU6050_MOT_YPOS  MPU6050_D4
#define MPU6050_MOT_YNEG  MPU6050_D5
#define MPU6050_MOT_XPOS  MPU6050_D6
#define MPU6050_MOT_XNEG  MPU6050_D7

// IC2_MST_DELAY_CTRL Register
#define MPU6050_I2C_SLV0_DLY_EN MPU6050_D0
#define MPU6050_I2C_SLV1_DLY_EN MPU6050_D1
#define MPU6050_I2C_SLV2_DLY_EN MPU6050_D2
#define MPU6050_I2C_SLV3_DLY_EN MPU6050_D3
#define MPU6050_I2C_SLV4_DLY_EN MPU6050_D4
#define MPU6050_DELAY_ES_SHADOW MPU6050_D7

// SIGNAL_PATH_RESET Register
#define MPU6050_TEMP_RESET  MPU6050_D0
#define MPU6050_ACCEL_RESET MPU6050_D1
#define MPU6050_GYRO_RESET  MPU6050_D2

// MOT_DETECT_CTRL Register
#define MPU6050_MOT_COUNT0      MPU6050_D0
#define MPU6050_MOT_COUNT1      MPU6050_D1
#define MPU6050_FF_COUNT0       MPU6050_D2
#define MPU6050_FF_COUNT1       MPU6050_D3
#define MPU6050_ACCEL_ON_DELAY0 MPU6050_D4
#define MPU6050_ACCEL_ON_DELAY1 MPU6050_D5

//MOT_COUNT
#define MPU6050_MOT_COUNT_0 (0)
#define MPU6050_MOT_COUNT_1 (bit(MPU6050_MOT_COUNT0))
#define MPU6050_MOT_COUNT_2 (bit(MPU6050_MOT_COUNT1))
#define MPU6050_MOT_COUNT_3 (bit(MPU6050_MOT_COUNT1)|bit(MPU6050_MOT_COUNT0))
#define MPU6050_MOT_COUNT_RESET MPU6050_MOT_COUNT_0

//FF_COUNT
#define MPU6050_FF_COUNT_0 (0)
#define MPU6050_FF_COUNT_1 (bit(MPU6050_FF_COUNT0))
#define MPU6050_FF_COUNT_2 (bit(MPU6050_FF_COUNT1))
#define MPU6050_FF_COUNT_3 (bit(MPU6050_FF_COUNT1)|bit(MPU6050_FF_COUNT0))
#define MPU6050_FF_COUNT_RESET MPU6050_FF_COUNT_0

//ACCEL_ON_DELAY
#define MPU6050_ACCEL_ON_DELAY_0 (0)
#define MPU6050_ACCEL_ON_DELAY_1 (bit(MPU6050_ACCEL_ON_DELAY0))
#define MPU6050_ACCEL_ON_DELAY_2 (bit(MPU6050_ACCEL_ON_DELAY1))
#define MPU6050_ACCEL_ON_DELAY_3 (bit(MPU6050_ACCEL_ON_DELAY1)|bit(MPU6050_ACCEL_ON_DELAY0))
#define MPU6050_ACCEL_ON_DELAY_0MS MPU6050_ACCEL_ON_DELAY_0
#define MPU6050_ACCEL_ON_DELAY_1MS MPU6050_ACCEL_ON_DELAY_1
#define MPU6050_ACCEL_ON_DELAY_2MS MPU6050_ACCEL_ON_DELAY_2
#define MPU6050_ACCEL_ON_DELAY_3MS MPU6050_ACCEL_ON_DELAY_3

// USER_CTRL Register
#define MPU6050_SIG_COND_RESET MPU6050_D0
#define MPU6050_I2C_MST_RESET  MPU6050_D1
#define MPU6050_FIFO_RESET     MPU6050_D2
#define MPU6050_I2C_IF_DIS     MPU6050_D4  
#define MPU6050_I2C_MST_EN     MPU6050_D5
#define MPU6050_FIFO_EN        MPU6050_D6

// PWR_MGMT_1 Register
#define MPU6050_CLKSEL0      MPU6050_D0
#define MPU6050_CLKSEL1      MPU6050_D1
#define MPU6050_CLKSEL2      MPU6050_D2
#define MPU6050_TEMP_DIS     MPU6050_D3    
#define MPU6050_CYCLE        MPU6050_D5    
#define MPU6050_SLEEP        MPU6050_D6   
#define MPU6050_DEVICE_RESET MPU6050_D7   

//CLKSEL
#define MPU6050_CLKSEL_0 (0)
#define MPU6050_CLKSEL_1 (bit(MPU6050_CLKSEL0))
#define MPU6050_CLKSEL_2 (bit(MPU6050_CLKSEL1))
#define MPU6050_CLKSEL_3 (bit(MPU6050_CLKSEL1)|bit(MPU6050_CLKSEL0))
#define MPU6050_CLKSEL_4 (bit(MPU6050_CLKSEL2))
#define MPU6050_CLKSEL_5 (bit(MPU6050_CLKSEL2)|bit(MPU6050_CLKSEL0))
#define MPU6050_CLKSEL_6 (bit(MPU6050_CLKSEL2)|bit(MPU6050_CLKSEL1))
#define MPU6050_CLKSEL_7 (bit(MPU6050_CLKSEL2)|bit(MPU6050_CLKSEL1)|bit(MPU6050_CLKSEL0))
#define MPU6050_CLKSEL_INTERNAL    MPU6050_CLKSEL_0
#define MPU6050_CLKSEL_X           MPU6050_CLKSEL_1
#define MPU6050_CLKSEL_Y           MPU6050_CLKSEL_2
#define MPU6050_CLKSEL_Z           MPU6050_CLKSEL_3
#define MPU6050_CLKSEL_EXT_32KHZ   MPU6050_CLKSEL_4
#define MPU6050_CLKSEL_EXT_19_2MHZ MPU6050_CLKSEL_5
#define MPU6050_CLKSEL_RESERVED    MPU6050_CLKSEL_6
#define MPU6050_CLKSEL_STOP        MPU6050_CLKSEL_7

// PWR_MGMT_2 Register
#define MPU6050_STBY_ZG       MPU6050_D0
#define MPU6050_STBY_YG       MPU6050_D1
#define MPU6050_STBY_XG       MPU6050_D2
#define MPU6050_STBY_ZA       MPU6050_D3
#define MPU6050_STBY_YA       MPU6050_D4
#define MPU6050_STBY_XA       MPU6050_D5
#define MPU6050_LP_WAKE_CTRL0 MPU6050_D6
#define MPU6050_LP_WAKE_CTRL1 MPU6050_D7

//LP_WAKE_CTRL
#define MPU6050_LP_WAKE_CTRL_0 (0)
#define MPU6050_LP_WAKE_CTRL_1 (bit(MPU6050_LP_WAKE_CTRL0))
#define MPU6050_LP_WAKE_CTRL_2 (bit(MPU6050_LP_WAKE_CTRL1))
#define MPU6050_LP_WAKE_CTRL_3 (bit(MPU6050_LP_WAKE_CTRL1)|bit(MPU6050_LP_WAKE_CTRL0))
#define MPU6050_LP_WAKE_1_25HZ MPU6050_LP_WAKE_CTRL_0
#define MPU6050_LP_WAKE_2_5HZ  MPU6050_LP_WAKE_CTRL_1
#define MPU6050_LP_WAKE_5HZ    MPU6050_LP_WAKE_CTRL_2
#define MPU6050_LP_WAKE_10HZ   MPU6050_LP_WAKE_CTRL_3

//MPU-6050의 기본 I2C 주소는 0x68.
#define MPU6050_I2C_ADDRESS 0x68

//저항, axis 변수들을 선언
typedef union accel_t_gyro_union
{
  struct
  {
    uint8_t x_accel_h;
    uint8_t x_accel_l;
    uint8_t y_accel_h;
    uint8_t y_accel_l;
    uint8_t z_accel_h;
    uint8_t z_accel_l;
    uint8_t t_h;
    uint8_t t_l;
    uint8_t x_gyro_h;
    uint8_t x_gyro_l;
    uint8_t y_gyro_h;
    uint8_t y_gyro_l;
    uint8_t z_gyro_h;
    uint8_t z_gyro_l;
  } reg;
  struct 
  {
    int x_accel;
    int y_accel;
    int z_accel;
    int temperature;
    int x_gyro;
    int y_gyro;
    int z_gyro;
  } value;
};

//변경된 각도와 최종 각도를 저장할 변수
unsigned long last_read_time;
float         last_x_angle;  
float         last_y_angle;
float         last_z_angle;  
float         last_gyro_x_angle;  
float         last_gyro_y_angle;
float         last_gyro_z_angle;

//각도를 바꾸는 함수
void set_last_read_angle_data(unsigned long time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) {
  last_read_time = time;
  last_x_angle = x;
  last_y_angle = y;
  last_z_angle = z;
  last_gyro_x_angle = x_gyro;
  last_gyro_y_angle = y_gyro;
  last_gyro_z_angle = z_gyro;
}

//값 반환 함수
inline unsigned long get_last_time() {return last_read_time;}
inline float get_last_x_angle() {return last_x_angle;}
inline float get_last_y_angle() {return last_y_angle;}
inline float get_last_z_angle() {return last_z_angle;}
inline float get_last_gyro_x_angle() {return last_gyro_x_angle;}
inline float get_last_gyro_y_angle() {return last_gyro_y_angle;}
inline float get_last_gyro_z_angle() {return last_gyro_z_angle;}

//센서에서 바로 읽어온 값을 저장할 변수
float    base_x_accel;
float    base_y_accel;
float    base_z_accel;

float    base_x_gyro;
float    base_y_gyro;
float    base_z_gyro;

//원시 데이터를 읽어오는 함수
int read_gyro_accel_vals(uint8_t* accel_t_gyro_ptr) {
  
  accel_t_gyro_union* accel_t_gyro = (accel_t_gyro_union *) accel_t_gyro_ptr;
   
  int error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) accel_t_gyro, sizeof(*accel_t_gyro));

  //high byte와 low byte 바꾸기
  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap

  SWAP ((*accel_t_gyro).reg.x_accel_h, (*accel_t_gyro).reg.x_accel_l);
  SWAP ((*accel_t_gyro).reg.y_accel_h, (*accel_t_gyro).reg.y_accel_l);
  SWAP ((*accel_t_gyro).reg.z_accel_h, (*accel_t_gyro).reg.z_accel_l);
  SWAP ((*accel_t_gyro).reg.t_h, (*accel_t_gyro).reg.t_l);
  SWAP ((*accel_t_gyro).reg.x_gyro_h, (*accel_t_gyro).reg.x_gyro_l);
  SWAP ((*accel_t_gyro).reg.y_gyro_h, (*accel_t_gyro).reg.y_gyro_l);
  SWAP ((*accel_t_gyro).reg.z_gyro_h, (*accel_t_gyro).reg.z_gyro_l);

  return error;
}

//움직임이 없을 때 일차적으로 값을 읽어와 전역변수에 저장
void calibrate_sensors() {
  int                   num_readings = 10;
  float                 x_accel = 0;
  float                 y_accel = 0;
  float                 z_accel = 0;
  float                 x_gyro = 0;
  float                 y_gyro = 0;
  float                 z_gyro = 0;
  accel_t_gyro_union    accel_t_gyro;
 
  //첫번째 부분 읽어오기
  read_gyro_accel_vals((uint8_t *) &accel_t_gyro);
  
  //원시 데이터들의 평균 읽기
  for (int i = 0; i < num_readings; i++) {
    read_gyro_accel_vals((uint8_t *) &accel_t_gyro);
    x_accel += accel_t_gyro.value.x_accel;
    y_accel += accel_t_gyro.value.y_accel;
    z_accel += accel_t_gyro.value.z_accel;
    x_gyro += accel_t_gyro.value.x_gyro;
    y_gyro += accel_t_gyro.value.y_gyro;
    z_gyro += accel_t_gyro.value.z_gyro;
    delay(100);
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

void setup()
{      
  int error;
  uint8_t c;

  Serial.begin(19200);

  Wire.begin();      

  error = MPU6050_read (MPU6050_WHO_AM_I, &c, 1);

  error = MPU6050_read (MPU6050_PWR_MGMT_2, &c, 1);

  //센서 시작 모드
  MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);
  
  //각도 초기화
  calibrate_sensors();  
  set_last_read_angle_data(millis(), 0, 0, 0, 0, 0, 0);
}


void loop()
{
  int error;
  double dT;
  accel_t_gyro_union accel_t_gyro;

  error = read_gyro_accel_vals((uint8_t*) &accel_t_gyro);
  
  //회전을 했을 떄 시간 알기
  unsigned long t_now = millis();

  //원시 데이터를 각도로 변환
  float FS_SEL = 131;
 
  float gyro_x = (accel_t_gyro.value.x_gyro - base_x_gyro)/FS_SEL;
  float gyro_y = (accel_t_gyro.value.y_gyro - base_y_gyro)/FS_SEL;
  float gyro_z = (accel_t_gyro.value.z_gyro - base_z_gyro)/FS_SEL;
  
  //acceleration 원시 데이터 저장
  float accel_x = accel_t_gyro.value.x_accel;
  float accel_y = accel_t_gyro.value.y_accel;
  float accel_z = accel_t_gyro.value.z_accel;
  
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
  
  //프로세싱으로 데이터 보내기
  Serial.print(F("DEL:"));              //Delta T
  Serial.print(dt, DEC);
  Serial.print(F("#ACC:"));             //Accelerometer angle
  Serial.print(accel_angle_x, 2);
  Serial.print(F(","));
  Serial.print(accel_angle_y, 2);
  Serial.print(F(","));
  Serial.print(accel_angle_z, 2);
  Serial.print(F("#GYR:"));
  Serial.print(unfiltered_gyro_angle_x, 2);  //Gyroscope angle
  Serial.print(F(","));
  Serial.print(unfiltered_gyro_angle_y, 2);
  Serial.print(F(","));
  Serial.print(unfiltered_gyro_angle_z, 2);
  Serial.print(F("#FIL:"));                  //Filtered angle
  Serial.print(angle_x, 2);
  Serial.print(F(","));
  Serial.print(angle_y, 2);
  Serial.print(F(","));
  Serial.print(angle_z, 2);
  Serial.println(F(""));
  
  delay(5);
}

//MPU6050에서 데이터 읽기
int MPU6050_read(int start, uint8_t *buffer, int size)
{
  int i, n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
    return (-10);

  n = Wire.endTransmission(false);   
  if (n != 0)
    return (n);

  Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);

  return (0);  // return : no error
}

//전송 버퍼에 데이터 기록하는 함수
int MPU6050_write(int start, const uint8_t *pData, int size)
{
  int n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);        
  if (n != 1)
    return (-20);

  n = Wire.write(pData, size);  
  if (n != size)
    return (-21);

  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
    return (error);

  return (0);         // return : no error
}

//싱글 레지스터에 기록하기 위한 함수
int MPU6050_write_reg(int reg, uint8_t data)
{
  int error;

  error = MPU6050_write(reg, &data, 1);

  return (error);
}


//
// 전역 선언들 모음.
//

#pragma once

// for roundf()
#define __USE_C99_MATH

#include <stdbool.h>
//#include <stdlib.h>
//#include <stdint.h>
#include <math.h>
//#include <ctype.h>
//#include <string.h>
#include <stdio.h>

#ifndef __CC_ARM
// only need this garbage on gcc
#define USE_LAME_PRINTF
#include "printf.h"
#endif



#define NULL 0

#ifndef M_PI
#define M_PI       3.14159265358979323846f
#endif /* M_PI */

#define RADX10 (M_PI / 1800.0f)                  // 0.001745329252f
#define RAD    (M_PI / 180.0f)

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))



#include "stm32f10x.h"

#define RC_CHANS    (18)




typedef enum {
    SENSOR_GYRO = 1 << 0, // always present
    SENSOR_ACC = 1 << 1,
    SENSOR_BARO = 1 << 2,
    SENSOR_MAG = 1 << 3,
    SENSOR_SONAR = 1 << 4,
    SENSOR_GPS = 1 << 5,
    SENSOR_GPSMAG = 1 << 6,
} AvailableSensors;



// Type of accelerometer used/detected
typedef enum AccelSensors {
    ACC_DEFAULT = 0,
    ACC_ADXL345 = 1,
    ACC_MPU6050 = 2,
    ACC_MMA8452 = 3,
    ACC_BMA280 = 4,
    ACC_MPU6500 = 5,
    ACC_NONE = 6
} AccelSensors;

typedef enum CompassSensors {
    MAG_DEFAULT = 0,
    MAG_HMC5883L = 1,
    MAG_AK8975 = 2,
    MAG_NONE = 3
} CompassSensors;


typedef enum {
    X = 0,
    Y,
    Z
} sensor_axis_e;


typedef enum {
    ALIGN_DEFAULT = 0,                                      // driver-provided alignment
    CW0_DEG = 1,
    CW90_DEG = 2,
    CW180_DEG = 3,
    CW270_DEG = 4,
    CW0_DEG_FLIP = 5,
    CW90_DEG_FLIP = 6,
    CW180_DEG_FLIP = 7,
    CW270_DEG_FLIP = 8
} sensor_align_e;


enum {
    GYRO_UPDATED = 1 << 0,
    ACC_UPDATED = 1 << 1,
    MAG_UPDATED = 1 << 2,
    TEMP_UPDATED = 1 << 3
};

typedef struct sensor_data_t {
    int16_t gyro[3];
    int16_t acc[3];
    int16_t mag[3];
    float temperature;
    int updated;
} sensor_data_t;


typedef void (*sensorInitFuncPtr)(sensor_align_e align);   // sensor init prototype
typedef void (*sensorReadFuncPtr)(int16_t *data);          // sensor read and align prototype
typedef void (*baroOpFuncPtr)(void);                       // baro start operation
typedef void (*baroCalculateFuncPtr)(int32_t *pressure, int32_t *temperature);             // baro calculation (filled params are pressure and temperature)
typedef void (*serialReceiveCallbackPtr)(uint16_t data);   // used by serial drivers to return frames to app
typedef uint16_t (*rcReadRawDataPtr)(uint8_t chan);        // used by receiver driver to return channel data
typedef void (*pidControllerFuncPtr)(void);                // pid controller function prototype


typedef struct sensor_t {
    sensorInitFuncPtr init;                                 // initialize function
    sensorReadFuncPtr read;                                 // read 3 axis data function
    sensorReadFuncPtr temperature;                          // read temperature if available
    float scale;                                            // scalefactor (currently used for gyro only, todo for accel)
} sensor_t;




#define LED0
#define LED1




#define MAX_MOTORS  12




#include "stm32f10x_i2c.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_dma.h"


#include "drv_gpio.h"
#include "drv_system.h"
#include "drv_serial.h"
#include "drv_uart.h"


#include "drv_i2c.h"
#include "drv_hmc5883l.h"
#include "drv_mpu6050.h"
#include "utils.h"



#define LED0_GPIO   GPIOC
#define LED0_PIN    Pin_9 // PB3 (LED)
#define LED1_GPIO   GPIOC
#define LED1_PIN    Pin_8 // PB4 (LED)
#define BEEP_GPIO   GPIOA
#define BEEP_PIN    Pin_12 // PA12 (Buzzer)
#define BARO_GPIO   GPIOC
#define BARO_PIN    Pin_13
#define INV_PIN     Pin_2 // PB2 (BOOT1) abused as inverter select GPIO
#define INV_GPIO    GPIOB



// Helpful macros
#ifdef LED0
#define LED0_TOGGLE              digitalToggle(LED0_GPIO, LED0_PIN);
#define LED0_OFF                 digitalHi(LED0_GPIO, LED0_PIN);
#define LED0_ON                  digitalLo(LED0_GPIO, LED0_PIN);
#else
#define LED0_TOGGLE
#define LED0_OFF
#define LED0_ON
#endif

#ifdef LED1
#define LED1_TOGGLE              digitalToggle(LED1_GPIO, LED1_PIN);
#define LED1_OFF                 digitalHi(LED1_GPIO, LED1_PIN);
#define LED1_ON                  digitalLo(LED1_GPIO, LED1_PIN);
#else
#define LED1_TOGGLE
#define LED1_OFF
#define LED1_ON
#endif

#ifdef BEEP_GPIO
#define BEEP_TOGGLE              digitalToggle(BEEP_GPIO, BEEP_PIN);
#define BEEP_OFF                 systemBeep(false);
#define BEEP_ON                  systemBeep(true);
#else
#define BEEP_TOGGLE              ;
#define BEEP_OFF                 ;
#define BEEP_ON                  ;
#endif





/*********** RC alias *****************/
enum {
    ROLL = 0,
    PITCH,
    YAW,
    THROTTLE,
    AUX1,
    AUX2,
    AUX3,
    AUX4
};

enum {
    PIDROLL,
    PIDPITCH,
    PIDYAW,
    PIDALT,
    PIDPOS,
    PIDPOSR,
    PIDNAVR,
    PIDLEVEL,
    PIDMAG,
    PIDVEL,
    PIDITEMS
};

enum {
    BOXARM = 0,
    BOXANGLE,
    BOXHORIZON,
    BOXBARO,
    BOXVARIO,
    BOXMAG,
    BOXHEADFREE,
    BOXHEADADJ,
    BOXCAMSTAB,
    BOXCAMTRIG,
    BOXGPSHOME,
    BOXGPSHOLD,
    BOXPASSTHRU,
    BOXBEEPERON,
    BOXLEDMAX,
    BOXLEDLOW,
    BOXLLIGHTS,
    BOXCALIB,
    BOXGOV,
    BOXOSD,
    BOXTELEMETRY,
    BOXSERVO1,
    BOXSERVO2,
    BOXSERVO3,
    CHECKBOXITEMS
};

#define ROL_LO (1 << (2 * ROLL))
#define ROL_CE (3 << (2 * ROLL))
#define ROL_HI (2 << (2 * ROLL))
#define PIT_LO (1 << (2 * PITCH))
#define PIT_CE (3 << (2 * PITCH))
#define PIT_HI (2 << (2 * PITCH))
#define YAW_LO (1 << (2 * YAW))
#define YAW_CE (3 << (2 * YAW))
#define YAW_HI (2 << (2 * YAW))
#define THR_LO (1 << (2 * THROTTLE))
#define THR_CE (3 << (2 * THROTTLE))
#define THR_HI (2 << (2 * THROTTLE))


enum {
    ALIGN_GYRO = 0,
    ALIGN_ACCEL = 1,
    ALIGN_MAG = 2
};

#define CALIBRATING_GYRO_CYCLES             1000
#define CALIBRATING_ACC_CYCLES              400
#define CALIBRATING_BARO_CYCLES             200


typedef struct flags_t {
    uint8_t OK_TO_ARM;
    uint8_t ARMED;
    uint8_t ACC_CALIBRATED;
    uint8_t ANGLE_MODE;
    uint8_t HORIZON_MODE;
    uint8_t MAG_MODE;
    uint8_t BARO_MODE;
    uint8_t GPS_HOME_MODE;
    uint8_t GPS_HOLD_MODE;
    uint8_t HEADFREE_MODE;
    uint8_t PASSTHRU_MODE;
    uint8_t GPS_FIX;
    uint8_t GPS_FIX_HOME;
    uint8_t SMALL_ANGLE;
    uint8_t CALIBRATE_MAG;
    uint8_t VARIO_MODE;
    uint8_t FIXED_WING;                     // set when in flying_wing or airplane mode. currently used by althold selection code
    uint8_t MOTORS_STOPPED;
    uint8_t FW_FAILSAFE_RTH_ENABLE;
    uint8_t CLIMBOUT_FW;
} flags_t;



typedef struct config_t {
    uint8_t pidController;                  // 0 = multiwii original, 1 = rewrite from http://www.multiwii.com/forum/viewtopic.php?f=8&t=3671
    //uint8_t P8[PIDITEMS];
    //uint8_t I8[PIDITEMS];
    //uint8_t D8[PIDITEMS];

    uint8_t rcRate8;
    uint8_t rcExpo8;
    uint8_t thrMid8;
    uint8_t thrExpo8;

    uint8_t rollPitchRate;
    uint8_t yawRate;

    uint8_t dynThrPID;
    uint16_t tpa_breakpoint;                // Breakpoint where TPA is activated
    int16_t mag_declination;                // Get your magnetic decliniation from here : http://magnetic-declination.com/
    int16_t angleTrim[2];                   // accelerometer trim

    // sensor-related stuff
    uint8_t acc_lpf_factor;                 // Set the Low Pass Filter factor for ACC. Increasing this value would reduce ACC noise (visible in GUI), but would increase ACC lag time. Zero = no filter
    uint8_t accz_deadband;                  // set the acc deadband for z-Axis, this ignores small accelerations
    uint8_t accxy_deadband;                 // set the acc deadband for xy-Axis
    uint8_t baro_tab_size;                  // size of baro filter array
    float baro_noise_lpf;                   // additional LPF to reduce baro noise
    float baro_cf_vel;                      // apply Complimentary Filter to keep the calculated velocity based on baro velocity (i.e. near real velocity)
    float baro_cf_alt;                      // apply CF to use ACC for height estimation
    float accz_lpf_cutoff;                  // cutoff frequency for the low pass filter used on the acc z-axis for althold in Hz
    uint8_t acc_unarmedcal;                 // turn automatic acc compensation on/off
    uint8_t small_angle;                    // what is considered a safe angle for arming

    //uint16_t activate[CHECKBOXITEMS];       // activate switches

    // Radio/ESC-related configuration
    uint8_t deadband;                       // introduce a deadband around the stick center for pitch and roll axis. Must be greater than zero.
    uint8_t yawdeadband;                    // introduce a deadband around the stick center for yaw axis. Must be greater than zero.
    uint8_t alt_hold_throttle_neutral;      // defines the neutral zone of throttle stick during altitude hold, default setting is +/-40
    uint8_t alt_hold_fast_change;           // when disabled, turn off the althold when throttle stick is out of deadband defined with alt_hold_throttle_neutral; when enabled, altitude changes slowly proportional to stick movement
    uint16_t throttle_correction_angle;     // the angle when the throttle correction is maximal. in 0.1 degres, ex 225 = 22.5 ,30.0, 450 = 45.0 deg
    uint8_t throttle_correction_value;      // the correction that will be applied at throttle_correction_angle.

    // Servo-related stuff
    //servoParam_t servoConf[MAX_SERVOS];     // servo configuration

    // Failsafe related configuration
    uint8_t failsafe_delay;                 // Guard time for failsafe activation after signal lost. 1 step = 0.1sec - 1sec in example (10)
    uint8_t failsafe_off_delay;             // Time for Landing before motors stop in 0.1sec. 1 step = 0.1sec - 20sec in example (200)
    uint16_t failsafe_throttle;             // Throttle level used for landing - specify value between 1000..2000 (pwm pulse width for slightly below hover). center throttle = 1500.
    uint16_t failsafe_detect_threshold;     // Update controls channel only if pulse is above failsafe_detect_threshold. below this trigger failsafe.

    // mixer-related configuration
    int8_t yaw_direction;
    uint8_t tri_unarmed_servo;              // send tail servo correction pulses even when unarmed

    // gimbal-related configuration
    uint8_t gimbal_flags;                   // in servotilt mode, various things that affect stuff

    // gps-related stuff
    uint16_t gps_wp_radius;                 // if we are within this distance to a waypoint then we consider it reached (distance is in cm)
    uint8_t gps_lpf;                        // Low pass filter cut frequency for derivative calculation (default 20Hz)
    uint8_t nav_slew_rate;                  // Adds a rate control to nav output, will smoothen out nav angle spikes
    uint8_t nav_controls_heading;           // copter faces toward the navigation point, maghold must be enabled for it
    uint16_t nav_speed_min;                 // cm/sec
    uint16_t nav_speed_max;                 // cm/sec
    uint16_t ap_mode;                       // Temporarily Disables GPS_HOLD_MODE to be able to make it possible to adjust the Hold-position when moving the sticks, creating a deadspan for GPS

    float fw_roll_throw;
    float fw_pitch_throw;
    uint8_t fw_vector_trust;
    uint8_t fw_flaperons_invert;
    int16_t fw_gps_maxcorr;                    // Degrees banking Allowed by GPS.
    int16_t fw_gps_rudder;                     // Maximum Rudder
    int16_t fw_gps_maxclimb;                   // Degrees climbing . To much can stall the plane.
    int16_t fw_gps_maxdive;                    // Degrees Diving . To much can overspeed the plane.
    uint16_t fw_climb_throttle;                // Max allowed throttle in GPS modes.
    uint16_t fw_cruise_throttle;               // Throttle to set for cruisespeed.
    uint16_t fw_idle_throttle;                 // Lowest throttleValue during Descend
    uint16_t fw_scaler_throttle;               // Adjust to Match Power/Weight ratio of your model
    float fw_roll_comp;

} config_t;



// System-wide
typedef struct master_t {
    uint8_t version;
    uint16_t size;
    uint8_t magic_be;                       // magic number, should be 0xBE

    uint8_t mixerConfiguration;
    uint32_t enabledFeatures;
    uint16_t looptime;                      // imu loop time in us
    uint8_t emf_avoidance;                  // change pll settings to avoid noise in the uhf band
    //motorMixer_t customMixer[MAX_MOTORS];   // custom mixtable
    //servoMixer_t customServoMixer[MAX_SERVO_RULES]; // custom servo mixtable

    // motor/esc/servo related stuff
    uint16_t minthrottle;                   // Set the minimum throttle command sent to the ESC (Electronic Speed Controller). This is the minimum value that allow motors to run at a idle speed.
    uint16_t maxthrottle;                   // This is the maximum value for the ESCs at full power this value can be increased up to 2000
    uint16_t mincommand;                    // This is the value for the ESCs when they are not armed. In some cases, this value must be lowered down to 900 for some specific ESCs
    uint16_t deadband3d_low;                // min 3d value
    uint16_t deadband3d_high;               // max 3d value
    uint16_t neutral3d;                     // center 3d value
    uint16_t deadband3d_throttle;           // default throttle deadband from MIDRC
    uint16_t motor_pwm_rate;                // The update rate of motor outputs (50-498Hz)
    uint16_t servo_pwm_rate;                // The update rate of servo outputs (50-498Hz)
    uint8_t pwm_filter;                     // Hardware filter for incoming PWM pulses (larger = more filtering)

    // global sensor-related stuff
    sensor_align_e gyro_align;              // gyro alignment
    sensor_align_e acc_align;               // acc alignment
    sensor_align_e mag_align;               // mag alignment
    int16_t board_align_roll;               // board alignment correction in roll (deg)
    int16_t board_align_pitch;              // board alignment correction in pitch (deg)
    int16_t board_align_yaw;                // board alignment correction in yaw (deg)
    int8_t yaw_control_direction;           // change control direction of yaw (inverted, normal)
    uint8_t acc_hardware;                   // Which acc hardware to use on boards with more than one device
    uint8_t mag_hardware;                   // Which mag hardware to use
    uint16_t gyro_lpf;                      // gyro LPF setting - values are driver specific, in case of invalid number, a reasonable default ~30-40HZ is chosen.
    uint16_t gyro_cmpf_factor;              // Set the Gyro Weight for Gyro/Acc complementary filter. Increasing this value would reduce and delay Acc influence on the output of the filter.
    uint16_t gyro_cmpfm_factor;             // Set the Gyro Weight for Gyro/Magnetometer complementary filter. Increasing this value would reduce and delay Magnetometer influence on the output of the filter
    uint8_t moron_threshold;                // people keep forgetting that moving model while init results in wrong gyro offsets. and then they never reset gyro. so this is now on by default.
    uint16_t max_angle_inclination;         // max inclination allowed in angle (level) mode. default 500 (50 degrees).
    int16_t accZero[3];
    int16_t magZero[3];

    // Safety features
    uint8_t auto_disarm_board;              // Disarm board when motors not spinning at armed enabled (0 = disabled, 1 - 60 seconds when to automatically disarm)

    // Battery/ADC stuff
    uint16_t currentscale;                  // scale the current sensor output voltage to milliamps. Value in 1/10th mV/A
    uint16_t currentoffset;                 // offset of the current sensor in millivolt steps
    uint8_t multiwiicurrentoutput;          // if set to 1 output the amperage in milliamp steps instead of 0.01A steps via msp
    uint8_t vbatscale;                      // adjust this to match battery voltage to reported value
    uint8_t vbatmaxcellvoltage;             // maximum voltage per cell, used for auto-detecting battery voltage in 0.1V units, default is 43 (4.3V)
    uint8_t vbatmincellvoltage;             // minimum voltage per cell, this triggers FASTER battery out alarm, in 0.1V units, default is 33 (3.3V)
    uint8_t vbatwarningcellvoltage;         // minimum voltage per cell, this triggers SLOWER battery out alarm, in 0.1V units, default is 35 (3.5V)
    uint8_t power_adc_channel;              // which channel is used for current sensor. Right now, only 3 places are supported: RC_CH2 (unused when in CPPM mode, = 1), RC_CH8 (last channel in PWM mode, = 9), ADC_EXTERNAL_PAD (Rev5 only, = 5), 0 to disable

    // Radio/ESC-related configuration
    uint8_t rcmap[8];                       // mapping of radio channels to internal RPYTA+ order
    uint8_t serialrx_type;                  // type of UART-based receiver (0 = spek 10, 1 = spek 11, 2 = sbus). Must be enabled by FEATURE_SERIALRX first.
    uint8_t spektrum_sat_bind;              // Spektrum satellite bind. 0 - 10 (0 = disabled)
    uint8_t spektrum_sat_on_flexport;       // Spektrum satellite on USART3 (flexport, available with rev5sp hardware)
    uint16_t midrc;                         // Some radios have not a neutral point centered on 1500. can be changed here
    uint16_t mincheck;                      // minimum rc end
    uint16_t maxcheck;                      // maximum rc end
    uint8_t retarded_arm;                   // allow disarsm/arm on throttle down + roll left/right
    uint8_t disarm_kill_switch;             // AUX disarm independently of throttle value
    int8_t fw_althold_dir;                  // +1 or -1 for pitch/althold gain. later check if need more than just sign
    uint8_t rssi_aux_channel;               // Read rssi from channel. 1+ = AUX1+, 0 to disable.
    uint8_t rssi_adc_channel;               // Read analog-rssi from RC-filter (RSSI-PWM to RSSI-Analog), RC_CH2 (unused when in CPPM mode, = 1), RC_CH8 (last channel in PWM mode, = 9), ADC_EXTERNAL_PAD (Rev5 only, = 5), 0 to disable (disabled if rssi_aux_channel > 0 or rssi_adc_channel == power_adc_channel)
    uint16_t rssi_adc_max;                  // max input voltage defined by RC-filter (is RSSI never 100% reduce the value) (1...4095)
    uint16_t rssi_adc_offset;               // input offset defined by RC-filter (0...4095)

    // gps-related stuff
    uint8_t gps_type;                       // See GPSHardware enum.
    int8_t gps_baudrate;                    // See GPSBaudRates enum.
    int8_t gps_ubx_sbas;                    // UBX SBAS setting.  -1 = disabled, 0 = AUTO, 1 = EGNOS, 2 = WAAS, 3 = MSAS, 4 = GAGAN (default = 0 = AUTO)
    uint8_t gps_autobaud;                   // GPS autobaud setting. When enabled GPS baud rate will be lowered if there are timeout. 0 = disabled, 1 = enabled


    uint32_t serial_baudrate;               // primary serial (MSP) port baudrate

    uint32_t softserial_baudrate;           // shared by both soft serial ports
    uint8_t softserial_1_inverted;          // use inverted softserial input and output signals on port 1
    uint8_t softserial_2_inverted;          // use inverted softserial input and output signals on port 2

    uint8_t telemetry_provider;             // See TelemetryProvider enum.
    uint8_t telemetry_port;                 // See TelemetryPort enum.
    uint8_t telemetry_switch;               // Use aux channel to change serial output & baudrate( MSP / Telemetry ). It disables automatic switching to Telemetry when armed.
    //config_t profile[3];                    // 3 separate profiles
    uint8_t current_profile;                // currently loaded profile
    uint8_t reboot_character;               // which byte is used to reboot. Default 'R', could be changed carefully to something else.

    uint8_t magic_ef;                       // magic number, should be 0xEF
    uint8_t chk;                            // XOR checksum
} master_t;



// Core runtime settings
typedef struct core_t {
    serialPort_t *mainport;
    serialPort_t *flexport;
    serialPort_t *gpsport;
    serialPort_t *telemport;
    serialPort_t *rcvrport;
    uint8_t mpu6050_scale;                  // es/non-es variance between MPU6050 sensors, half my boards are mpu6000ES, need this to be dynamic. automatically set by mpu6050 driver.
    uint8_t numRCChannels;                  // number of rc channels as reported by current input driver
    bool useServo;                          // feature SERVO_TILT or wing/airplane mixers will enable this
    uint8_t numServos;                      // how many total hardware servos we have. used by mixer
} core_t;



extern core_t core;
extern config_t cfg;
extern master_t mcfg;
extern flags_t f;
extern serialPort_t *g_mainport;
extern uint32_t currentTime;

extern int16_t gyroZero[3];
extern int16_t gyroData[3];
extern int16_t angle[2];
extern float anglerad[2];    // absolute angle inclination in radians

extern sensor_t acc;
extern sensor_t gyro;

extern uint16_t acc_1G;
extern int16_t heading, magHold;
extern int16_t gyroADC[3], accADC[3], accSmooth[3], magADC[3];
extern int32_t accSum[3];


void serialInit(uint32_t baudrate);
void serialCom(void);
void serialize8(uint8_t a);
void serialize16(int16_t a);


// IMU
void imuInit(void);
void annexCode(void);
void computeIMU(void);
//void blinkLED(uint8_t num, uint8_t wait, uint8_t repeat);
int getEstimatedAltitude(void);


// Sensors
bool sensorsAutodetect(void);
void ACC_getADC(void);
void Gyro_getADC(void);
void Mag_init(void);
int Mag_getADC(void);




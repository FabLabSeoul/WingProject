//#include "board.h"
//#include "mw.h"
#include "global.h"


int16_t gyroADC[3], accADC[3], accSmooth[3], magADC[3];
int32_t accSum[3];
uint32_t accTimeSum = 0;        // keep track for integration of acc
int accSumCount = 0;
int16_t smallAngle = 0;
int32_t baroPressure = 0;
int32_t baroTemperature = 0;
uint32_t baroPressureSum = 0;
int32_t BaroAlt = 0;
float sonarTransition = 0;
int32_t baroAlt_offset = 0;
int32_t sonarAlt = -1;         // in cm , -1 indicate sonar is not in range
int32_t EstAlt;                // in cm
int32_t BaroPID = 0;
int32_t AltHold;
int32_t setVelocity = 0;
uint8_t velocityControl = 0;
int32_t errorVelocityI = 0;
int32_t vario = 0;                      // variometer in cm/s
int16_t throttleAngleCorrection = 0;    // correction of throttle in lateral wind,
float magneticDeclination = 0.0f;       // calculated at startup from config
float accVelScale;
float throttleAngleScale;
float fc_acc;

// **************
// gyro+acc IMU
// **************
int16_t gyroData[3] = { 0, 0, 0 };
int16_t gyroZero[3] = { 0, 0, 0 };
int16_t angle[2] = { 0, 0 };     // absolute angle inclination in multiple of 0.1 degree    180 deg = 1800
float anglerad[2] = { 0.0f, 0.0f };    // absolute angle inclination in radians

static void getEstimatedAttitude(void);


void imuInit(void)
{
    smallAngle = lrintf(acc_1G * cosf(RAD * cfg.small_angle));
    accVelScale = 9.80665f / acc_1G / 10000.0f;
    throttleAngleScale = (1800.0f / M_PI) * (900.0f / cfg.throttle_correction_angle);

    fc_acc = 0.5f / (M_PI * cfg.accz_lpf_cutoff); // calculate RC time constant used in the accZ lpf

    Mag_init();
}

void computeIMU(void)
{
 //   static int16_t gyroYawSmooth = 0;
		//int16_t accelgyro[6];

    Gyro_getADC();
//    if (sensors(SENSOR_ACC)) 
		{
        ACC_getADC();
        getEstimatedAttitude();
    } 
//		else 
//		{
 //       accADC[X] = 0;
 //       accADC[Y] = 0;
 //       accADC[Z] = 0;
 //   }

//    if (mcfg.mixerConfiguration == MULTITYPE_TRI) {
//        gyroData[YAW] = (gyroYawSmooth * 2 + gyroADC[YAW]) / 3;
//        gyroYawSmooth = gyroData[YAW];
//    } else {
        gyroData[YAW] = gyroADC[YAW];
//    }
    gyroData[ROLL] = gyroADC[ROLL];
    gyroData[PITCH] = gyroADC[PITCH];
		
		
		
		//accelgyro[ 0] = accADC[ 0];
		//accelgyro[ 1] = accADC[ 1];
		//accelgyro[ 2] = accADC[ 2];
		//accelgyro[ 3] = gyroADC[ 0];
		//accelgyro[ 4] = gyroADC[ 1];
		//accelgyro[ 5] = gyroADC[ 2];		
		//SendSerialAccelGryro(accelgyro);
		
}

// **************************************************
// Simplified IMU based on "Complementary Filter"
// Inspired by http://starlino.com/imu_guide.html
//
// adapted by ziss_dm : http://www.multiwii.com/forum/viewtopic.php?f=8&t=198
//
// The following ideas was used in this project:
// 1) Rotation matrix: http://en.wikipedia.org/wiki/Rotation_matrix
//
// Currently Magnetometer uses separate CF which is used only
// for heading approximation.
//
// **************************************************

#define INV_GYR_CMPF_FACTOR   (1.0f / ((float)mcfg.gyro_cmpf_factor + 1.0f))
#define INV_GYR_CMPFM_FACTOR  (1.0f / ((float)mcfg.gyro_cmpfm_factor + 1.0f))

typedef struct fp_vector {
    float X;
    float Y;
    float Z;
} t_fp_vector_def;

typedef union {
    float A[3];
    t_fp_vector_def V;
} t_fp_vector;

t_fp_vector EstG;

// Normalize a vector
void normalizeV(struct fp_vector *src, struct fp_vector *dest)
{
    float length;

    length = sqrtf(src->X * src->X + src->Y * src->Y + src->Z * src->Z);
    if (length != 0) {
        dest->X = src->X / length;
        dest->Y = src->Y / length;
        dest->Z = src->Z / length;
    }
}

// Rotate Estimated vector(s) with small angle approximation, according to the gyro data
void rotateV(struct fp_vector *v, float *delta)
{
    struct fp_vector v_tmp = *v;

    // This does a  "proper" matrix rotation using gyro deltas without small-angle approximation
    float mat[3][3];
    float cosx, sinx, cosy, siny, cosz, sinz;
    float coszcosx, sinzcosx, coszsinx, sinzsinx;

    cosx = cosf(delta[ROLL]);
    sinx = sinf(delta[ROLL]);
    cosy = cosf(delta[PITCH]);
    siny = sinf(delta[PITCH]);
    cosz = cosf(delta[YAW]);
    sinz = sinf(delta[YAW]);

    coszcosx = cosz * cosx;
    sinzcosx = sinz * cosx;
    coszsinx = sinx * cosz;
    sinzsinx = sinx * sinz;

    mat[0][0] = cosz * cosy;
    mat[0][1] = -cosy * sinz;
    mat[0][2] = siny;
    mat[1][0] = sinzcosx + (coszsinx * siny);
    mat[1][1] = coszcosx - (sinzsinx * siny);
    mat[1][2] = -sinx * cosy;
    mat[2][0] = (sinzsinx) - (coszcosx * siny);
    mat[2][1] = (coszsinx) + (sinzcosx * siny);
    mat[2][2] = cosy * cosx;

    v->X = v_tmp.X * mat[0][0] + v_tmp.Y * mat[1][0] + v_tmp.Z * mat[2][0];
    v->Y = v_tmp.X * mat[0][1] + v_tmp.Y * mat[1][1] + v_tmp.Z * mat[2][1];
    v->Z = v_tmp.X * mat[0][2] + v_tmp.Y * mat[1][2] + v_tmp.Z * mat[2][2];
}

int32_t applyDeadband(int32_t value, int32_t deadband)
{
    if (abs(value) < deadband) {
        value = 0;
    } else if (value > 0) {
        value -= deadband;
    } else if (value < 0) {
        value += deadband;
    }
    return value;
}

// rotate acc into Earth frame and calculate acceleration in it
void acc_calc(uint32_t deltaT)
{
    static int32_t accZoffset = 0;
    static float accz_smooth = 0;
    float dT = 0;
    float rpy[3];
    t_fp_vector accel_ned;

    // deltaT is measured in us ticks
    dT = (float)deltaT * 1e-6f;

    // the accel values have to be rotated into the earth frame
    rpy[0] = -(float)anglerad[ROLL];
    rpy[1] = -(float)anglerad[PITCH];
    rpy[2] = -(float)heading * RAD;

    accel_ned.V.X = accSmooth[0];
    accel_ned.V.Y = accSmooth[1];
    accel_ned.V.Z = accSmooth[2];

    rotateV(&accel_ned.V, rpy);

    if (cfg.acc_unarmedcal == 1) {
        if (!f.ARMED) {
            accZoffset -= accZoffset / 64;
            accZoffset += accel_ned.V.Z;
        }
        accel_ned.V.Z -= accZoffset / 64;  // compensate for gravitation on z-axis
    } else
        accel_ned.V.Z -= acc_1G;

    accz_smooth = accz_smooth + (dT / (fc_acc + dT)) * (accel_ned.V.Z - accz_smooth); // low pass filter

    // apply Deadband to reduce integration drift and vibration influence and
    // sum up Values for later integration to get velocity and distance
    accSum[X] += applyDeadband(lrintf(accel_ned.V.X), cfg.accxy_deadband);
    accSum[Y] += applyDeadband(lrintf(accel_ned.V.Y), cfg.accxy_deadband);
    accSum[Z] += applyDeadband(lrintf(accz_smooth), cfg.accz_deadband);

    accTimeSum += deltaT;
    accSumCount++;
}

void accSum_reset(void)
{
    accSum[0] = 0;
    accSum[1] = 0;
    accSum[2] = 0;
    accSumCount = 0;
    accTimeSum = 0;
}

// baseflight calculation by Luggi09 originates from arducopter
static int16_t calculateHeading(t_fp_vector *vec)
{
    int16_t head;

    float cosineRoll = cosf(anglerad[ROLL]);
    float sineRoll = sinf(anglerad[ROLL]);
    float cosinePitch = cosf(anglerad[PITCH]);
    float sinePitch = sinf(anglerad[PITCH]);
    float Xh = vec->A[X] * cosinePitch + vec->A[Y] * sineRoll * sinePitch + vec->A[Z] * sinePitch * cosineRoll;
    float Yh = vec->A[Y] * cosineRoll - vec->A[Z] * sineRoll;
    float hd = (atan2f(Yh, Xh) * 1800.0f / M_PI + magneticDeclination) / 10.0f;
    head = lrintf(hd);
    if (head < 0)
        head += 360;

    return head;
}

static void getEstimatedAttitude(void)
{
    int32_t axis;
    int32_t accMag = 0;
    static t_fp_vector EstM;
    static t_fp_vector EstN = { .A = { 1.0f, 0.0f, 0.0f } };
    static float accLPF[3];
    static uint32_t previousT;
    uint32_t currentT = micros();
    uint32_t deltaT;
    float scale, deltaGyroAngle[3];
    deltaT = currentT - previousT;
    scale = deltaT * gyro.scale;
	
	// TO: dont understand
	// �ð��� �ʱ�ȭ �Ǵ� ���� ������ �߰���.
	// ������ ���� �𸣰���.
	// ����ð��� �� �ð� ���̰� �ʹ� ũ�� �������� �ʴ´�.
	if (currentT - previousT > 10000) // 10ms over
	{
	    previousT = currentT;
		return;
	}

    previousT = currentT;	
	
    // Initialization
	for (axis = 0; axis < 3; axis++)
	{
        deltaGyroAngle[axis] = gyroADC[axis] * scale;
        if (cfg.acc_lpf_factor > 0) 
		{
            accLPF[axis] = accLPF[axis] * (1.0f - (1.0f / cfg.acc_lpf_factor)) + accADC[axis] * (1.0f / cfg.acc_lpf_factor);
            accSmooth[axis] = accLPF[axis];
        } 
		else 
		{
            accSmooth[axis] = accADC[axis];
        }
				
        accMag += (int32_t)accSmooth[axis] * accSmooth[axis];
    }
    accMag = accMag * 100 / ((int32_t)acc_1G * acc_1G);

    rotateV(&EstG.V, deltaGyroAngle);

    // Apply complimentary filter (Gyro drift correction)
    // If accel magnitude >1.15G or <0.85G and ACC vector outside of the limit range => we neutralize the effect of accelerometers in the angle estimation.
    // To do that, we just skip filter, as EstV already rotated by Gyro
    if (72 < (uint16_t)accMag && (uint16_t)accMag < 133) {
        for (axis = 0; axis < 3; axis++)
            EstG.A[axis] = (EstG.A[axis] * (float)mcfg.gyro_cmpf_factor + accSmooth[axis]) * INV_GYR_CMPF_FACTOR;
    }

    f.SMALL_ANGLE = (EstG.A[Z] > smallAngle);

    // Attitude of the estimated vector
    anglerad[ROLL] = atan2f(EstG.V.Y, EstG.V.Z);
    anglerad[PITCH] = atan2f(-EstG.V.X, sqrtf(EstG.V.Y * EstG.V.Y + EstG.V.Z * EstG.V.Z));
    angle[ROLL] = lrintf(anglerad[ROLL] * (1800.0f / M_PI));
    angle[PITCH] = lrintf(anglerad[PITCH] * (1800.0f / M_PI));

//    if (sensors(SENSOR_MAG)) 
	if (0)
	{
        rotateV(&EstM.V, deltaGyroAngle);
       for (axis = 0; axis < 3; axis++)
            EstM.A[axis] = (EstM.A[axis] * (float)mcfg.gyro_cmpfm_factor + magADC[axis]) * INV_GYR_CMPFM_FACTOR;
       heading = calculateHeading(&EstM);
    } 
	else 
	{
        rotateV(&EstN.V, deltaGyroAngle);
        normalizeV(&EstN.V, &EstN.V);
        heading = calculateHeading(&EstN);
	}

    acc_calc(deltaT); // rotate acc vector into earth frame

    if (cfg.throttle_correction_value) 
	{
        float cosZ = EstG.V.Z / sqrtf(EstG.V.X * EstG.V.X + EstG.V.Y * EstG.V.Y + EstG.V.Z * EstG.V.Z);

        if (cosZ <= 0.015f) 
		{ // we are inverted, vertical or with a small angle < 0.86 deg
            throttleAngleCorrection = 0;
        } 
		else 
		{
            int deg = lrintf(acosf(cosZ) * throttleAngleScale);
            if (deg > 900)
                deg = 900;
            throttleAngleCorrection = lrintf(cfg.throttle_correction_value * sinf(deg / (900.0f * M_PI / 2.0f)));
        }
    }		
		
	//printf( "Radian, %d, %d, %d, %d \n", millis(), angle[0], angle[1], heading );	
}



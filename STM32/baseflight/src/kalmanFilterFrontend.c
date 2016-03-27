/*
 * kalman.c
 *
 *  Created on: 25.11.2012
 *      Author: robert
 *
 *      acts more as a filter front end for testing :)
 */

#include <stdint.h>
#include "board.h"
#include "kalmanFilterFrontend.h"
#include "kalman.h"
//#include "barofilter.h"

// accelerometer
KalmanFilter kax;
KalmanFilter kay;
KalmanFilter kaz;

// gyro
KalmanFilter kgx;
KalmanFilter kgy;
KalmanFilter kgz;

// could create two filters - but the
KalmanFilter alloc_filter_1d(float P, float Q, float R)
{
	KalmanFilter f = alloc_filter(1, 1);

	set_identity_matrix(&f.state_transition);

	/* We observe (x) in each time step */
	{
		const float values[] = { 1.0, 1.0, 0.0 };
		set_matrix(&f.observation_model, sizeof(values) / sizeof(values[0]), values);
	}

	/* Noise in the world. */
	set_identity_matrix_value(&f.process_noise_covariance, Q);

	/* Noise in our observation */
	set_identity_matrix_value(&f.observation_noise_covariance, R);

	/* The start position is totally unknown, so give a high variance */
	scale_matrix(&f.state_estimate, 0.0);
	set_identity_matrix_value(&f.estimate_covariance, P);

	return f;
}

int16_t kalman_filter_step(KalmanFilter *f, int16_t val)
{
		const float values[] = { val};
		set_matrix(&f->observation, sizeof(values) / sizeof(values[0]), values);
		update(f);
		// the position will be returned
		// correct possible rounding issue
		return (int16_t) (f->state_estimate.data[0][0] + 0.5f);
}


static void initKalmanGyro(int16_t gyros[3])
{
	// real bad on my small jakub frame
//#define Q 0.0625 // process noise covariance
//#define	R 4.0	// measurement noise covariance
//#define P 0.47	// estimation error covariance

// small jakub frame
//#define Q 1.0 	// process noise covariance
//#define	R 0.01	// measurement noise covariance
//#define P 0.22	// estimation error covariance

	const float p = 0.22;
	const float q = 10.0;
	const float r = 0.1;

	kgx = alloc_filter_1d(p, q, r);
	kgy = alloc_filter_1d(p, q, r);
	kgz = alloc_filter_1d(p, q, r);
}

static void initKalmanAccel(int16_t acc[3])
{
	// small jakub frame
//#define Q 0.0625		// process noise covariance
//#define	R 1.0		// measurement noise covariance
//#define P 0.22		// estimation error covariance
	const float p = 0.22;
	const float q = 0.1;
	const float r = 10.0;

	kax = alloc_filter_1d(p, q, r);
	kay = alloc_filter_1d(p, q, r);
	kaz = alloc_filter_1d(p, q, r);
}


void accelKalmanfilterStep(int16_t acc[3])
{
	static int _init = 0;
	if (!_init)
	{
		_init = 1;
		initKalmanAccel(acc);
	}
	else
	{
		acc[0] = (int16_t)kalman_filter_step(&kax, acc[0]);
		acc[1] = (int16_t)kalman_filter_step(&kay, acc[1]);
		acc[2] = (int16_t)kalman_filter_step(&kaz, acc[2]);
	}
}

void gyroKalmanfilterStep(int16_t gyros[3])
{
	static int _init = 0;
	if (!_init)
	{
		_init = 1;
		initKalmanGyro(gyros);
	}
	else
	{
		gyros[0] = (int16_t)kalman_filter_step(&kgx, gyros[0]);
		gyros[1] = (int16_t)kalman_filter_step(&kgy, gyros[1]);
		gyros[2] = (int16_t)kalman_filter_step(&kgz, gyros[2]);
	}
}

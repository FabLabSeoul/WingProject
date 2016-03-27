/*
 * kalman.h
 *
 *  Created on: 25.11.2012
 *      Author: rob
 */

#ifndef KALMAN_H_
#define KALMAN_H_

void initKalmanfilters(void);

void accelKalmanfilterStep(int16_t acc[3]);
void gyroKalmanfilterStep(int16_t gyros[3]);
void baroKalmanfilterStep(int32_t *baro);

#endif /* KALMAN_H_ */

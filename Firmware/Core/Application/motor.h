/*
 * motor.h
 *
 *  Created on: Jan 14, 2025
 *      Author: Riki
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Motor driver initialization
 */
void motor_init(void);

/**
 * @brief Motor driver regular job
 */
void motor_job(void);

/**
 * @brief Motor driver initialization
 * @param power Motor power -1000 (rewind max.) .. 0 (stop) .. 1000 (forward max.)
 */
void motor_set(int32_t power);

/* ---------------------------------------------------------------------------*/

#endif /* _H_ */

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
 * @param periods Period count or zero for infinity
 */
void motor_set(int32_t power, uint32_t periods);

/**
 * @brief Driver period counter update
 */
void motor_update(void);

/**
 * @brief Driver input data update callback
 */
__weak void motor_update_callback(void);

/* ---------------------------------------------------------------------------*/

#endif /* _H_ */

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

/* Defines -------------------------------------------------------------------*/

/* Maximal driver output voltage equivalent */
#define MOT_VOLT_MAX                      1000

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Motor driver initialization
 * @param filter_length Length of the request filter buffer
 */
void motor_init(uint16_t filter_length);

/**
 * @brief Motor driver regular job
 */
void motor_job(void);

/**
 * @brief Motor driver timer interrupt handler
 */
void motor_irq_handler(void);

/**
 * @brief Set motor driver output voltage equivalent
 * @param voltage Output voltage -MOT_VOLT_MAX .. MOT_VOLT_MAX or 0 for motor stop
 */
void motor_set_voltage(int16_t voltage);

/**
 * @brief Start and stop the motor
 * @param run True to start the motor, False to stop
 */
void motor_control(uint8_t run);

/**
 * @brief Return the motor running state
 * @return True if motor is running
 */
uint8_t is_motor_running(void);

/**
 * @brief Return the actual filter length set
 * @return Actual filter length
 */
uint16_t get_filter_length(void);

/**
 * @brief Driver period counter update
 */
void motor_update(void);

/**
 * @brief Driver input data update callback
 * @return Next sample of motor driver signal
 */
int16_t motor_update_callback(void);

/* ---------------------------------------------------------------------------*/

#endif /* _H_ */

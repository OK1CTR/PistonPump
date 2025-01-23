/*
 * top.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Riki
 */

#ifndef _TOP_H_
#define _TOP_H_

/* Includes ------------------------------------------------------------------*/

#include <stdint.h>

#include "common.h"

/* Defines -------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/**
 * @brief Initialize the top level module
 */
void top_init(void);

/**
 * @brief Top level module regular job
 */
void top_job(void);

/**
 * @brief Driver input data update callback
 */
int16_t motor_update_callback(void);

/**
 * @brief Read programmable wave buffer item
 * @param address Item address relative to buffer begin
 * @param value Pointer for storing the value of the item
 *
 */
Status_t prog_wave_read(uint16_t address, uint16_t *value);

/**
 * @brief Write programmable wave buffer item
 * @param address Item address relative to buffer begin
 * @param value Item value
 * @return STATE_OK if succeed
 */
Status_t prog_wave_write(uint16_t address, uint16_t value);

/* ---------------------------------------------------------------------------*/

#endif /* _TOP_H_ */

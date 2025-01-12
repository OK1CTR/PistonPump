/*
 * common.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Riki
 */

#ifndef APPLICATION_COMMON_H_
#define APPLICATION_COMMON_H_

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Defines -------------------------------------------------------------------*/

/**
 * Success status
 */
#define STATUS_OK        0
/**
 * Error or fail status
 */
#define STATUS_ERROR     1

/**
 * Error or fail status
 */
#define STATUS_TIMEOUT   2

/**
 * Busy status
 */
#define STATUS_BUSY      3

/**
 * Saturate the X to the given upper bound VAL
 */
#define SAT_UP(x, val)     ((x) = ((x)>(val))?(val):(x))

/**
 * Saturate the X to the given lower bound VAL
 */
#define SAT_DOWN(x, val)   ((x) = ((x)<(val))?(val):(x))

#ifndef __weak
    #define __weak   __attribute__((weak))
#endif /* __weak */

/**
 * Macro to check 32-bit unsigned tick timeout constant
 */
#define TICK_EXPIRED(a)    (HAL_GetTick() - (a) < 0x7fffffff)

/**
 * General status return type
 */
typedef int16_t Status_t;

/* ---------------------------------------------------------------------------*/

#endif

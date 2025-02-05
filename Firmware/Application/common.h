/**
 * @file       common.h
 * @author     OK1CTR
 * @date       Jan 21, 2025
 * @brief      Common definitions and utilities
 *
 * @addtogroup grCommon
 * @{
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/* Includes ------------------------------------------------------------------*/

#include "main.h"

/* Defines -------------------------------------------------------------------*/

/*! Success status */
#define STATUS_OK          0

/*! Error or fail status */
#define STATUS_ERROR       1

/*! Error or fail status */
#define STATUS_TIMEOUT     2

/*! Busy status */
#define STATUS_BUSY        3

/* Macros --------------------------------------------------------------------*/

/*! Saturate the X to the given upper bound VAL */
#define SAT_UP(x, val)     ((x) = ((x)>(val))?(val):(x))

/*! Saturate the X to the given lower bound VAL */
#define SAT_DOWN(x, val)   ((x) = ((x)<(val))?(val):(x))

/*! Macro to check 32-bit unsigned tick timeout constant */
#define TICK_EXPIRED(a)    (HAL_GetTick() - (a) < 0x7fffffff)

/* Attributes ----------------------------------------------------------------*/

#ifndef __weak
    #define __weak         __attribute__((weak))
#endif /* __weak */

/* Typedefs ------------------------------------------------------------------*/

/*! General status return type */
typedef int16_t Status_t;

/* ---------------------------------------------------------------------------*/

#endif /* _COMMON_H_ */

/** @} */

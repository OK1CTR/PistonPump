/*
 * mb_rtu_app.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Riki
 */

#ifndef MB_RTU_APP_H_
#define MB_RTU_APP_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/


/**
 * Return value for registers where read is not permitted
 */
#define MB_READ_NOT_SUPPORTED         0xFFFF

/**
 * Bulk data register count in holding registers space
 */
#define MB_HOLDING_BULK_NUM               16

/**
 * Bulk data address offset in holding registers space
 */
#define MB_HOLDING_BULK_OFFSET           100


/** @defgroup grMbRegInput
 *  @ingroup grMbRtuApp
 *  @brief Input register addresses
 *  @{
 */

#define MB_INPUT_FIRST                     0

#define MB_STATE                           0u
#define MB_ERROR                           1u
#define MB_IN_RES_1                        2u
#define MB_IN_RES_2                        3u

#define MB_INPUT_LAST                      3

/** @} */


/** @defgroup grMbRegHolding
 *  @ingroup grMbRtuApp
 *  @brief Holding register addresses
 *  @{
 */

#define MB_HOLD_FIRST                      0

#define MB_COMMAND                         0u
#define MB_TIME_FWD                        1u
#define MB_SPEED_FWD                       2u
#define MB_TIME_REW                        3u
#define MB_SPEED_REW                       4u
#define MB_REPEAT_CNT                      5u
#define MB_REPEAT_PERIOD                   6u

#define MB_HOLD_LAST                     115u

/** @} */

/* Typedefs-------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/**
 * Application callback for reading holding registers one by one.
 *
 * @param address Address of the register
 * @param value Pointer where register value should be stored
 * @return Status
 */
Status_t MbRtu_ReadHoldingRegCallback(uint16_t address, uint16_t *value);

/**
 * Application callback for reading input registers one by on.
 * @param address Address of the register
 * @param value Value where register value should be stored.
 * @return Status
 */
Status_t MbRtu_ReadInputRegCallback(uint16_t address, uint16_t *value);

/**
 * Application callback for writing holding register one by one
 * @param address Address of the register to write
 * @param value New value of the register
 * @return Status
 */
Status_t MbRtu_WriteHoldingRegCallback(uint16_t address, uint16_t value);

#endif /* MB_RTU_APP_H_ */
/** @} */

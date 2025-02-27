/**
 * @file       mb_rtu_app.h
 * @author     OK1CTR
 * @date       Jan 21, 2025
 * @brief      Modbus RTU server module
 *
 * @addtogroup grModServer
 * @{
 */

#ifndef MB_RTU_APP_H_
#define MB_RTU_APP_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/


/*! Return value for registers where read is not permitted */
#define MB_READ_NOT_SUPPORTED         0xFFFF

/*! Bulk data register count in holding registers space */
#define MB_HOLDING_BULK_NUM               16

/*! Bulk data address offset in holding registers space */
#define MB_HOLDING_BULK_OFFSET           100


/** @defgroup grMbRegInput
 *  @ingroup grMbRtuApp
 *  @brief Input register addresses
 *  @{
 */
#define MB_INPUT_FIRST                     0

#define MB_STATE                           0u  ///< system global state flag register
#define MB_ERROR                           1u  ///< system global error flag register
#define MB_IN_RES_1                        2u  ///< reserved
#define MB_IN_RES_2                        3u  ///< reserved

#define MB_INPUT_LAST                      3
/** @} */


/** @defgroup grMbRegHolding
 *  @ingroup grMbRtuApp
 *  @brief Holding register addresses
 *  @{
 */
#define MB_HOLD_FIRST                      0

#define MB_COMMAND                         0u  ///< operation command
#define MB_TIME_FWD                        1u  ///< maximal time of motor run forward
#define MB_SPEED_FWD                       2u  ///< motor speed (driver duty factor) for run forward
#define MB_TIME_REW                        3u  ///< maximal time of motor run back
#define MB_SPEED_REW                       4u  ///< motor speed (driver duty factor) for run back
#define MB_FILTER_LENGTH                   5u  ///< filter motor length, configure to update
#define MB_REPEAT_CNT                      6u  ///< repeat count of programmable wave
#define MB_REPEAT_PERIOD                   7u  ///< programmable wave repeat period in milliseconds

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

/* ---------------------------------------------------------------------------*/

/** @} */

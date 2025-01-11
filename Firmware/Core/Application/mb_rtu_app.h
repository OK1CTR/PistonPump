/**
 * @file       mb_rtu_app.h
 * @version    $(APP_VERSION)
 * @date       $(RELEASE_DATE)
 * @brief      Modbus RTU application implementation
 * @author     jan.bartovsky
 *
 * @copyright  Logic Elements Copyright
 *
 * @defgroup grMbRtuApp Modbus RTU server application
 * @{
 * @brief Application callbacks for Modbus RTU server
 *
 * This module contains callbacks for reading and writing registers.
 *
 * @par Main features:
 *
 */
#ifndef MB_RTU_APP_H_
#define MB_RTU_APP_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/


/**
 * Return value for registers where read is not permitted
 */
#define MB_READ_NOT_SUPPORTED             0xFFFF


/** @defgroup grMbRegInput
 *  @ingroup grMbRtuApp
 *  @brief Input register addresses
 *  @{
 */

#define MB_INPUT_FIRST                     0

#define MB_INPUT_A                         0u
#define MB_INPUT_B                         1u
#define MB_INPUT_C                         2u
#define MB_INPUT_D                         3u

#define MB_INPUT_LAST                      3

/** @} */


/** @defgroup grMbRegHolding
 *  @ingroup grMbRtuApp
 *  @brief Holding register addresses
 *  @{
 */

#define MB_HOLD_FIRST                      0

#define MB_HOLD_A                          0u
#define MB_HOLD_B                          1u
#define MB_HOLD_C                          2u
#define MB_HOLD_D                          3u

#define MB_HOLD_LAST                       3

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

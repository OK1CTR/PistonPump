/**
 * @file       modbus_slave.h
 * @author     OK1CTR
 * @date       Jan 21, 2025
 * @brief      Modbus slave over USB module
 *
 * @addtogroup grModbus
 * @{
 */

#ifndef _MODBUS_SLAVE_H_
#define _MODBUS_SLAVE_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/
/* Typedefs-------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/**
 * @brief Initialize Modbus module
 * @return Status Standard status code (0 is OK)
 */
Status_t MbSlave_Init(void);

/**
 * @brief Receive chunk of incoming message, start decoding and receive timeout
 * @param chunk_data Pointer to the data chunk buffer
 * @param chunk_len Chunk data length
 */
Status_t MbSlave_Receive(uint8_t* chunk_data, uint32_t chunk_len);

/**
 * @brief Incoming message handler
 * @return Status Standard status code (0 is OK)
 */
Status_t MbSlave_Handle(void);

/**
 * @brief Settings of modbus has changed, reinitialize module
 * @return Status Standard status code (0 is OK)
 */
Status_t MbSlave_SettingsChanged(void);

/**
 * @brief Terminate current transfer and start waiting for next packet header
 * @return Status Standard status code (0 is OK)
 */
Status_t MbSlave_BusReset(void);

/**
 * @brief Call this function to update modbus slave address
 * @return Always 0
 */
Status_t MbSlave_UpdateSlaveAddress(void);

/* ---------------------------------------------------------------------------*/

#endif /* _MODBUS_SLAVE_H_ */

/** @} */

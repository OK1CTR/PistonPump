/*
 * modbus_slave.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Riki
 */

#ifndef MODBUS_SLAVE_H_
#define MODBUS_SLAVE_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Definitions----------------------------------------------------------------*/
/* Typedefs-------------------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/**
 * Initialize Modbus module.
 *
 * @param address - modbus device address
 * @return Status - standard status code (0 - OK)
 */
Status_t MbSlave_Init(void);

/**
 *
 */
Status_t MbSlave_Receive(uint8_t* chunk_data, uint32_t chunk_len);

/**
 * Modbus packet handler.
 *
 * @return Status - standard status code (0 - OK)
 */
Status_t MbSlave_Handle(void);

/**
 * Settings of modbus has changed, reinit UART
 *
 * @return Status
 */
Status_t MbSlave_SettingsChanged(void);

/**
 * Terminate current transfer and start waiting for next packet header.
 *
 * @return Status - standard status code (0 - OK)
 */
Status_t MbSlave_BusReset(void);

/**
 * Call this function to update modbus slave address
 * @return STATUS_OK
 */
Status_t MbSlave_UpdateSlaveAddress(void);

/* ---------------------------------------------------------------------------*/

#endif /* MODBUS_SLAVE_H_ */

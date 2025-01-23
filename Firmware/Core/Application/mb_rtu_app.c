/*
 * mb_rtu_app.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Riki
 */

/* Includes ------------------------------------------------------------------*/

#include "mb_rtu_app.h"
#include "configuration.h"
#include "top.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros  -----------------------------------------------------------*/
/* Private typedefs ----------------------------------------------------------*/

/**
 * Declaration of all private variables
 */
typedef struct
{
}MbRtu_Private_t;

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
 * Instance of all private variables (except HAL handles)
 */
static MbRtu_Private_t mb;

/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Functions -----------------------------------------------------------------*/


Status_t MbRtu_ReadInputRegCallback(uint16_t address, uint16_t *value)
{
  Status_t ret = STATUS_OK;

  UNUSED(mb);

  switch (address)
  {
    case MB_STATE:
      *value = cfg.state;
      break;
    case MB_ERROR:
      *value = cfg.error;
      break;
    case MB_IN_RES_1:
      *value = 0;
      break;
    case MB_IN_RES_2:
      *value = 65535;
      break;

    default:
      *value = MB_READ_NOT_SUPPORTED;
      ret = STATUS_ERROR;
      break;
  }

  /* Reverse byte order */
  *value = __REV16(*value);

  return ret;
}


Status_t MbRtu_ReadHoldingRegCallback(uint16_t address, uint16_t *value)
{
  Status_t ret = STATUS_OK;

  // bulk registers
  if (address >= MB_HOLDING_BULK_OFFSET && address < MB_HOLDING_BULK_OFFSET + MB_HOLDING_BULK_NUM)
  {
    ret = prog_wave_read(address - MB_HOLDING_BULK_OFFSET, value);
    return ret;
  }

  // standard registers
  switch (address)
  {
    case MB_COMMAND:
      *value = cfg.command;
      break;
    case MB_TIME_FWD:
      *value = cfg.forward_time;
      break;
    case MB_SPEED_FWD:
      *value = cfg.forward_speed;
      break;
    case MB_TIME_REW:
      *value = cfg.rewind_time;
      break;
    case MB_SPEED_REW:
      *value = cfg.rewind_speed;
      break;

    default:
      *value = MB_READ_NOT_SUPPORTED;
      ret = STATUS_ERROR;
      break;
  }

  /* Reverse byte order */
  *value = __REV16(*value);

  return ret;
}


Status_t MbRtu_WriteHoldingRegCallback(uint16_t address, uint16_t value)
{
  Status_t ret = STATUS_OK;

  // bulk registers
  if (address >= MB_HOLDING_BULK_OFFSET && address < MB_HOLDING_BULK_OFFSET + MB_HOLDING_BULK_NUM)
  {
    ret = prog_wave_write(address - MB_HOLDING_BULK_OFFSET, value);
    return ret;
  }

  // standard registers
  switch (address)
  {
    case MB_COMMAND:
      cfg.command = value;
      break;
    case MB_TIME_FWD:
      cfg.forward_time = value;
      break;
    case MB_SPEED_FWD:
      cfg.forward_speed = value;
      break;
    case MB_TIME_REW:
      cfg.rewind_time = value;
      break;
    case MB_SPEED_REW:
      cfg.rewind_speed = value;
      break;

    default:
      ret = STATUS_ERROR;
      break;
  }

  return ret;
}

/* Private Functions ---------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/

/**
 * @file       mb_rtu_app.c
 * @author     OK1CTR
 * @date       Jan 21, 2025
 * @brief      Modbus RTU server module
 *
 * @addtogroup grModServer
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "mb_rtu_app.h"
#include "configuration.h"
#include "app.h"

/* Private defines -----------------------------------------------------------*/
/* Private macros  -----------------------------------------------------------*/
/* Private typedefs ----------------------------------------------------------*/

/*! Declaration of all private variables */
typedef struct
{
} MbRtu_Private_t;

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

  // reverse byte order
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
    case MB_FILTER_LENGTH:
      *value = cfg.filter_length;
      break;
    case MB_REPEAT_CNT:
      *value = cfg.repeat_count;
      break;
    case MB_REPEAT_PERIOD:
      *value = cfg.repeat_period;
      break;
    default:
      *value = MB_READ_NOT_SUPPORTED;
      ret = STATUS_ERROR;
      break;
  }

  // reverse byte order
  *value = __REV16(*value);

  return ret;
}

/*! Test upper value limit */
#define TEST_MAX(dest, src, lim) if (src <= lim) (dest) = (src); else ret = STATUS_ERROR;
/*! Test lower value limit */
#define TEST_MIN(dest, src, lim) if (src >= lim) (dest) = (src); else ret = STATUS_ERROR;

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
      TEST_MAX(cfg.forward_speed, value, CFG_SPEED_MAX)
      break;
    case MB_TIME_REW:
      cfg.rewind_time = value;
      break;
    case MB_SPEED_REW:
      TEST_MAX(cfg.rewind_speed, value, CFG_SPEED_MAX)
      break;
    case MB_FILTER_LENGTH:
      cfg.filter_length = value;
      break;
    case MB_REPEAT_CNT:
      TEST_MAX(cfg.repeat_count, value, CFG_REPEAT_CNT_MAX)
      break;
    case MB_REPEAT_PERIOD:
      cfg.repeat_period = value;
      break;
    default:
      ret = STATUS_ERROR;
      break;
  }

  return ret;
}

/* Private Functions ---------------------------------------------------------*/
/* ---------------------------------------------------------------------------*/

/** @} */

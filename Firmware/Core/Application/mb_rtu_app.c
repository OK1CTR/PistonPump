/**
 * @file       mb_rtu_app.c
 * @brief      Modbus RTU server implementation
 * @addtogroup grMbRtuApp
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include "mb_rtu_app.h"

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
    case MB_INPUT_A:
      *value = 0;  //TODO for first test
      break;
    case MB_INPUT_B:
      *value = 1;  //TODO for first test
      break;
    case MB_INPUT_C:
      *value = 2;  //TODO for first test
      break;
    case MB_INPUT_D:
      *value = 3;  //TODO for first test
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

  switch (address)
  {
    case MB_HOLD_A:
      *value = 0;  //TODO for first test
      break;
    case MB_HOLD_B:
      *value = 1;  //TODO for first test
      break;
    case MB_HOLD_C:
      *value = 2;  //TODO for first test
      break;
    case MB_HOLD_D:
      *value = 3;  //TODO for first test
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
  uint32_t id = 0;

  switch (address)
  {
    case MB_HOLD_A:
      value;  //TODO do something with value
      id = 1;  //TODO can be set after register sequence
      break;
    case MB_HOLD_B:
      value;  //TODO do something with value
      id = 1;  //TODO can be set after register sequence
      break;
    case MB_HOLD_C:
      value;  //TODO do something with value
      id = 1;  //TODO can be set after register sequence
      break;
    case MB_HOLD_D:
      value;  //TODO do something with value
      id = 1;  //TODO can be set after register sequence
      break;

    default:
      ret = STATUS_ERROR;
      break;
  }

  if (id != 0)
  {
    __NOP();  //TODO updated
  }

  return ret;
}

/* Private Functions ---------------------------------------------------------*/

/** @} */

/*
 * configuration.h
 *
 *  Created on: Jan 11, 2025
 *      Author: Riki
 */

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Typedefs ------------------------------------------------------------------*/

/* Configuration, input and output data structure */
typedef struct config
{
  uint16_t state;             ///< system global state flag register
  uint16_t error;             ///< system global error flag register
  uint16_t command;           ///< operation command
  uint16_t forward_time;      ///< maximal time of motor run forward
  uint16_t forward_speed;     ///< motor speed (driver duty factor) for run forward
  uint16_t rewind_time;       ///< maximal time of motor run back
  uint16_t rewind_speed;      ///< motor speed (driver duty factor) for run back
} config_t;

/* List of commands */
enum commands_e {
  CMD_NONE = 0,
  CMD_STEP_FORWARD,
  CMD_STEP_REWIND
};

/* Public variables ----------------------------------------------------------*/

/* Configuration, input and output data structure */
extern config_t cfg;

/* ---------------------------------------------------------------------------*/

#endif /* _CONFIGURATION_H_ */

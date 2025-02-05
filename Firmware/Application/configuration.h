/**
 * @file       configuration.c
 * @author     OK1CTR
 * @date       Jan 21, 2025
 * @brief      Application configuration module
 *
 * @addtogroup grConfig
 * @{
 */

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

/* Includes ------------------------------------------------------------------*/

#include "common.h"

/* Defines -------------------------------------------------------------------*/

/*! Default register contents */
#define CFG_DEFAULT {0, 0, 0, 1000, 1000, 1000, 1000, 100, 13, 1100}

/*! Speed maximal limit */
#define CFG_SPEED_MAX         1000
/*! Wave repeat count maximal limit */
#define CFG_REPEAT_CNT_MAX      16

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
  uint16_t filter_length;     ///< filter motor length, configure to update
  uint16_t repeat_count;      ///< repeat count of programmable wave
  uint16_t repeat_period;     ///< programmable wave repeat period in milliseconds
} config_t;

/* Public variables ----------------------------------------------------------*/

/* Configuration, input and output data structure */
extern config_t cfg;

/* Functions -----------------------------------------------------------------*/

/**
 * @brief Set configuration to defaults
 */
void config_set_defaults(void);

/**
 * @brief Save configuration to backup
 * @param user_data Pointer to user data to store with configuration
 * @param user_size Size of user data
 */
void config_save(uint8_t *user_data, uint8_t user_size);

/**
 * @brief Load configuration from backup
 * @param user_data Pointer to user data to store with configuration
 * @param user_size Size of user data
 * @return True if successful
 */
uint8_t config_load(uint8_t *user_data, uint8_t user_size);

/* ---------------------------------------------------------------------------*/

#endif /* _CONFIGURATION_H_ */

/** @} */

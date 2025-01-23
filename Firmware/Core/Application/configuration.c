/*
 * configuration.c
 *
 *  Created on: Jan 11, 2025
 *      Author: Riki
 */

/* Includes ------------------------------------------------------------------*/

#include <string.h>

#include "configuration.h"

/* Public variables ----------------------------------------------------------*/

/* Default configuration structure */
static const config_t cfg_default = CFG_DEFAULT;

/* Configuration, input and output data structure */
config_t cfg;

/* Functions -----------------------------------------------------------------*/

/* Set configuration to defaults */
void config_set_defaults(void)
{
  memcpy(&cfg, &cfg_default, sizeof(config_t));
}


/* Save configuration to backup */
void config_save(uint8_t *user_data, uint8_t user_size)
{
}


/* Load configuration from backup */
uint8_t config_load(uint8_t *user_data, uint8_t user_size)
{
  return STATUS_ERROR;
}

/* ---------------------------------------------------------------------------*/

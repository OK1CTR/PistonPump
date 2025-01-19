/*
 * top.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Riki
 */

/* Includes ------------------------------------------------------------------*/

#include "top.h"
#include "main.h"
#include "common.h"
#include "configuration.h"
#include "motor.h"

/* Private typedefs ----------------------------------------------------------*/

/* Definition of all private variables */
typedef struct
{
  uint32_t sample_count;
  int16_t sample;
} Top_Private_t;

/* Public variables ----------------------------------------------------------*/

/* All private variables */
Top_Private_t top;

/* Functions -----------------------------------------------------------------*/

/* Initialize the top level module */
void top_init(void)
{
  top.sample_count = 0;
}


/* Top level module regular job */
void top_job(void)
{
  if (cfg.command == CMD_STEP_FORWARD)
  {
    top.sample_count = cfg.forward_time;
    top.sample = MOT_VOLT_MAX;
    cfg.command = CMD_NONE;
  }
  else if (cfg.command == CMD_STEP_REWIND)
  {
    top.sample_count = cfg.rewind_time;
    top.sample = -MOT_VOLT_MAX;
    cfg.command = CMD_NONE;
  }
}


/* Driver input data update callback */
int16_t motor_update_callback(void)
{
  if (top.sample_count > 0)
  {
    top.sample_count--;
    return top.sample;
  }
  return 0;
}

/* ---------------------------------------------------------------------------*/

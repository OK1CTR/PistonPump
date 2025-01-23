/*
 * top.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Riki
 */

/* Includes ------------------------------------------------------------------*/

#include <string.h>

#include "top.h"
#include "main.h"
#include "common.h"
#include "configuration.h"
#include "motor.h"

/* Private defines -----------------------------------------------------------*/

/* Timetable length */
#define TIME_TABLE_LEN              8

/* Default timetable contents */
#define TT_DEFAULT {\
  { 1000,  150},\
  { -100,  150},\
  {    0,  200},\
  {  300,  150},\
  {    0,  150},\
  {    0,    0},\
  {    0,    0},\
  {    0,    0}}

/* Private typedefs ----------------------------------------------------------*/

/* Definition of all private variables except timetable */
typedef struct
{
  uint32_t sample_count;
  int16_t sample;
  uint8_t tt_ptr;
} Top_Private_t;

/* Timetable element definition */
typedef struct
{
  int16_t volt;               ///< element motor voltage
  uint16_t time;              ///< element duration
} TtEelem_t;

/* List of commands */
enum commands_e {
  CMD_NONE = 0,               ///< no action needed
  CMD_STEP_FORWARD,           ///< do forward step
  CMD_STEP_REWIND,            ///< do rewind step
  CMD_WAVE,                   ///< do programmed wave
  CMD_CFG_SAVE,               ///< save the configuration to backup and reinitialize system
  CMD_CFG_LOAD,               ///< load the configuration from backup and reinitialize system
  CMD_CFG_DEFAULT,            ///< restore configuration to default and reinitialize system
  CMD_STOP                    ///< emergency motor stop
};

/* Public variables ----------------------------------------------------------*/

/* All private variables except timetable */
Top_Private_t top;

/* Timetable array */
TtEelem_t tt[TIME_TABLE_LEN];

/* Programmable pulse buffer */
TtEelem_t tt_buf[TIME_TABLE_LEN];

/* Default programmable pulse */
static const TtEelem_t tt_default[TIME_TABLE_LEN] = TT_DEFAULT;

/* Private function prototypes -----------------------------------------------*/

/**
 * @brief Clear the timetable
 */
static void timetable_clear(void);

/* Functions -----------------------------------------------------------------*/

/* Initialize the top level module */
void top_init(void)
{
  if (config_load((uint8_t*)tt_buf, sizeof(TtEelem_t) * TIME_TABLE_LEN) != STATUS_OK)
  {
    config_set_defaults();
    memcpy(tt_buf, tt_default, sizeof(TtEelem_t) * TIME_TABLE_LEN);
  }
  timetable_clear();
  motor_init(cfg.filter_length);
  top.sample_count = 0;
  top.tt_ptr = 0;
}


/* Top level module regular job */
void top_job(void)
{
  if (cfg.command == CMD_STEP_FORWARD)
  {
    if (!is_motor_running())
    {
      timetable_clear();
      tt[0].time = cfg.forward_time;
      tt[0].volt = cfg.forward_speed;
      tt[1].time = get_filter_length();
      tt[1].volt = 0;
      top.tt_ptr = 0;
      top.sample_count = 0;
      motor_control(1);
    }
    cfg.command = CMD_NONE;
  }

  else if (cfg.command == CMD_STEP_REWIND)
  {
    if (!is_motor_running())
    {
      timetable_clear();
      tt[0].time = cfg.rewind_time;
      tt[0].volt = -cfg.rewind_speed;
      tt[1].time = get_filter_length();
      tt[1].volt = 0;
      top.tt_ptr = 0;
      top.sample_count = 0;
      motor_control(1);
    }
    cfg.command = CMD_NONE;
  }

  else if (cfg.command == CMD_WAVE)
  {
    if (!is_motor_running())
    {
      memcpy(tt, tt_buf, sizeof(TtEelem_t) * TIME_TABLE_LEN);
      top.tt_ptr = 0;
      top.sample_count = 0;
      motor_control(1);
    }
    cfg.command = CMD_NONE;
  }

  else if (cfg.command == CMD_CFG_SAVE)
  {
    if (!is_motor_running())
    {
      config_save((uint8_t*)tt_buf, sizeof(TtEelem_t) * TIME_TABLE_LEN);
      motor_init(cfg.filter_length);
    }
    cfg.command = CMD_NONE;
  }

  else if (cfg.command == CMD_CFG_LOAD)
  {
    if (!is_motor_running())
    {
      config_load((uint8_t*)tt_buf, sizeof(TtEelem_t) * TIME_TABLE_LEN);
      motor_init(cfg.filter_length);
    }
    cfg.command = CMD_NONE;
  }

  else if (cfg.command == CMD_CFG_DEFAULT)
  {
    if (!is_motor_running())
    {
      config_set_defaults();
      memcpy(tt_buf, tt_default, sizeof(TtEelem_t) * TIME_TABLE_LEN);
      motor_init(cfg.filter_length);
    }
    cfg.command = CMD_NONE;
  }

  else if (cfg.command == CMD_STOP)
  {
    motor_control(0);
    cfg.command = CMD_NONE;
  }
}


/* Driver input data update callback */
int16_t motor_update_callback(void)
{
  if (top.sample_count > 0)
  {
    // play samples in one element
    top.sample_count--;
    return top.sample;
  }
  else
  {
    if (top.tt_ptr > TIME_TABLE_LEN || tt[top.tt_ptr].time == 0)
    {
      // end of timetable, stop
      motor_control(0);
      return 0;
    }
    else
    {
      // take next element
      top.sample_count = tt[top.tt_ptr].time;
      top.sample = tt[top.tt_ptr].volt;
      top.tt_ptr++;
      return top.sample;
    }
  }
}

/* Private functions ---------------------------------------------------------*/

/* Clear the timetable */
static void timetable_clear(void)
{
  uint8_t i;
  for (i = 0; i < TIME_TABLE_LEN; i++)
  {
    tt[i].time = 0;
    tt[i].volt = 0;
  }
}

/* ---------------------------------------------------------------------------*/

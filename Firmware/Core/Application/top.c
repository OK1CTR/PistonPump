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
#include "configuration.h"
#include "motor.h"

/* Private defines -----------------------------------------------------------*/

/* Timetable length */
#define TIME_TABLE_LEN              8

/* Default timetable contents */
#define TT_DEFAULT {\
  {-1000,  150},\
  {  100,  150},\
  {    0,  200},\
  { -300,  150},\
  {    0,  150},\
  {    0,    0},\
  {    0,    0},\
  {    0,    0}}

/* Private typedefs ----------------------------------------------------------*/

/* Definition of all private variables except timetable */
typedef struct
{
  uint32_t sample_count;
  uint32_t repeat_tmr;
  int16_t sample;
  uint8_t tt_ptr;
  uint8_t repeat_count;
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
  CMD_STOP,                   ///< emergency motor stop
  CMD_REPEAT                  ///< repeat programmable wave
};

/* Public variables ----------------------------------------------------------*/

/* All private variables except timetable */
Top_Private_t top;

/* Timetable array */
TtEelem_t tt[TIME_TABLE_LEN];

/* Programmable wave buffer */
TtEelem_t tt_buf[TIME_TABLE_LEN];

/* Default programmable wave */
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
  top.repeat_count = 0;
  top.repeat_tmr = 0;
}


/* Top level module regular job */
void top_job(void)
{
  uint16_t command;
  // timed event handling
  if (cfg.command == CMD_NONE)
  {
    if (top.repeat_count > 0 && TICK_EXPIRED(top.repeat_tmr))
    {
      top.repeat_count--;
      top.repeat_tmr = HAL_GetTick() + cfg.repeat_period;
      command = CMD_WAVE;
    }
    else
    {
      return;  // don't forget!
    }
  }
  else
  {
    command = cfg.command;
    cfg.command = CMD_NONE;
  }

  // command handling
  if (command == CMD_STEP_FORWARD)
  {
    if (!is_motor_running() && top.repeat_count == 0)
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
  }

  else if (command == CMD_STEP_REWIND)
  {
    if (!is_motor_running() && top.repeat_count == 0)
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
  }

  else if (command == CMD_WAVE)
  {
    if (!is_motor_running())
    {
      memcpy(tt, tt_buf, sizeof(TtEelem_t) * TIME_TABLE_LEN);
      top.tt_ptr = 0;
      top.sample_count = 0;
      motor_control(1);
    }
  }

  else if (command == CMD_CFG_SAVE)
  {
    if (!is_motor_running() && top.repeat_count == 0)
    {
      config_save((uint8_t*)tt_buf, sizeof(TtEelem_t) * TIME_TABLE_LEN);
      motor_init(cfg.filter_length);
    }
  }

  else if (command == CMD_CFG_LOAD)
  {
    if (!is_motor_running() && top.repeat_count == 0)
    {
      config_load((uint8_t*)tt_buf, sizeof(TtEelem_t) * TIME_TABLE_LEN);
      motor_init(cfg.filter_length);
    }
  }

  else if (command == CMD_CFG_DEFAULT)
  {
    if (!is_motor_running() && top.repeat_count == 0)
    {
      config_set_defaults();
      memcpy(tt_buf, tt_default, sizeof(TtEelem_t) * TIME_TABLE_LEN);
      motor_init(cfg.filter_length);
    }
  }

  else if (command == CMD_STOP)
  {
    motor_control(0);
    top.repeat_count = 0;
    top.repeat_tmr = 0;
  }

  else if (command == CMD_REPEAT)
  {
    if (!is_motor_running() && top.repeat_count == 0)
    {
      top.repeat_count = cfg.repeat_count;
      top.repeat_tmr = 0;
    }
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


/* Read programmable wave buffer item */
Status_t prog_wave_read(uint16_t address, uint16_t *value)
{
  if (address > TIME_TABLE_LEN * 2)
  {
    return STATUS_ERROR;
  }
  *value = (address % 2) ? tt_buf[address / 2].time : tt_buf[address / 2].volt;
  return STATUS_OK;
}


/* Write programmable wave buffer item */
Status_t prog_wave_write(uint16_t address, uint16_t value)
{
  if (address > TIME_TABLE_LEN * 2)
  {
    return STATUS_ERROR;
  }
  if (address % 2)
  {
    tt_buf[address / 2].time = value;
  }
  else
  {
    tt_buf[address / 2].volt = value;
  }
  return STATUS_OK;
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

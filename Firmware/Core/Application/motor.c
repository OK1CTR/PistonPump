/*
 * motor.c
 *
 *  Created on: Jan 14, 2025
 *      Author: Riki
 */

/* Includes ------------------------------------------------------------------*/

#include <stdlib.h>

#include "main.h"
#include "motor.h"

/* Private defines -----------------------------------------------------------*/

/* PWM timer instance */
#define PWM_TIMER_INSTANCE        htim1
/* Maximal state in the PWM timer&counter */
#define MAX_PWM_TMR_STATE         4095

/* Public variables ----------------------------------------------------------*/

/* PWM timer instance */
extern TIM_HandleTypeDef PWM_TIMER_INSTANCE;

/* Private variables ---------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

/* Motor driver initialization */
void motor_init(void)
{
  HAL_GPIO_WritePin(GPIOB, EN_L_Pin | EN_R_Pin, GPIO_PIN_SET);
  __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1, 0);
  __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2, 0);
  HAL_TIM_PWM_Start(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2);
}

/* Motor driver regular job */
void motor_job(void)
{
}

/* Motor driver initialization */
void motor_set(int32_t power)
{
  if (power == 0)
  {
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1, 0);
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2, 0);
  }
  else if (power > 0)
  {
    uint32_t pwr = power * MAX_PWM_TMR_STATE / 1000;
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2, 0);
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1, pwr);
  }
  else
  {
    uint32_t pwr = abs(power) * MAX_PWM_TMR_STATE / 1000;
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1, 0);
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2, pwr);
  }
}

/* ---------------------------------------------------------------------------*/

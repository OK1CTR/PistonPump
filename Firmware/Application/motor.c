/**
 * @file       motor.c
 * @author     OK1CTR
 * @date       Jan 21, 2025
 * @brief      DC motor PWM bridge control module
 *
 * @addtogroup grMotor
 * @{
 */

/* Includes ------------------------------------------------------------------*/

#include <stdlib.h>

#include "main.h"
#include "motor.h"

/* Private defines -----------------------------------------------------------*/

/*! PWM timer instance */
#define PWM_TIMER_INSTANCE        htim1
/*! Maximal state in the PWM timer&counter */
#define PWM_TMR_MAX               4095
/*! Maximal filter length = filter buffer length */
#define FILER_LEN_MAX             1000

/* Private typedefs ----------------------------------------------------------*/

/*! Definition of all private variables */
typedef struct
{
  uint16_t filter_len;            ///< filter buffer actual length
  uint16_t filter_ptr;            ///< filter buffer read/write pointer
  int32_t filter_sum;             ///< actual sum of filter buffer contents
  volatile uint8_t running;       ///< if true, motor controller is in active state
} Mot_Private_t;

/* Public variables ----------------------------------------------------------*/

/*! PWM timer instance */
extern TIM_HandleTypeDef PWM_TIMER_INSTANCE;

/* Private variables ---------------------------------------------------------*/

/*! All private variables except filter memory */
Mot_Private_t mot;

/*! Filter buffer */
int16_t fltbuf[FILER_LEN_MAX];

/* Private function prototypes -----------------------------------------------*/

/*! Filter initialization */
static void filt_init(uint16_t length);

/*! Filter job */
static int16_t filt_job(int16_t sample);

/* Functions -----------------------------------------------------------------*/

/* Motor driver initialization */
void motor_init(uint16_t filter_length)
{
  // stop the driver
  mot.running = 0;
  // initialize sample filter
  filt_init(filter_length);

  // initialize PWM timer
  HAL_GPIO_WritePin(GPIOB, EN_L_Pin | EN_R_Pin, GPIO_PIN_SET);
  __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1, 0);
  __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2, 0);
  HAL_TIM_PWM_Start(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2);
  __HAL_TIM_ENABLE_IT(&PWM_TIMER_INSTANCE, TIM_IT_UPDATE);
}

/* Motor driver regular job */
void motor_job(void)
{
}


/* Motor driver timer interrupt handler */
void motor_irq_handler(void)
{
  if (mot.running)
  {
    motor_set_voltage(filt_job(motor_update_callback()));
  }
  else
  {
    motor_set_voltage(0);
  }
}


/* Set motor driver output voltage equivalent */
void motor_set_voltage(int16_t voltage)
{
  if (voltage == 0)
  {
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1, 0);
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2, 0);
  }
  else if (voltage > 0)
  {
    uint32_t volt = voltage * PWM_TMR_MAX / 1000;
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2, 0);
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1, volt);
  }
  else
  {
    uint32_t volt = abs(voltage) * PWM_TMR_MAX / 1000;
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_1, 0);
    __HAL_TIM_SetCompare(&PWM_TIMER_INSTANCE, TIM_CHANNEL_2, volt);
  }
}


/* Start and stop the motor */
void motor_control(uint8_t run)
{
  mot.running = run;
  HAL_GPIO_WritePin(LED_ONBOARD_GPIO_Port, LED_ONBOARD_Pin, (run) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}


/* Return the motor running state */
uint8_t is_motor_running(void)
{
  return mot.running;
}


/* Return the actual filter length set */
uint16_t get_filter_length(void)
{
  return mot.filter_len;
}

/* Driver input data update callback */
__weak int16_t motor_update_callback(void)
{
  return 0;
}

/* Private Functions ---------------------------------------------------------*/

/* Filter initialization */
static void filt_init(uint16_t length)
{
  uint32_t i;

  if (length > FILER_LEN_MAX)
  {
    length = FILER_LEN_MAX;
  }

  mot.filter_len = length;
  mot.filter_ptr = 0;

  for (i = 0; i < FILER_LEN_MAX; i++)
  {
    fltbuf[i] = 0;
  }
}


/* Filter job */
static int16_t filt_job(int16_t sample)
{
  mot.filter_sum += sample;
  mot.filter_sum -= fltbuf[mot.filter_ptr];
  fltbuf[mot.filter_ptr] = sample;
  if (++mot.filter_ptr > mot.filter_len - 1)
  {
    mot.filter_ptr = 0;
  }
  return mot.filter_sum / mot.filter_len;
}

/* ---------------------------------------------------------------------------*/

/** @} */

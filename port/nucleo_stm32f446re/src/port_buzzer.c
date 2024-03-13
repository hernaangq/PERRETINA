/**
 * @file port_buzzer.c
 * @brief Portable functions to interact with the buzzer of the system.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 07/05/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "port_buzzer.h"
#include <math.h>

/* Defines --------------------------------------------------------------------*/
#define ALT_FUNC1_TIM_PWM 1 /*!< TIM2 Alternate Function mapping */

/* Typedefs --------------------------------------------------------------------*/

/// @brief Structure to define the HW dependencies of a buzzer.
typedef struct
{
  GPIO_TypeDef *p_port; /*!< GPIO where the buzzer is connected */
  uint8_t pin;          /*!< Pin/line where the infrared buzzer is connected */
  uint8_t alt_func;     /*!< Alternate function value according to the Alternate function table of the datasheet */

} port_buzzer_hw_t;

/* Global variables ------------------------------------------------------------*/

/// @brief Array of elements that represents the HW characteristics of the buzzers.
static port_buzzer_hw_t buzzers_arr[] = {
    [BUZZER_0_ID] = {.p_port = BUZZER_0_GPIO, .pin = BUZZER_0_PIN, .alt_func = ALT_FUNC1_TIM_PWM},
};

/* buzzer private functions */

/// @brief Configure the PWM timer. This timer configures the PWM for the buzzer.
/// @param buzzer_id Buzzer ID. If we have more than one buzzer, this value allows us to know which one of the we want to configure.
static void _timer_pwm_setup(uint32_t buzzer_id)
{
  if (buzzer_id == BUZZER_0_ID)
  {
    TIM2->CR1 |= 0x0001;
    TIM2->CCER |= 0x0010;
    TIM2->CCMR1 |= 0x6000;
    TIM2->CCR2 = 0;
  }
}

/* Public functions */

void port_buzzer_init(uint8_t buzzer_id)
{
  port_system_gpio_config(buzzers_arr[buzzer_id].p_port, buzzers_arr[buzzer_id].pin, GPIO_MODE_ALTERNATE, GPIO_PUPDR_NOPULL);
  port_system_gpio_config_alternate(buzzers_arr[buzzer_id].p_port, buzzers_arr[buzzer_id].pin, buzzers_arr[buzzer_id].alt_func);
  _timer_pwm_setup(buzzer_id);
}

void port_buzzer_pwm_timer_set(uint8_t buzzer_id, uint32_t freq)
{
  if (buzzer_id == BUZZER_0_ID)
  {
    uint32_t arr = ((SystemCoreClock / freq) - 1);
    TIM2->ARR = arr;
    TIM2->PSC = 0;
    TIM2->EGR |= 0x0001;
    uint32_t ccr2 = BUZZER_PWM_DC * (arr + 1);
    TIM2->CCR2 = ccr2;
    port_system_delay_ms(500);
    TIM2->CCR2 = 0;
  }
}

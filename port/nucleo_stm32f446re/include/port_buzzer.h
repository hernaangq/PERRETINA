/**
 * @file port_buzzer.h
 * @brief Header for port_buzzer.c file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 7/05/2023
 */

#ifndef PORT_BUZZER_H_
#define PORT_BUZZER_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "port_system.h"

/* HW dependent includes */

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define BUZZER_0_ID 0       /*!< Buzzer identifier */
#define BUZZER_0_GPIO GPIOB /*!< Buzzer GPIO port */
#define BUZZER_0_PIN 3      /*!< Buzzer GPIO pin */
#define BUZZER_PWM_DC 0.9   /*!< PWM DC level */

/* Musical notes frequencies */
#define NOTA_DO 261.63
#define NOTA_DO2 277.18
#define NOTA_RE 293.66
#define NOTA_RE2 311.13
#define NOTA_MI 329.63
#define NOTA_FA 349.23
#define NOTA_FA2 369.99
#define NOTA_SOL 392
#define NOTA_SOL2 415.3
#define NOTA_LA 440
#define NOTA_LA2 466
#define NOTA_SI 493.88
#define NOTA_SIB 466.16
#define NOTA_MIB 311.13

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Configure the HW specifications of a given buzzer.
/// @param buzzer_id Buzzer ID. This index is used to select the element of the buzzers_arr[] array.
/// @param status To indicate if PWM starts, or not, from the beginning.
void port_buzzer_init(uint8_t buzzer_id);

/// @brief Set the PWM ON or OFF, with the note frequency given
/// @param buzzer_id Buzzer ID. This index is used to select the element of the buzzers_arr[] array.
/// @param freq Set the frequency of the sound produced
void port_buzzer_pwm_timer_set(uint8_t buzzer_id, uint32_t freq);

#endif
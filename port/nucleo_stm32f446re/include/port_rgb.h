/**
 * @file port_rgb.h
 * @brief Header for port_rgb.c file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 18/04/2023
 */

#ifndef PORT_RGB_H_
#define PORT_RGB_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define RGB_0_ID 0         /*!< RBG ID */
#define RGB_R_0_GPIO GPIOB /*!< R GPIO */
#define RGB_R_0_PIN 4      /*!< R GPIO pin*/
#define RGB_G_0_GPIO GPIOC /*!< G GPIO */
#define RGB_G_0_PIN 7      /*!< G GPIO pin */
#define RGB_B_0_GPIO GPIOB /*!< B GPIO */
#define RGB_B_0_PIN 5      /*!< B GPIO pin */

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Configure the HW specifications of a given RGB LED.
/// @param rgb_id RGB LED ID. This index is used to select the element of the rgb_arr[] array
void port_rgb_init(uint8_t rgb_id);

/// @brief Set a color on the RGB LED.
/// @param rgb_id RGB LED ID. This index is used to select the element of the rgb_arr[] array
/// @param r Intensity level of the RED LED
/// @param g Intensity level of the GREEN LED
/// @param b Intensity level of the BLUE LED
void port_rgb_set_color(uint8_t rgb_id, uint8_t r, uint8_t g, uint8_t b);

#endif
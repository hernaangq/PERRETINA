/**
 * @file port_sensor.h
 * @brief Header for port_sensor.c file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 7/05/2023
 */

#ifndef PORT_SENSOR_H_
#define PORT_SENSOR_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>
#include "port_system.h"

/* HW dependent includes */

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define SENSOR_0_ID 0       /*!< SENSOR identifier */
#define SENSOR_0_GPIO GPIOA /*!< SENSOR GPIO port */
#define SENSOR_0_PIN 10     /*!< SENSOR GPIO pin */

#define SENSOR_LIMIT 0.5 /*!< SENSOR limit between light and no light */

/* Function prototypes and explanation -------------------------------------------------*/
/// @brief Configure the HW specifications of a given sensor.
/// @param sensor_id Sensor ID. This index is used to select the element of the sensors_arr[] array.
void port_sensor_init(uint32_t sensor_id);

/// @brief Return the value of the sensor
/// @param sensor_id Sensor ID. This index is used to select the element of the sensors_arr[] array.
/// @return uint32_t Light sensor value
uint32_t port_sensor_get_value(uint32_t sensor_id);

#endif

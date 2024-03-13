/**
 * @file port_sensor.c
 * @brief File containing portable functions related to the HW of the light sensor .
 *
 * This files defines an internal struct which coontains the HW information of the
 *
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 07/05/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "port_sensor.h"

/* Typedefs --------------------------------------------------------------------*/

/// @brief Structure to define the HW dependencies of a light sensor.
typedef struct
{
    GPIO_TypeDef *p_port; /*!< GPIO where the sensor is connected */
    uint8_t pin;          /*!< Pin/line where the sensor is connected */
} port_sensor_hw_t;

/* Global variables ------------------------------------------------------------*/

/// @brief Array of elements that represents the HW characteristics of the sensors.
static port_sensor_hw_t sensors_arr[] = {
    [SENSOR_0_ID] = {.p_port = SENSOR_0_GPIO, .pin = SENSOR_0_PIN},
};

void port_sensor_init(uint32_t sensor_id)
{
    port_system_gpio_config(sensors_arr[sensor_id].p_port, sensors_arr[sensor_id].pin, GPIO_MODE_IN, GPIO_PUPDR_NOPULL);                        // input, no-pulls
    port_system_gpio_config_exti(sensors_arr[sensor_id].p_port, sensors_arr[sensor_id].pin, (TRIGGER_RISING_EDGE | TRIGGER_ENABLE_INTERR_REQ)); // ???????????
    port_system_gpio_exti_enable(sensors_arr[sensor_id].pin, 1, 0);
}

uint32_t port_sensor_get_value(uint32_t sensor_id)
{
    uint32_t value = port_system_gpio_read(sensors_arr[SENSOR_0_ID].p_port, sensors_arr[SENSOR_0_ID].pin);
    return value;
}

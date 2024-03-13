/**
 * @file fsm_sensor.h
 * @brief Header for fsm_sensor.c file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 23/03/2023
 */

#ifndef FSM_SENSOR_H_
#define FSM_SENSOR_H_


/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Return the light of the sensor.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_sensor_t.
/// @return bool
bool fsm_sensor_get_light(fsm_t * p_this);

/// @brief Initialize a sensor FSM. 
/// @param p_this Pointer to an fsm_t struct than contains an fsm_sensor_t.
/// @param sensor_id Unique sensor identifier number.
void fsm_sensor_init(fsm_t *p_this, uint32_t sensor_id);

/// @brief Create a new sensor FSM.
/// @param sensor_id Unique sensor identifier number.
/// @return A pointer to the sensor FSM.
fsm_t* fsm_sensor_new(uint32_t sensor_id);


/// @brief Check if the sensor FSM is active, or not.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_sensor_t.
/// @return true
/// @return false
bool fsm_sensor_check_activity(fsm_t *p_this);


#endif


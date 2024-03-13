/**
 * @file fsm_retina.h
 * @brief Header for fsm_retina.c file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 23/03/2023
 */
#ifndef FSM_RETINA_H_
#define FSM_RETINA_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"


/* Function prototypes and explanation ---------------------------------------*/

/// @brief Create a new RETINA FSM.
/// @param p_fsm_button User button FSM.
/// @param button_press_time Duration in ms of the button press to change between transmitter and receiver modes.
/// @param p_fsm_tx Infrared transmitter FSM.
/// @param p_fsm_rx Pointer to an fsm_t struct than contains an fsm_rx_t. Added in Version 3.
/// @param rgb_id 	Unique RGB identifier number. Added in Version 3.
/// @param buzzer_id 	Unique RGB identifier number. Added in MEJORAS.
/// @param p_fsm_sensor 	Pointer to an fsm_t struct than contains an fsm_sensor_t.
/// @return A pointer to the Retina FSM.
fsm_t *fsm_retina_new(fsm_t *p_fsm_button, uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id, uint8_t buzzer_id, fsm_t *p_fsm_sensor);

/// @brief Initialize the infrared transmitter FSM.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @param p_fsm_button Pointer to an fsm_t struct than contains an fsm_button_t.
/// @param button_press_time Duration in ms of the button press to change between transmitter and receiver modes.
/// @param p_fsm_tx Pointer to an fsm_t struct than contains an fsm_tx_t.
/// @param p_fsm_rx Pointer to an fsm_t struct than contains an fsm_rx_t. Added in Version 3.
/// @param rgb_id Unique RGB identifier number. Added in Version 3.
/// @param buzzer_id 	Unique RGB identifier number. Added in MEJORAS.
/// @param p_fsm_sensor 	Pointer to an fsm_t struct than contains an fsm_sensor_t.
void fsm_retina_init(fsm_t *p_this, fsm_t *p_fsm_button, uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id, uint8_t buzzer_id, fsm_t *p_fsm_sensor);

#endif

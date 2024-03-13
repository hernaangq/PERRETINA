/**
 * @file fsm_button.h
 * @brief Header for fsm_button.c file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 23/03/2023
 */

#ifndef FSM_BUTTON_H_
#define FSM_BUTTON_H_


/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Function prototypes and explanation -------------------------------------------------*/

/// @brief Return the duration of the last button press.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_button_t.
/// @return uint32_t Duration of the last button press.
uint32_t fsm_button_get_duration(fsm_t * p_this); 

/// @brief Initialize a button FSM. 
/// @param p_this Pointer to an fsm_t struct than contains an fsm_button_t.
/// @param debounce_time Anti-debounce time in milliseconds.
/// @param button_id Unique button identifier number.
void fsm_button_init(fsm_t * p_this, uint32_t debounce_time, uint32_t button_id);

/// @brief Create a new button FSM.
/// @param debounce_time Anti-debounce time in milliseconds.
/// @param button_id Unique button identifier number.
/// @return A pointer to the button FSM.
fsm_t* fsm_button_new(uint32_t 	debounce_time, uint32_t button_id);

/// @brief Reset the duration of the last button press.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_button_t.
void fsm_button_reset_duration(fsm_t * p_this);


/// @brief Check if the button FSM is active, or not.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_button_t.
/// @return true
/// @return false
bool fsm_button_check_activity(fsm_t *p_this);


#endif


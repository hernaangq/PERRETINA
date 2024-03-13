/**
 * @file fsm_rx.c
 * @brief Infrared receiver FSM main file.
 * @author Ángel Rodrigo Pérez Iglesias
 * @author Hernán García Quijano
 * @date 30/04/2023
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdio.h>

/* Other includes */
#include "fsm_rx.h"
#include "fsm_rx_nec.h"
#include "port_system.h"
#include "port_rx.h"

/* Typedefs --------------------------------------------------------------------*/

/// @brief Structure to define the infrared receiver FSM.
typedef struct
{
  fsm_t f;                     // Infrared receiver FSM
  fsm_t *p_fsm_rx_nec;         // Pointer to the FSM to parse NEC protocol codes
  uint32_t message_timeout_ms; // Time in milliseconds after which, if no edge is detected, the processing process begins
  uint32_t last_tick;          // Time-tick when of last edge detected.
  uint32_t num_edges_detected; // Number of edges detected
  uint32_t code;               // Code received once it is parsed
  bool is_repetition;          // Indicate if the received code is a repetition code or not
  bool is_error;               // Indicate if the received code is an error or not
  bool status;                 // Indicate if the infrared receiver must be turned on, or off
  uint8_t rx_id;               // Receiver ID. Must be unique.
} fsm_rx_t;

/* Defines and enums ----------------------------------------------------------*/
/* Enums */

/// @brief Enumeration of the states.
enum
{
  OFF_RX,  // Starting state of the FSM waiting the status to be true.
  IDLE_RX, // Idle state where the FSM remains checking if a reception has started or the system must turn OFF.
  WAIT_RX  // State active during the reception of an infrared code. It leaves the status after a timeout without receptions.
};

/* State machine input or transition functions */

/// @brief Check if the infrared receiver must be turned ON.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_rx_t.
/// @return TRUE if status is ON
static bool check_on_rx(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  return p_fsm->status;
}

/// @brief Check if the infrared receiver must be turned ON.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_rx_t.
/// @return TRUE if status is OFF
static bool check_off_rx(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  return !(p_fsm->status);
}

/// @brief Check if there has been a change in the GPIO connected to the infrared receiver.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_rx_t.
/// @return TRUE if change detected
static bool check_edge_detection(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  uint32_t edges = port_rx_get_num_edges(p_fsm->rx_id);
  return (edges != p_fsm->num_edges_detected);
}

/// @brief Checks if a timeout has elapsed without detecting any change in infrared reception.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_rx_t.
/// @return TRUE if there has been a timeout without detecting anything
static bool check_timeout(fsm_t *p_this)
{
  uint32_t ticks = port_system_get_millis();
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  return ((ticks - p_fsm->last_tick) > p_fsm->message_timeout_ms);
}

/* State machine output or action functions */

/// @brief Start the infrared reception system (when the system changes to receiver mode).
/// @param p_this Pointer to an fsm_t struct than contains an fsm_rx_t.
static void do_rx_start(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  p_fsm->p_fsm_rx_nec = fsm_rx_NEC_new();
  port_rx_tmr_start();
  p_fsm->num_edges_detected = 0;
  port_rx_clean_buffer(p_fsm->rx_id);
  port_rx_en(p_fsm->rx_id, true);
}

/// @brief Stop the infrared reception system (when the system changes to receiver mode).
/// @param p_this Pointer to an fsm_t struct than contains an fsm_rx_t.
static void do_rx_stop(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  port_rx_tmr_stop();
  port_rx_en(p_fsm->rx_id, false);
  fsm_destroy(p_fsm->p_fsm_rx_nec);
}

/// @brief Transcribes the received code information using the time-ticks of the edges detected by the infrared receiver.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_rx_t.
static void do_store_data(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  uint16_t *buffer_edges = port_rx_get_buffer_edges(p_fsm->rx_id);
  uint32_t num_edges = port_rx_get_num_edges(p_fsm->rx_id);
  uint32_t *p_code = (uint32_t *)(&(p_fsm->code));
  p_fsm->is_repetition = fsm_rx_NEC_parse_code(p_fsm->p_fsm_rx_nec, buffer_edges, num_edges, p_code);
  p_fsm->is_error = (p_fsm->code == 0x00) && (!p_fsm->is_repetition);
  p_fsm->num_edges_detected = 0;
  port_rx_clean_buffer(p_fsm->rx_id);
}

/// @brief Update the time of the last tick and the number of edges detected.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_rx_t.
static void do_update_len_and_timeout(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  p_fsm->last_tick = port_system_get_millis();
  p_fsm->num_edges_detected = port_rx_get_num_edges(p_fsm->rx_id);
}

/// @brief Array representing the transitions table of the infrared receiver FSM.
static fsm_trans_t fsm_trans_rx[] = {
    //{EstadoIni, FuncCompruebaCondicion, EstadoSig, FuncAccionesSiTransicion}
    {OFF_RX, check_on_rx, IDLE_RX, do_rx_start},
    {IDLE_RX, check_off_rx, OFF_RX, do_rx_stop},
    {IDLE_RX, check_edge_detection, WAIT_RX, do_update_len_and_timeout},
    {WAIT_RX, check_edge_detection, WAIT_RX, do_update_len_and_timeout},
    {WAIT_RX, check_timeout, IDLE_RX, do_store_data},
    {-1, NULL, -1, NULL}};

/* Other auxiliary functions */

void fsm_rx_set_rx_status(fsm_t *p_this, bool status)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  p_fsm->status = status;
}

uint32_t fsm_rx_get_code(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  return p_fsm->code;
}

bool fsm_rx_get_repetition(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  return p_fsm->is_repetition;
}

bool fsm_rx_get_error_code(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  return p_fsm->is_error;
}

void fsm_rx_reset_code(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  p_fsm->code = 0;
  p_fsm->is_error = false;
  p_fsm->is_repetition = false;
}

void fsm_rx_init(fsm_t *p_this, uint8_t rx_id)
{
  fsm_init(p_this, fsm_trans_rx);
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  p_fsm->rx_id = rx_id;
  p_fsm->code = 0;
  p_fsm->num_edges_detected = 0;
  p_fsm->last_tick = 0;
  p_fsm->is_error = false;
  p_fsm->is_repetition = false;
  p_fsm->status = true;
  p_fsm->message_timeout_ms = NEC_MESSAGE_TIMEOUT_US;
  port_rx_init(p_fsm->rx_id);
}

fsm_t *fsm_rx_new(uint8_t rx_id)
{
  fsm_t *p_fsm = malloc(sizeof(fsm_rx_t));
  fsm_rx_init(p_fsm, rx_id);
  return p_fsm;
}

bool fsm_rx_check_activity(fsm_t *p_this)
{
  fsm_rx_t *p_fsm = (fsm_rx_t *)(p_this);
  return (p_fsm->f.current_state == WAIT_RX);
}
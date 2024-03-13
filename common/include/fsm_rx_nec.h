/**
 * @file fsm_rx_nec.h
 * @brief Header for fsm_rx_nec.c file.
 * @author Ángel Rodrigo Pérez Iglesias
 * @author Hernán García Quijano
 * @date fecha
 */

#ifndef NEC_PROTOCOL_H_
#define NEC_PROTOCOL_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdint.h>
#include <stdbool.h>

/* Other includes */
#include "fsm.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
/* NEC reception macros */
#define NEC_ADDRESS_BITS 16                                  /*!< Total number of address bits of a NEC frame */
#define NEC_COMMAND_BITS 16                                  /*!< Total number of command bits of a NEC frame */
#define NEC_FRAME_BITS (NEC_ADDRESS_BITS + NEC_COMMAND_BITS) /*!< Total number of bits of a NEC frame */
#define NEC_PROLOGUE_EDGES 3                                 /*!< Number of edges of the prologue of a NEC code */
#define NEC_EPILOGUE_EDGES 1                                 /*!< Number of edges of the epilogue of a NEC code */
#define NEC_SYMBOL_EDGES 2                                   /*!< Number of edges of the symbols of a NEC code */
#define NEC_FRAME_EDGES 256                                  /*!< Array-size large enough to store all the edges received (the NEC code has much less edges) */

/* NEC pulses and silences times (minimum and maximum tolerances) */
#define TOLERANCE 0.30

#define NEC_RX_PROLOGUE_SILENCE_MIN_US 9000 * (1 - TOLERANCE)
#define NEC_RX_PROLOGUE_SILENCE_MAX_US 9000 * (1 + TOLERANCE)
#define NEC_RX_PROLOGUE_PULSE_MIN_US 4500 * (1 - TOLERANCE)
#define NEC_RX_PROLOGUE_PULSE_MAX_US 4500 * (1 + TOLERANCE)

#define NEC_RX_SYMBOL_SILENCE_MIN_US 562.5 * (1 - TOLERANCE)
#define NEC_RX_SYMBOL_SILENCE_MAX_US 562.5 * (1 + TOLERANCE)
#define NEC_RX_SYMBOL_0_PULSE_MIN_US (1125 - 562.5) * (1 - TOLERANCE)
#define NEC_RX_SYMBOL_0_PULSE_MAX_US (1125 - 562.5) * (1 + TOLERANCE)
#define NEC_RX_SYMBOL_1_PULSE_MIN_US (2250 - 562.5) * (1 - TOLERANCE)
#define NEC_RX_SYMBOL_1_PULSE_MAX_US (2250 - 562.5) * (1 + TOLERANCE)

#define NEC_RX_REPETITION_PULSE_MIN_US 1700 /*!< Minimum width of epilogue pulse at RX in microseconds */
#define NEC_RX_REPETITION_PULSE_MAX_US 2700 /*!< Maximum width of epilogue pulse at RX in microseconds */

#define NEC_MESSAGE_TIMEOUT_US 100 /*!< Timeout to wait without receiving an edge in microseconds */

/* NEC pulses and silences ticks (minimum and maximum tolerances) */
#define NEC_RX_TIMER_TICK_BASE_US 10                                                                   /*!< Number of microseconds that represents a tick of the reference clock. */
#define NEC_RX_PROLOGUE_TICKS_SILENCE_MIN (NEC_RX_PROLOGUE_SILENCE_MIN_US / NEC_RX_TIMER_TICK_BASE_US) /*!< #NEC_RX_PROLOGUE_SILENCE_MIN_US as ticks */
#define NEC_RX_PROLOGUE_TICKS_SILENCE_MAX (NEC_RX_PROLOGUE_SILENCE_MAX_US / NEC_RX_TIMER_TICK_BASE_US) /*!< #NEC_RX_PROLOGUE_SILENCE_MAX_US as ticks */
#define NEC_RX_PROLOGUE_TICKS_PULSE_MIN (NEC_RX_PROLOGUE_PULSE_MIN_US / NEC_RX_TIMER_TICK_BASE_US)     /*!< #NEC_RX_PROLOGUE_PULSE_MIN_US as ticks */
#define NEC_RX_PROLOGUE_TICKS_PULSE_MAX (NEC_RX_PROLOGUE_PULSE_MAX_US / NEC_RX_TIMER_TICK_BASE_US)     /*!< #NEC_RX_PROLOGUE_PULSE_MAX_US as ticks */
#define NEC_RX_REPETITION_TICKS_PULSE_MIN (NEC_RX_REPETITION_PULSE_MIN_US / NEC_RX_TIMER_TICK_BASE_US) /*!< #NEC_RX_REPETITION_PULSE_MIN_US as ticks */
#define NEC_RX_REPETITION_TICKS_PULSE_MAX (NEC_RX_REPETITION_PULSE_MAX_US / NEC_RX_TIMER_TICK_BASE_US) /*!< #NEC_RX_REPETITION_PULSE_MAX_US as ticks */
#define NEC_RX_SYMBOL_TICKS_SILENCE_MIN (NEC_RX_SYMBOL_SILENCE_MIN_US / NEC_RX_TIMER_TICK_BASE_US)     /*!< #NEC_RX_SYMBOL_SILENCE_MIN_US as ticks */
#define NEC_RX_SYMBOL_TICKS_SILENCE_MAX (NEC_RX_SYMBOL_SILENCE_MAX_US / NEC_RX_TIMER_TICK_BASE_US)     /*!< #NEC_RX_SYMBOL_SILENCE_MAX_US as ticks */
#define NEC_RX_SYMBOL_0_TICKS_PULSE_MIN (NEC_RX_SYMBOL_0_PULSE_MIN_US / NEC_RX_TIMER_TICK_BASE_US)     /*!< #NEC_RX_SYMBOL_0_PULSE_MIN_US as ticks */
#define NEC_RX_SYMBOL_0_TICKS_PULSE_MAX (NEC_RX_SYMBOL_0_PULSE_MAX_US / NEC_RX_TIMER_TICK_BASE_US)     /*!< #NEC_RX_SYMBOL_0_PULSE_MAX_US as ticks */
#define NEC_RX_SYMBOL_1_TICKS_PULSE_MIN (NEC_RX_SYMBOL_1_PULSE_MIN_US / NEC_RX_TIMER_TICK_BASE_US)     /*!< #NEC_RX_SYMBOL_1_PULSE_MIN_US as ticks */
#define NEC_RX_SYMBOL_1_TICKS_PULSE_MAX (NEC_RX_SYMBOL_1_PULSE_MAX_US / NEC_RX_TIMER_TICK_BASE_US)     /*!< #NEC_RX_SYMBOL_1_PULSE_MAX_US as ticks */

/* Function prototypes and explanation -------------------------------------------------*/
/**
 * @brief Create a new NEC processing FSM
 *
 * This FSM is created when the main system (RETINA) works in reception mode and it is destroyed when the system works in transmission mode.
 *
 * This FSM parses a given array of time-ticks into a NEC code. Each time tick indicates that there was an edge (rising or falling edge) in the GPIO connected to the infrared receiver.
 *
 * @attention The GPIO of the infrared receiver works in input mode and it is always at high level, thus, **the first edge is always a falling edge**. Being this so, we can assert that: *all edges in even positions of the array are falling edges, and edges in odd positions of the array are rising edges*. This is useful information to debug the FSM.
 *
 * The FSM works with time tolerances (deviations in the pulse and silence widths). It is assumed that the timer used to control the time ticks has a resolution of #NEC_RX_TIMER_TICK_BASE_US microseconds.
 *
 * **The FSM goes through all the edges comparing the duration between the current and the following edge**. With this information, we can check if we are parsing (i) the prologue, (ii) a symbol 0, (iii) a symbol 1, (iv) the epilogue, (v) a repetition, or (vi) if the infrared data received was noise of a spurious event, or even a code of another protocol.
 *
 * The FSM receives information of the time ticks of the edges and the number of edges detected. The FSM parses the code and returns it as a pointer. It also indicates if the code was a repetition code or not (it was a command). The FSM stores information on the number of bits remaining to read (expected to be #NEC_FRAME_BITS).
 *
 * Check the transition table `fsm_trans_rx_nec` for further information.
 *
 * @return A pointer to the FSM that parses infrared data as NEC protocol
 */
fsm_t *fsm_rx_NEC_new(void);

/// @brief Initialize a NEC processing FSM
/// @param p_this Pointer to an fsm_t struct than contains an fsm_rx_nec_t
void fsm_rx_NEC_init(fsm_t *p_this);

/// @brief Process a set of time ticks to a NEC code.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_rx_nec_t
/// @param p_edge_ticks Pointer to the array containing the the time ticks of the edges detected by the infrared receiver
/// @param num_edges Number of edges detected by the infrared receiver. It is used to know how far to count.
/// @param p_code 	Pointer given to store the code
/// @return true to indicate that the received code was a repetition
/// @return false to indicate that the received code was not a repetition (it was a command or noise)
bool fsm_rx_NEC_parse_code(fsm_t *p_this, uint16_t *p_edge_ticks, uint32_t num_edges, uint32_t *p_code);

#endif
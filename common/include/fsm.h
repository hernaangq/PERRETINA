/**
 * @file fsm.h
 * @brief Header for fsm.c file.
 * @author Teachers from the Departamento de Ingeniería Electrónica. Original authors: José M. Moya and Pedro J. Malagón. Latest contributor: Román Cárdenas.
 * @date 2023-01-01
 */

#ifndef FSM_H_
#define FSM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdbool.h>

/* Typedefs --------------------------------------------------------------------*/

/**
 * @brief Make `fsm_t` struct public 
 */
typedef struct fsm_t fsm_t;

/**
 * @brief Alias to refer to a pointer to an input condition function.
 */
typedef bool (*fsm_input_func_t)(fsm_t *); 

/**
 * @brief Alias to refer to a pointer to an output modification function.
 */
typedef void (*fsm_output_func_t)(fsm_t *);

/**
 * @brief Structure to define a state machine transition table.
 */
typedef struct fsm_trans_t
{
  int orig_state;        /*!< Origin state  */
  fsm_input_func_t in;   /*!< Input condition function */
  int dest_state;        /*!< Output state */
  fsm_output_func_t out; /*!< Output modification function */
} fsm_trans_t;

/**
 * @brief Structure that defines a state machine.
 */
struct fsm_t
{
  int current_state; /*!< Current state of the FSM */
  fsm_trans_t *p_tt; /*!< Pointer to the  state machine transition table */
};

/* Function prototypes -----------------------------------------------------------------*/
/**
 * @brief Allocates memory and create a new state machine from a transition table.
 *
 * The starting state of the state machine will correspond to the origin state of the first transition found in the transition table.  The transition table must end with a null transition {-1, NULL, -1, NULL}. In this way, the state machine will be able to detect that it has reached the end of the transition table.
 *
 * @param p_tt Pointer to the  state machine transition table
 * @return fsm_t* Pointer to the memory address where the new state machine is located
 */
fsm_t *fsm_new(fsm_trans_t *p_tt);

/**
 * @brief Create a new state machine from a table of transitions.
 *
 * The starting state of the state machine will correspond to the origin state of the first transition found in the transition table. The transition table must end with a null transition {-1, NULL, -1, NULL}. This will allow the state machine to detect that it has reached the end of the table. Unlike `fsm_new`, this function does not allocate memory for the state machine. Instead, it uses the memory address provided by the user.
 *
 * @param p_fsm Pointer to the memory address where the new state machine is located
 * @param p_tt Pointer to the  state machine transition table
 */
void fsm_init(fsm_t *p_fsm, fsm_trans_t *p_tt);

/**
 * @brief Check the full transition table.
 *
 * It loops through the transition table and, if an input condition is met, it switches to a new state and executes the corresponding output modification function.
 *
 * @param p_fsm Pointer to the memory address where the new state machine is located
 */
void fsm_fire(fsm_t *p_fsm);

/**
 * @brief
 *
 * It frees the memory previously allocated for the state machine. Once this function is called, the state machine becomes unusable. It is only necessary to call this function if the state machine was previously created by calling the `fsm_new` function.
 *
 * @param p_fsm Pointer to the memory address where the new state machine is located
 */
void fsm_destroy(fsm_t *p_fsm);

#endif /* FSM_H_ */

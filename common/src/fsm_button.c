/**
 * @file fsm_button.c
 * @brief Button FSM main file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 23/03/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "fsm_button.h"
#include "port_button.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the Button FSM.
 */
typedef struct fsm_button
{
    fsm_t f; /*!<Button FSM*/

    uint32_t debounce_time; /*!<Button debounce time in ms*/

    uint32_t next_timeout; /*!<Next timeout for the anti-debounce in ms*/

    uint32_t tick_pressed; /*!<Number of ticks when the button was pressed*/

    uint32_t duration; /*!<How much time the button has been pressed*/

    uint32_t button_id; /*!<Button ID. Must be unique.*/

} fsm_button_t;

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum
{
    BUTTON_RELEASED = 0,  /*!<Starting state. Also comes here when the button has been released*/
    BUTTON_RELEASED_WAIT, /*!<State to perform the anti-debounce mechanism for a falling edge*/
    BUTTON_PRESSED,       /*!<State while the button is being pressed*/
    BUTTON_PRESSED_WAIT   /*!<State to perform the anti-debounce mechanism for a rising edge*/
};

/* State machine input or transition functions */

/// @brief Check if button has been pressed.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_button_t.
/// @return true
/// @return false
static bool check_button_pressed(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this); // casteo de p_this
    return port_button_is_pressed(p_fsm->button_id);
}

/// @brief Check if button has been released
/// @param p_this Pointer to an fsm_t struct than contains an fsm_button_t.
/// @return true
/// @return false
static bool check_button_released(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this); // cast p_this
    return !(port_button_is_pressed(p_fsm->button_id));
}

/// @brief Check if the debounce-time has passed.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_button_t.
/// @return true
/// @return false
static bool check_timeout(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this); // cast p_this
    uint32_t aux = port_button_get_tick();
    return (aux > p_fsm->next_timeout);
}

/* State machine output or action functions */

/// @brief Store the system tick when the button was pressed. Update tick_pressed and next_timeout.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_button_t.
static void do_store_tick_pressed(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);                   // cast p_this
    p_fsm->tick_pressed = port_button_get_tick();                     // update tick_pressed to current tick
    p_fsm->next_timeout = p_fsm->tick_pressed + p_fsm->debounce_time; // update next_timeout considering current tick and the debounce time of the button
}

/// @brief Store the duration of the button press.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_button_t.
static void do_set_duration(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this); // cast p_this
    uint32_t actual_tick = port_button_get_tick();
    p_fsm->duration = (actual_tick - (p_fsm->tick_pressed));      // update duration to current tick - tick when pressed
    p_fsm->next_timeout = (actual_tick + (p_fsm->debounce_time)); // update next_timeout considering current tick and the debounce time of the button
}

/// @brief Array representing the transitions table of the FSM button.
static fsm_trans_t fsm_trans_button[] = {
    //{EstadoIni, FuncCompruebaCondicion, EstadoSig, FuncAccionesSiTransicion}
    {BUTTON_RELEASED, check_button_pressed, BUTTON_PRESSED_WAIT, do_store_tick_pressed},
    {BUTTON_PRESSED_WAIT, check_timeout, BUTTON_PRESSED, NULL},
    {BUTTON_PRESSED, check_button_released, BUTTON_RELEASED_WAIT, do_set_duration},
    {BUTTON_RELEASED_WAIT, check_timeout, BUTTON_RELEASED, NULL},
    {-1, NULL, -1, NULL}};

/* Other auxiliary functions */

uint32_t fsm_button_get_duration(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this); // cast p_this
    return (p_fsm->duration);
}

void fsm_button_reset_duration(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this); // cast p_this
    p_fsm->duration = 0;                            // Set duration to 0ms
}

fsm_t *fsm_button_new(uint32_t debounce_time, uint32_t button_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_button_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_button_init(p_fsm, debounce_time, button_id);
    return p_fsm;
}

void fsm_button_init(fsm_t *p_this, uint32_t debounce_time, uint32_t button_id)
{

    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    fsm_init(p_this, fsm_trans_button);

    p_fsm->debounce_time = debounce_time;
    p_fsm->button_id = button_id;

    p_fsm->tick_pressed = 0;
    p_fsm->duration = 0;

    port_button_init(button_id);
}

bool fsm_button_check_activity(fsm_t *p_this)
{
    fsm_button_t *p_fsm = (fsm_button_t *)(p_this);
    return (p_fsm->f.current_state != BUTTON_RELEASED);
}

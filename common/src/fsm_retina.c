/**
 * @file fsm_retina.c
 * @brief Retina FSM main file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 23/03/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "fsm_retina.h"
#include "commands.h"

#include "fsm_button.h"
#include "fsm_tx.h"
#include "fsm_rx.h"

#include "port_rgb.h"
#include "port_buzzer.h"
#include "port_system.h"
#include "port_sensor.h"
#include "fsm_sensor.h"
#include <stdio.h>

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define COMMANDS_MEMORY_SIZE 8 /*!< Number of NEC commands stored in the memory of the system Retina */

/* Enums */
enum
{
    WAIT_TX = 0, /*!< **Single state in Version 2**. State to wait in transmission mode */
    WAIT_RX,     /*!< **Single state in Version 2**. State to wait in reception mode */
    SLEEP_RX,    /*!< **Single state in Version 4**. State to sleep in transmission mode */
    SLEEP_TX,    /*!< **Single state in Version 4**. State to sleep in reception mode */
    EMERGENCY    /*!< **Single state in MEJORAS**. State of EMERGENCY when low light is detected */
};

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the Retina FSM.
 */
typedef struct
{
    fsm_t f;                                     /*!< Retina FSM  */
    fsm_t *p_fsm_button;                         /*!< Pointer to the FSM of the user button */
    uint32_t long_button_press_ms;               /*!< Duration of the button press to change between transmitter and receiver modes  */
    fsm_t *p_fsm_tx;                             /*!< Pointer to the FSM of the infrared transmitter */
    uint32_t tx_codes_arr[COMMANDS_MEMORY_SIZE]; /*!< Array to store in the memory of the system a number of codes to send in a loop */
    uint8_t tx_codes_index;                      /*!< Index to go though the elements of the tx_codes_arr  */
    fsm_t *p_fsm_rx;                             /*!<Pointer to the FSM of the infrared receiver*/
    uint32_t rx_code;                            /*!<Code received to be processed*/
    uint8_t rgb_id;                              /*!<Unique RGB LED Identifier*/
    uint8_t buzzer_id;                           /*!<Unique BUZZER Identifier*/
    fsm_t *p_fsm_sensor;                         /*!<Pointer to the FSM of the sensor*/

} fsm_retina_t;

/* Private functions */

/// @brief Identify the command and light the corresponding color.
/// @param rgb_id 	RGB LED ID. Must be unique.
/// @param code Code parsed that identifies a color.
/// @param buzzer_id BUZZER ID. Must be unique.
void _process_rgb_code(uint8_t rgb_id, uint32_t code, uint32_t buzzer_id)
{
    if (code == LIL_RED_BUTTON)
    {
        port_rgb_set_color(rgb_id, HIGH, LOW, LOW);
    }
    if (code == LIL_GREEN_BUTTON)
    {
        port_rgb_set_color(rgb_id, LOW, HIGH, LOW);
    }
    if (code == LIL_BLUE_BUTTON)
    {
        port_rgb_set_color(rgb_id, LOW, LOW, HIGH);
    }
    if (code == LIL_CYAN_BUTTON)
    {
        port_rgb_set_color(rgb_id, LOW, HIGH, HIGH);
    }
    if (code == LIL_MAGENTA_BUTTON)
    {
        port_rgb_set_color(rgb_id, HIGH, LOW, HIGH);
    }
    if (code == LIL_YELLOW_BUTTON)
    {
        port_rgb_set_color(rgb_id, HIGH, HIGH, LOW);
    }
    if (code == LIL_WHITE_BUTTON || code == LIL_ON_BUTTON)
    {
        port_rgb_set_color(rgb_id, HIGH, HIGH, HIGH);
    }
    if (code == LIL_OFF_BUTTON)
    {
        port_rgb_set_color(rgb_id, LOW, LOW, LOW);
    }
    if (code == LIL_FADE_BUTTON)
    {
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SIB);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_FA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SIB);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_FA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_FA2);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_FA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_FA2);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_FA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_FA2);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_FA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MIB);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_RE);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_LA);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_DO);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_MI);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
        port_buzzer_pwm_timer_set(buzzer_id, NOTA_SOL);
    }
}

/* State machine input or transition functions */

/// @brief Check if the button has been pressed fast to send a new command.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @return true
/// @return false
static bool check_short_pressed(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    int cond1 = (fsm_button_get_duration(p_fsm_retina->p_fsm_button) != 0);
    int cond2 = (fsm_button_get_duration(p_fsm_retina->p_fsm_button) < p_fsm_retina->long_button_press_ms);
    return cond1 && cond2;
}

/// @brief Check if the button has been pressed long to change between transmitter and receiver modes.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @return true
/// @return false
static bool check_long_pressed(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    int cond1 = (fsm_button_get_duration(p_fsm_retina->p_fsm_button) != 0);
    int cond2 = (fsm_button_get_duration(p_fsm_retina->p_fsm_button) >= p_fsm_retina->long_button_press_ms);
    return cond1 && cond2;
}

/// @brief Check if there is a new code in the infrared receiver FSM.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @return true
/// @return false
static bool check_code(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this);
    uint32_t code = fsm_rx_get_code(p_fsm_retina->p_fsm_rx);

    if (code != 0x00)
    {
        p_fsm_retina->rx_code = code;
        return true;
    }
    else
    {
        return false;
    }
}

/// @brief Check if there has been a repetition.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @return true
/// @return false
static bool check_repetition(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this);
    return fsm_rx_get_repetition(p_fsm_retina->p_fsm_rx);
}

/// @brief Check if there it been received an erroneous code.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @return true
/// @return false
static bool check_error(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this);
    return fsm_rx_get_error_code(p_fsm_retina->p_fsm_rx);
}

/// @brief Check if any of the elements of the system is active.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @return true
/// @return false
static bool check_activity(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this);
    bool cond1 = fsm_button_check_activity(p_fsm_retina->p_fsm_button);
    bool cond2 = fsm_tx_check_activity(p_fsm_retina->p_fsm_tx);
    bool cond3 = fsm_rx_check_activity(p_fsm_retina->p_fsm_rx);
    bool cond4 = fsm_sensor_check_activity(p_fsm_retina->p_fsm_sensor);
    return (cond1 | cond2 | cond3 | cond4);
}

/// @brief Check if all the is system active.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @return true
/// @return false
static bool check_no_activity(fsm_t *p_this)
{
    return !check_activity(p_this);
}

/// @brief Check if there is no light.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @return true
/// @return false

static bool check_no_light(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this);
    return (fsm_sensor_get_light(p_fsm_retina->p_fsm_sensor));
}

/// @brief Check if there is light.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
/// @return true
/// @return false

static bool check_light(fsm_t *p_this)
{
    return !check_no_light(p_this);
}

/* State machine output or action functions */

/// @brief Transmit the next code stored in memory after a short button press.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
static void do_send_next_msg(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    fsm_tx_set_code(p_fsm_retina->p_fsm_tx, p_fsm_retina->tx_codes_arr[p_fsm_retina->tx_codes_index]);
    fsm_button_reset_duration(p_fsm_retina->p_fsm_button);
    p_fsm_retina->tx_codes_index = (p_fsm_retina->tx_codes_index + 1) % COMMANDS_MEMORY_SIZE;
}

/// @brief Actuate according to the code received. This function can be used to act on the RGB LED or any other hardware.
/// @param p_this Pointer to an fsm_t struct than contains an fsm_retina_t.
static void do_execute_code(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    _process_rgb_code(p_fsm_retina->rgb_id, p_fsm_retina->rx_code, p_fsm_retina->buzzer_id);
    fsm_rx_reset_code(p_fsm_retina->p_fsm_rx);
}

/// @brief Switch the system to transmitter mode.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_retina_t.
static void do_rx_off_tx_on(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    fsm_rx_set_rx_status(p_fsm_retina->p_fsm_rx, false);
    port_rgb_set_color(p_fsm_retina->rgb_id, 0, 0, 0);
}

/// @brief Switch the system to receiver mode.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_retina_t.
static void do_tx_off_rx_on(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    fsm_rx_set_rx_status(p_fsm_retina->p_fsm_rx, true);
    _process_rgb_code(p_fsm_retina->rgb_id, p_fsm_retina->rx_code, p_fsm_retina->buzzer_id);
    fsm_button_reset_duration(p_fsm_retina->p_fsm_button);
}

/// @brief Actuate accordingly when receiving a repetition.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_retina_t.
static void do_execute_repetition(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    fsm_rx_reset_code(p_fsm_retina->p_fsm_rx);
}

/// @brief Discard and reset the code of the infrared receiver FSM.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_retina_t.
static void do_discard_rx_and_reset(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    fsm_rx_reset_code(p_fsm_retina->p_fsm_rx);
}

/// @brief Start the low power mode.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_retina_t.
static void do_sleep(fsm_t *p_this)
{
    port_system_sleep();
}

/// @brief Start the emergency signal.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_retina_t.
static void do_emergency(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    port_rgb_set_color(p_fsm_retina->rgb_id, HIGH, HIGH, HIGH);
    port_buzzer_pwm_timer_set(p_fsm_retina->rgb_id, 261.63);
}

/// @brief Start the emergency signal.
/// @param p_this 	Pointer to an fsm_t struct than contains an fsm_retina_t.
static void do_no_emergency(fsm_t *p_this)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    port_rgb_set_color(p_fsm_retina->rgb_id, LOW, HIGH, LOW);
    port_buzzer_pwm_timer_set(p_fsm_retina->rgb_id, 392);
}

/// @brief Array representing the transitions table of the FSM Retina.
static fsm_trans_t fsm_trans_retina[] = {
    {WAIT_TX, check_short_pressed, WAIT_TX, do_send_next_msg},
    {WAIT_TX, check_long_pressed, WAIT_RX, do_tx_off_rx_on},
    {WAIT_RX, check_code, WAIT_RX, do_execute_code},
    {WAIT_RX, check_repetition, WAIT_RX, do_execute_repetition},
    {WAIT_RX, check_error, WAIT_RX, do_discard_rx_and_reset},
    {WAIT_RX, check_long_pressed, WAIT_TX, do_rx_off_tx_on},
    {SLEEP_RX, check_activity, WAIT_RX, NULL},
    {SLEEP_RX, check_no_activity, SLEEP_RX, do_sleep},
    {WAIT_RX, check_no_activity, SLEEP_RX, do_sleep},
    {SLEEP_TX, check_activity, WAIT_TX, NULL},
    {SLEEP_TX, check_no_activity, SLEEP_TX, do_sleep},
    {WAIT_TX, check_no_activity, SLEEP_TX, do_sleep},

    /*ADDED IN MEJORAS*/
    {WAIT_RX, check_no_light, EMERGENCY, do_emergency},
    {EMERGENCY, check_light, WAIT_RX, do_no_emergency},
    /**/

    {-1, NULL, -1, NULL}

};

/* Other auxiliary functions */

fsm_t *fsm_retina_new(fsm_t *p_fsm_button, uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id, uint8_t buzzer_id, fsm_t *p_fsm_sensor)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_retina_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_retina_init(p_fsm, p_fsm_button, button_press_time, p_fsm_tx, p_fsm_rx, rgb_id, buzzer_id, p_fsm_sensor);
    return p_fsm;
}

void fsm_retina_init(fsm_t *p_this, fsm_t *p_fsm_button, uint32_t button_press_time, fsm_t *p_fsm_tx, fsm_t *p_fsm_rx, uint8_t rgb_id, uint8_t buzzer_id, fsm_t *p_fsm_sensor)
{
    fsm_retina_t *p_fsm_retina = (fsm_retina_t *)(p_this); // cast p_this
    fsm_init(p_this, fsm_trans_retina);
    p_fsm_retina->p_fsm_button = p_fsm_button;
    p_fsm_retina->p_fsm_tx = p_fsm_tx;
    p_fsm_retina->long_button_press_ms = button_press_time;
    p_fsm_retina->tx_codes_index = 0;
    p_fsm_retina->tx_codes_arr[0] = LIL_RED_BUTTON;
    p_fsm_retina->tx_codes_arr[1] = LIL_GREEN_BUTTON;
    p_fsm_retina->tx_codes_arr[2] = LIL_BLUE_BUTTON;
    p_fsm_retina->tx_codes_arr[3] = LIL_CYAN_BUTTON;
    p_fsm_retina->tx_codes_arr[4] = LIL_MAGENTA_BUTTON;
    p_fsm_retina->tx_codes_arr[5] = LIL_YELLOW_BUTTON;
    p_fsm_retina->tx_codes_arr[6] = LIL_WHITE_BUTTON;
    p_fsm_retina->tx_codes_arr[7] = LIL_OFF_BUTTON;
    p_fsm_retina->p_fsm_rx = p_fsm_rx;
    p_fsm_retina->rx_code = 0x00;
    p_fsm_retina->rgb_id = rgb_id;
    p_fsm_retina->buzzer_id = buzzer_id;
    p_fsm_retina->p_fsm_sensor = p_fsm_sensor;
    port_rgb_init(rgb_id);
    port_buzzer_init(buzzer_id);
}
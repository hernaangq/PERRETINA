/**
 * @file port_button.c
 * @brief File containing functions related to the HW of the button FSM.
 *
 * This files defines an internal struct which coontains the HW information of the button.
 *
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 23/03/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "port_button.h"
#include "port_sensor.h" /*!<Included in order to work with its interruption handler that lays within this file.*/

/* Typedefs --------------------------------------------------------------------*/

/// @brief Structure to define the HW dependencies of a button.
typedef struct
{
    GPIO_TypeDef *p_port; /*!< GPIO where the button is connected */
    uint8_t pin;          /*!< Pin/line where the button is connected */
    bool flag_pressed;    /*!< Flag to indicate that the button has been pressed. If it occurs in a rising or falling edge depends on how the function port_button_is_pressed implements it. */
} port_button_hw_t;

/* Global variables ------------------------------------------------------------*/

/// @brief Array of elements that represents the HW characteristics of the buttons.
static port_button_hw_t buttons_arr[] = {
    [BUTTON_0_ID] = {.p_port = BUTTON_0_GPIO, .pin = BUTTON_0_PIN, .flag_pressed = false},
};

void port_button_init(uint32_t button_id)
{
    GPIO_TypeDef *p_port1 = buttons_arr[button_id].p_port;
    uint8_t pin1 = buttons_arr[button_id].pin;

    port_system_gpio_config(p_port1, pin1, GPIO_MODE_IN, GPIO_PUPDR_NOPULL); // input, no-pulls
    port_system_gpio_config_exti(buttons_arr[button_id].p_port, buttons_arr[button_id].pin, (TRIGGER_BOTH_EDGE | TRIGGER_ENABLE_INTERR_REQ));
    port_system_gpio_exti_enable(buttons_arr[button_id].pin, 1, 0);
}

bool port_button_is_pressed(uint32_t button_id)
{
    return buttons_arr[button_id].flag_pressed;
}

uint32_t port_button_get_tick()
{
    return port_system_get_millis();
}

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief  This function handles Px10-Px15 global interrupts.
 *
 * First, this function identifies the line/ pin which has raised the interruption. Then, perform the desired action. Before leaving it cleans the interrupt pending register.
 *
 */
void EXTI15_10_IRQHandler(void)
{
    port_system_systick_resume();
    /* ISR user button in PC13 */
    if (EXTI->PR & BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin))
    {
        if (port_system_gpio_read(buttons_arr[BUTTON_0_ID].p_port, buttons_arr[BUTTON_0_ID].pin))
        {
            buttons_arr[BUTTON_0_ID].flag_pressed = false;
        }
        else
        {
            buttons_arr[BUTTON_0_ID].flag_pressed = true;
        }
        EXTI->PR |= BIT_POS_TO_MASK(buttons_arr[BUTTON_0_ID].pin); /* Limpiar flag , escribiendo un 1 */
    }

    if (EXTI->PR & BIT_POS_TO_MASK(SENSOR_0_PIN))
    {
        port_system_gpio_read(SENSOR_0_GPIO, SENSOR_0_PIN);
        EXTI->PR |= BIT_POS_TO_MASK(SENSOR_0_PIN); /* Limpiar flag , escribiendo un 1 */
    }
}

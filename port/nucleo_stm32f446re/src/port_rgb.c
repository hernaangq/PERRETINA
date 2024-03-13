/**
 * @file port_rgb.c
 * @brief Portable functions to interact with the infrared receiver FSM library.
 * @author Ángel Rodrigo Pérez Iglesias
 * @author Hernán García Quijano
 * @date 18/04/2023
 * */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include "port_rgb.h"
#include "port_system.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of an RGB LED.
 */
typedef struct
{
    GPIO_TypeDef *p_port_red;   // GPIO where the RED LED is connected
    uint8_t pin_red;            // Pin/line where the RED LED is connected
    GPIO_TypeDef *p_port_green; // GPIO where the GREEN LED is connected
    uint8_t pin_green;          // Pin/line where the GREEN LED is connected
    GPIO_TypeDef *p_port_blue;  // GPIO where the BLUE LED is connected
    uint8_t pin_blue;           // Pin/line where the BLUE LED is connected
} port_rgb_hw_t;

/* Global variables ------------------------------------------------------------*/
/**
 * @brief Array of elements that represents the HW characteristics of the RGB.
 */
static port_rgb_hw_t rgb_arr[] = {
    [RGB_0_ID] = {
        .p_port_red = RGB_R_0_GPIO,
        .pin_red = RGB_R_0_PIN,
        .p_port_green = RGB_G_0_GPIO,
        .pin_green = RGB_G_0_PIN,
        .p_port_blue = RGB_B_0_GPIO,
        .pin_blue = RGB_B_0_PIN,
    }};

void port_rgb_init(uint8_t rgb_id)
{
    port_system_gpio_config(RGB_R_0_GPIO, RGB_R_0_PIN, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);
    port_system_gpio_config(RGB_G_0_GPIO, RGB_G_0_PIN, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);
    port_system_gpio_config(RGB_B_0_GPIO, RGB_B_0_PIN, GPIO_MODE_OUT, GPIO_PUPDR_NOPULL);
    port_rgb_set_color(rgb_id, 0, 0, 0);
}

void port_rgb_set_color(uint8_t rgb_id, uint8_t r, uint8_t g, uint8_t b)
{
    port_system_gpio_write(rgb_arr[rgb_id].p_port_red, rgb_arr[rgb_id].pin_red, (bool)r);
    port_system_gpio_write(rgb_arr[rgb_id].p_port_green, rgb_arr[rgb_id].pin_green, (bool)g);
    port_system_gpio_write(rgb_arr[rgb_id].p_port_blue, rgb_arr[rgb_id].pin_blue, (bool)b);
}

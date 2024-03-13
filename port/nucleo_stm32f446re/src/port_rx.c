/**
 * @file port_rx.c
 * @brief Portable functions to interact with the infrared receiver FSM library.
 * @author Ángel Rodrigo Pérez Iglesias
 * @author Hernán García Quijano
 * @date 18/04/2023
 * */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <string.h> /* To use memset */

/* Other includes */
#include "port_rx.h"
#include "port_system.h"
#include "fsm_rx_nec.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the HW dependencies of an infrared receiver.
 */
typedef struct
{
  GPIO_TypeDef *p_port;                 // GPIO where the infrared transmitter is connected
  uint8_t pin;                          // Pin/line where the infrared transmitter is connected
  uint16_t edge_ticks[NEC_FRAME_EDGES]; // Array to store the time ticks of the edges detected by the infrared receiver. It size must be larger or equal than the number of expected edges of the NEC protocol.
  uint16_t edge_idx;                    // Index to go though the edge_ticks array.
} port_rx_hw_t;

/* Global variables ------------------------------------------------------------*/
/**
 * @brief Array of elements that represents the HW characteristics of the infrared receivers.
 */
static port_rx_hw_t receivers_arr[] = {
    [IR_RX_0_ID] = {.p_port = IR_RX_0_GPIO, .pin = IR_RX_0_PIN}};

/* Infrared receiver private functions */
/**
 * @brief Set the elements of the array of time ticks to '0' and init the index to '0' as well.
 *
 * > &nbsp;&nbsp;&nbsp;&nbsp;💡 To set all the elements of an array to one value, you can use function `memset`. You need a pointer to the array (its name), the value you want to set, and the length (in bytes) of the array (so you need to multiply the number of elements by the size of the type of the elements of the array). To use this function you need to include the <string.h> library.
 *
 * @param rx_id Receiver ID. This index is used to select the element of the `receivers_arr[]` array.
 */
static void _reset_edge_ticks_idx(uint8_t rx_id)
{
  memset(receivers_arr[rx_id].edge_ticks, 0, sizeof(uint16_t) * NEC_FRAME_EDGES);
  receivers_arr[rx_id].edge_idx = 0;
}

/// @brief Store the time tick of the last edge detected. This function is called by the ISR after an interruption of the GPIO.
/// @param rx_id 	Receiver ID. This index is used to select the element of the receivers_arr[] array.
static void _store_edge_tick(uint8_t rx_id)
{
  uint16_t edges_idx = receivers_arr[rx_id].edge_idx;
  if (port_system_gpio_read(receivers_arr[rx_id].p_port, receivers_arr[rx_id].pin) != (edges_idx & 1))
    return;
  if (edges_idx < NEC_FRAME_EDGES)
  {
    receivers_arr[rx_id].edge_ticks[edges_idx] = TIM3->CNT;
    receivers_arr[rx_id].edge_idx++;
  }
  else
  {
    return;
  }
}

/// @brief Configure the timer tick. This timer sets the basis for tick counting for checking received NEC symbols.
void _timer_rx_setup()
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  TIM3->CNT = 0;
  TIM3->ARR = 65535;
  TIM3->PSC = (SystemCoreClock * NEC_RX_TIMER_TICK_BASE_US / 1000000) - 1;
  TIM3->EGR = TIM_EGR_UG;
}

void port_rx_init(uint8_t rx_id)
{
  _timer_rx_setup();
  port_system_gpio_config(receivers_arr[rx_id].p_port, receivers_arr[rx_id].pin, GPIO_MODE_IN, GPIO_PUPDR_NOPULL);
  port_system_gpio_config_exti(receivers_arr[rx_id].p_port, receivers_arr[rx_id].pin, (TRIGGER_BOTH_EDGE | TRIGGER_ENABLE_INTERR_REQ));
  port_system_gpio_exti_enable(receivers_arr[rx_id].pin, 2, 0);
  _reset_edge_ticks_idx(rx_id);
}

void port_rx_en(uint8_t rx_id, bool interr_en)
{
  _reset_edge_ticks_idx(rx_id);
  if (interr_en)
  {
    port_system_gpio_exti_enable(receivers_arr[rx_id].pin, 2, 0);
  }
  else
  {
    port_system_gpio_exti_disable(receivers_arr[rx_id].pin);
  }
}

void port_rx_tmr_start()
{
  TIM3->CNT = 0;
  TIM3->CR1 |= TIM_CR1_CEN;
}

void port_rx_tmr_stop()
{
  TIM3->CR1 &= ~TIM_CR1_CEN;
}

uint32_t port_rx_get_num_edges(uint8_t rx_id)
{
  return receivers_arr[rx_id].edge_idx;
}

uint16_t *port_rx_get_buffer_edges(uint8_t rx_id)
{
  return (uint16_t *)(&(receivers_arr[rx_id].edge_ticks));
}

void port_rx_clean_buffer(uint8_t rx_id)
{
  _reset_edge_ticks_idx(rx_id);
}

/// @brief This function handles Px5-Px9 global interrupts.
void EXTI9_5_IRQHandler(void)
{
  port_system_systick_resume();
  if (EXTI->PR & BIT_POS_TO_MASK(receivers_arr[IR_RX_0_ID].pin))
  {
    EXTI->PR |= BIT_POS_TO_MASK(receivers_arr[IR_RX_0_ID].pin); /* Limpiar flag , escribiendo un 1 */
    _store_edge_tick(IR_RX_0_ID);
  }
}

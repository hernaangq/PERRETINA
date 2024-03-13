/**
 * @file port_rx.h
 * @brief Header for port_rx.c file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 18/04/2023
 */
#ifndef PORT_RX_H_
#define PORT_RX_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdbool.h>
#include <stdint.h>

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define IR_RX_0_ID 0       /*!< Infrared receiver identifier */
#define IR_RX_0_GPIO GPIOB /*!< Infrared receiver GPIO port */
#define IR_RX_0_PIN 6      /*!< Infrared receiver GPIO pin */

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief Return a pointer to de memory address of the array that stores the time ticks of the edges detected by the infrared receiver.
 *
 * @param rx_id Receiver ID. This index is used to select the element of the `receivers_arr[]` array.
 * @return uint16_t Pointer to the memory address of the array of time ticks.
 */
uint16_t *port_rx_get_buffer_edges(uint8_t rx_id);

/// @brief Configure the HW specifications of a given infrared receiver
/// @param rx_id Receiver ID. This index is used to select the element of the receivers_arr[] array
void port_rx_init(uint8_t rx_id);

/// @brief Enable/disable the interruptions of the infrared receiver.
/// @param rx_id Receiver ID. This index is used to select the element of the receivers_arr[] array
/// @param interr_en Argument to indicate if we want to enable or disable the interruptions of the infrared receiver
void port_rx_en(uint8_t rx_id, bool interr_en);

/// @brief Enable the tick count timer.
void port_rx_tmr_start();

/// @brief Disable the tick count timer.
void port_rx_tmr_stop();

/// @brief Return the number of edges detected by the infrared receiver so far.
/// @param rx_id Receiver ID. This index is used to select the element of the receivers_arr[] array
/// @return uint32_t Number of edges detected so far
uint32_t port_rx_get_num_edges(uint8_t rx_id);

/// @brief Clean the array that stores the time ticks of the edges detected by the infrared receiver.
/// @param rx_id Receiver ID. This index is used to select the element of the receivers_arr[] array
void port_rx_clean_buffer(uint8_t rx_id);

#endif

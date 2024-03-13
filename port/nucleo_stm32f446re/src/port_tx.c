/**
 * @file port_tx.c
 * @brief Portable functions to interact with the infrared transmitter FSM library.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 23/03/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "port_tx.h"
#include "fsm_tx.h"
#include <math.h>

/* Defines --------------------------------------------------------------------*/
#define ALT_FUNC1_TIM2    1 /*!< TIM2 Alternate Function mapping */

/* IMPORTANT
The timer symbol is the same for all the TX, so it is not in the structure of TX. It has been decided to be the TIM1. It is like a systick but faster.
*/

/* Typedefs --------------------------------------------------------------------*/

/// @brief Structure to define the HW dependencies of an infrared transmitter.
typedef struct
{
    GPIO_TypeDef *p_port; /*!< GPIO where the infrared transmitter is connected */
    uint8_t pin; /*!< Pin/line where the infrared transmitter is connected */
    uint8_t alt_func; /*!< Alternate function value according to the Alternate function table of the datasheet */

} port_tx_hw_t;


/* Global variables ------------------------------------------------------------*/

/// @brief Variable to store the count of ticks of the symbol timer
volatile uint32_t symbol_tick;


/// @brief Array of elements that represents the HW characteristics of the infrared transmitters.
static port_tx_hw_t transmitters_arr[] = {
  [IR_TX_0_ID] = {.p_port = IR_TX_0_GPIO, .pin = IR_TX_0_PIN, .alt_func = ALT_FUNC1_TIM2},
};

/* Infrared transmitter private functions */

/// @brief Configure the symbol timer. This timer sets the tick base as a reference for the symbols of the protocol.
static void _timer_symbol_setup()
{
  /* Primero , habilitamos siempre el reloj del timer */
  RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN ; /* TIM1 con APB2 . */

  /* Temporizacion */
  /* 1) Fuente de reloj : interno ( bits CKD =00) */
  TIM1 -> CR1 = 0x0000 ; /* Aunque es el valor por defecto */

  /* 2) Deshabilita el contador (no es indispensable). */
  TIM1 -> CR1 &= ~ TIM_CR1_CEN ;

  /* Habilita el preload (no es indispensable). */
  TIM1->CR1 |= TIM_CR1_ARPE;

  /* 3) Aseguramos inicio del contador a 0 */
  TIM1->CNT = 0;

  //CALCULOS
  uint32_t psmin =  (((SystemCoreClock * NEC_TX_TIMER_TICK_BASE_US /1000000))/(65536)) - 1 ;
  psmin = 0;
  uint32_t arrmin = (((SystemCoreClock * NEC_TX_TIMER_TICK_BASE_US / 1000000))/(psmin + 1)) - 1;
  
  /* 4) Cargamos el autorreload */
  TIM1->ARR = arrmin;

  /* 5) Cargamos el prescaler */
  TIM1->PSC = psmin;

  /* 6) ( IMPORTANTE ) Re - inicializa el contador y actualiza los registros .
  IMPORTANTE realizarlo despues de cargar ARR y PSC */
  /* Los valores de PSC y ARR estan en los registro de precarga . Para cargarlos
  en los registros activos , necesitamos que se actualice un evento . Esto
  podemos hacerlo manualmente actualizando el registro TIMx - >EGR , o esperar a
  que pase el timer . */
  TIM1->EGR = TIM_EGR_UG; /* Genera evento de actualizacion */

  /* Interrupciones */
  /* 7) Limpiamos las interrupciones (si hubiese ) */
  TIM1->SR = ~TIM_SR_UIF;

  /* 8) Habilitamos las interrupciones en el registro */
 TIM1->DIER |= TIM_DIER_UIE; /* Interrumpe al acabar de contar */

  /* Finally, set the priority and enable interruptions globally. */
  NVIC_SetPriority(TIM1_UP_TIM10_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0)); /* Priority 1, sub-priority 0 */
  NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);                                                          /* Enable interrupt */
}

/// @brief Configure the PWM timer. This timer configures the PWM for the NEC protocol.
/// @param tx_id Transmitter ID. If we have more than one transmitter, this value allows us to know which one of the we want to configure.
static void _timer_pwm_setup(uint32_t tx_id) /////AQUi
{

  if(tx_id==IR_TX_0_ID){ 

  /* Primero , habilitamos siempre el reloj del timer */
  RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN ; /* TIM2 con APB1 . */

  TIM2 -> CR1 =0x0000;

  /* 2) Deshabilita el contador (no es indispensable). */
  TIM2 -> CR1 &= ~ TIM_CR1_CEN ;

  /* Habilita el preload (no es indispensable). */
  TIM2->CR1 |= TIM_CR1_ARPE;

  /* 3) Aseguramos inicio del contador a 0 */
  TIM2->CNT = 0;

 //CALCULOS
  uint32_t psmin =   (((SystemCoreClock / NEC_PWM_FREQ_HZ))/(65536)) - 1 ;
  psmin =0 ;
  uint32_t arrmin =  (((SystemCoreClock / NEC_PWM_FREQ_HZ)/(psmin + 1)) - 1);
  uint32_t ccrxmin =  (NEC_PWM_DC*(arrmin+ 1));

  /* 4) Cargamos el autorreload */
  TIM2->ARR = arrmin;

  /* 5) Cargamos el prescaler */
  TIM2->PSC = psmin;

  TIM2->EGR = TIM_EGR_UG; /* Genera evento de actualizacion */

  //PWM mode configuration CHANNEL #3
  /* 7) Deshabilitar el output compare para hacer m o d i f i c a c i o n e s (no es
  in di sp en sa bl e ) */
  TIM2 -> CCER &= ~ TIM_CCER_CC3E;

  /* 8) Habilitamos modo PWM y habilitamos el preload */
  TIM2 -> CCMR2 |= TIM_CCMR2_OC3M; /* Modo PWM 1 */
  TIM2 -> CCMR2 |= TIM_CCMR2_OC3PE ; /* Habilita el preload */

  /* 9) Ancho del pulso de PWM */
  TIM2 -> CCR3 = ccrxmin;
  }
}

/* Public functions */

void port_tx_init(uint8_t tx_id, bool status){
  port_system_gpio_config(transmitters_arr[tx_id].p_port, transmitters_arr[tx_id].pin, GPIO_MODE_ALTERNATE, GPIO_PUPDR_NOPULL);
  port_system_gpio_config_alternate(transmitters_arr[tx_id].p_port, transmitters_arr[tx_id].pin, transmitters_arr[tx_id].alt_func);
  _timer_symbol_setup();
  _timer_pwm_setup(tx_id);
  port_tx_pwm_timer_set(tx_id, status);
}

/* In order to make the academic effort of configuring the PWM, the values: timer, channel and masks are hardcoded and this function is not generic for any timer/channel. It is not the best way, but it is as it. */

void port_tx_pwm_timer_set(uint8_t tx_id, bool status)
{
if(tx_id == IR_TX_0_ID){
  if(status){
    TIM2 -> CCER |= TIM_CCER_CC3E;
    TIM2 -> CR1 |= TIM_CR1_CEN ;
  }else{
    TIM2 -> CCER &= ~ TIM_CCER_CC3E;
    TIM2 -> CR1 &= ~ TIM_CR1_CEN ;
  }
  }
}

void port_tx_symbol_tmr_start()
{
  TIM1->CNT = 0;
  TIM1 -> CR1 |= TIM_CR1_CEN ;
  symbol_tick = 0;
}

void port_tx_symbol_tmr_stop()
{
  TIM1 -> CR1 &= ~ TIM_CR1_CEN ;
}

uint32_t port_tx_tmr_get_tick()
{
  return symbol_tick;
}

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------

/// @brief This function handles TIM1-TIM10 global interrupts.
void TIM1_UP_TIM10_IRQHandler(void)
{
  TIM1->SR &= ~TIM_SR_UIF;
  symbol_tick++;
}
/**
 * @file port_system.c
 * @brief File that defines the functions that are related to the access to the specific HW of the microcontroller.
 * @author Sistemas Digitales II
 * @date 2023-01-01
 */

/* Includes ------------------------------------------------------------------*/
#include "port_system.h"

/* Defines -------------------------------------------------------------------*/
#define HSI_VALUE ((uint32_t)16000000) /*!< Value of the Internal oscillator in Hz */

/* GLOBAL VARIABLES */
static uint32_t msTicks = 0; /*!< Variable to store millisecond ticks. @warning **It must be declared volatile!** Just because it is modified in an ISR. **Add it to the definition** after *static*. */

/* These variables are declared extern in CMSIS (system_stm32f4xx.h) */
uint32_t SystemCoreClock = HSI_VALUE;                                               /*!< Frequency of the System clock */
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9}; /*!< Prescaler values for AHB bus */
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};                          /*!< Prescaler values for APB bus */

//------------------------------------------------------
// SYSTEM CONFIGURATION
//------------------------------------------------------
/**
 * @brief  Setup the microcontroller system
 *         Initialize the FPU setting, vector table location and External memory
 *         configuration.
 *
 * @note   This function is called at startup by CMSIS in startup_stm32f446xx.s.
 */
void SystemInit(void)
{
/* FPU settings ------------------------------------------------------------*/
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */
#endif

#if defined(DATA_IN_ExtSRAM) || defined(DATA_IN_ExtSDRAM)
  SystemInit_ExtMemCtl();
#endif /* DATA_IN_ExtSRAM || DATA_IN_ExtSDRAM */

  /* Configure the Vector Table location -------------------------------------*/
#if defined(USER_VECT_TAB_ADDRESS)
  SCB->VTOR = VECT_TAB_BASE_ADDRESS | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#endif                                                 /* USER_VECT_TAB_ADDRESS */
}

/**
 * @brief System Clock Configuration
 *
 * @attention This function should NOT be accesible from the outside to avoid configuration problems.
 * @note This function starts a system timer that generates a SysTick every 1 ms.
 * @retval None
 */
static void system_clock_config(void)
{
  /** Configure the main internal regulator output voltage */
  /* Power controller (PWR) */
  /* Control the main internal voltage regulator output voltage to achieve a trade-off between performance and power consumption when the device does not operate at the maximum frequency */
  PWR->CR &= ~PWR_CR_VOS; // Clean and set value
  PWR->CR |= (PWR_CR_VOS & (POWER_REGULATOR_VOLTAGE_SCALE3 << PWR_CR_VOS_Pos));

  /* Initializes the RCC Oscillators. */
  /* Adjusts the Internal High Speed oscillator (HSI) calibration value.*/
  RCC->CR &= ~RCC_CR_HSITRIM; // Clean and set value
  RCC->CR |= (RCC_CR_HSITRIM & (RCC_HSI_CALIBRATION_DEFAULT << RCC_CR_HSITRIM_Pos));

  /* RCC Clock Config */
  /* Initializes the CPU, AHB and APB buses clocks */
  /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
      must be correctly programmed according to the frequency of the CPU clock
      (HCLK) and the supply voltage of the device. */

  /* Increasing the number of wait states because of higher CPU frequency */
  FLASH->ACR = FLASH_ACR_LATENCY_2WS; /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */

  /* Change in clock source is performed in 16 clock cycles after writing to CFGR */
  RCC->CFGR &= ~RCC_CFGR_SW; // Clean and set value
  RCC->CFGR |= (RCC_CFGR_SW & (RCC_CFGR_SW_HSI << RCC_CFGR_SW_Pos));

  /* Update the SystemCoreClock global variable */
  SystemCoreClock = HSI_VALUE >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos];

  /* Configure the source of time base considering new system clocks settings */
  SysTick_Config(SystemCoreClock / (1000U / TICK_FREQ_1KHZ)); /* Set Systick to 1 ms */
}

size_t port_system_init()
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  /* Configure Flash prefetch, Instruction cache, Data cache */
  /* Instruction cache enable */
  FLASH->ACR |= FLASH_ACR_ICEN;

  /* Data cache enable */
  FLASH->ACR |= FLASH_ACR_DCEN;

  /* Prefetch cache enable */
  FLASH->ACR |= FLASH_ACR_PRFTEN;

  /* Set Interrupt Group Priority */
  NVIC_SetPriorityGrouping(NVIC_PRIORITY_GROUP_4);

  /* Use systick as time base source and configure 1ms tick (default clock after Reset is HSI) */
  /* Configure the SysTick IRQ priority. It must be the highest (lower number: 0)*/
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0U, 0U)); /* Tick interrupt priority */

  /* Init the low level hardware */
  /* Reset and clock control (RCC) */
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; /* Syscfg clock enabling */

  /* Peripheral clock enable register */
  RCC->APB1ENR |= RCC_APB1ENR_PWREN; /* PWREN: Power interface clock enable */

  /* Configure the system clock */
  system_clock_config();

  return 0;
}

void port_system_power_stop()
{
  MODIFY_REG(PWR->CR, (PWR_CR_PDDS | PWR_CR_LPDS), PWR_CR_LPDS); // Select the regulator state in Stop mode: Set PDDS and LPDS bits according to PWR_Regulator value
  SCB->SCR |= ((uint32_t)SCB_SCR_SLEEPDEEP_Msk);                 // Set SLEEPDEEP bit of Cortex System Control Register
  __WFI();                                                       // Select Stop mode entry : Request Wait For Interrupt
  SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);                // Reset SLEEPDEEP bit of Cortex System Control Register
}

void port_system_sleep(void)
{
  port_system_systick_suspend();
  port_system_power_stop();
}

//------------------------------------------------------
// TIMER RELATED FUNCTIONS
//------------------------------------------------------

uint32_t port_system_get_millis()
{
  return msTicks;
}

void port_system_delay_ms(uint32_t ms)
{
  uint32_t tickstart = port_system_get_millis();

  while ((port_system_get_millis() - tickstart) < ms)
  {
  }
}

void port_system_delay_until_ms(uint32_t *p_t, uint32_t ms)
{
  uint32_t until = *p_t + ms;
  uint32_t now = port_system_get_millis();
  if (until > now)
  {
    port_system_delay_ms(until - now);
  }
  *p_t = port_system_get_millis();
}

void port_system_systick_suspend()
{
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void port_system_systick_resume()
{
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

//------------------------------------------------------
// GPIO RELATED FUNCTIONS
//------------------------------------------------------


void port_system_gpio_config(GPIO_TypeDef *p_port, uint8_t pin, uint8_t mode, uint8_t pupd)
{
  if (p_port == GPIOA)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
  }
  else if (p_port == GPIOB)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  }
  else if (p_port == GPIOC)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  }
  else if (p_port == GPIOD)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
  }
  else if (p_port == GPIOE)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
  }
  else if (p_port == GPIOF)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
  }
  else if (p_port == GPIOG)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
  }
  else if (p_port == GPIOH)
  {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
  }

  p_port->MODER &= ~(GPIO_MODER_MODER0 << 2 * pin);
  p_port->MODER |= (mode << 2 * pin);

  p_port->PUPDR &= ~(GPIO_MODER_MODER0 << 2 * pin);
  p_port->PUPDR |= (pupd << 2 * pin);
}


void port_system_gpio_config_exti(GPIO_TypeDef *p_port, uint8_t pin, uint32_t mode)
{

  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  uint32_t masc = 0;
  if (p_port == GPIOA)
  {
    masc = 0;
  }
  else if (p_port == GPIOB)
  {
    masc = 1;
  }
  else if (p_port == GPIOC)
  {
    masc = 2;
  }
  else if (p_port == GPIOD)
  {
    masc = 3;
  }
  else if (p_port == GPIOE)
  {
    masc = 4;
  }
  else if (p_port == GPIOF)
  {
    masc = 5;
  }
  else if (p_port == GPIOG)
  {
    masc = 6;
  }
  else if (p_port == GPIOH)
  {
    masc = 7;
  }

  if (pin > 0 && pin <= 3)
  {
    SYSCFG->EXTICR[0] &= ~(0x0FUL << 4 * (pin % 4)); 
    SYSCFG->EXTICR[0] |= (masc << 4 * (pin % 4));    
  }

  else if (4 <= pin && pin <= 7)
  {
    SYSCFG->EXTICR[1] &= ~(0x0FUL << 4 * (pin % 4)); 
    SYSCFG->EXTICR[1] |= (masc << 4 * (pin % 4));    
  }

  else if (8 <= pin && pin <= 11)
  {
    SYSCFG->EXTICR[2] &= ~(0x0FUL << 4 * (pin % 4)); 
    SYSCFG->EXTICR[2] |= (masc << 4 * (pin % 4));    
  }

  else if (12 <= pin && pin <= 15)
  {
    SYSCFG->EXTICR[3] &= ~(0x0FUL << 4 * (pin % 4)); 
    SYSCFG->EXTICR[3] |= (masc << 4 * (pin % 4));    
  }

  if ((mode & TRIGGER_RISING_EDGE))
  {
    EXTI->RTSR |= BIT_POS_TO_MASK(pin); 
  }
  if (mode & TRIGGER_FALLING_EDGE)
  {
    EXTI->FTSR |= BIT_POS_TO_MASK(pin); 
  }

  // Seleccionamos entre EVENT o INT dependiendo de mode
  if (mode & TRIGGER_ENABLE_EVENT_REQ)
  {
    EXTI->EMR |= BIT_POS_TO_MASK(pin); 
  }
  if (mode & TRIGGER_ENABLE_INTERR_REQ)
  {
    EXTI->IMR |= BIT_POS_TO_MASK(pin); 
  }
}

void port_system_gpio_exti_enable(uint8_t pin, uint8_t priority, uint8_t subpriority)
{
  NVIC_SetPriority(GET_PIN_IRQN(pin), NVIC_EncodePriority(NVIC_GetPriorityGrouping(), priority, subpriority)); // establecer el nivel de prioridad y subprioridad
  NVIC_EnableIRQ(GET_PIN_IRQN(pin));                                                                           // habilitar la interrupci´on de una l´ınea externa dada
}

void port_system_gpio_exti_disable(uint8_t pin)
{
  NVIC_DisableIRQ(GET_PIN_IRQN(pin)); 
}

// EDITADO
bool port_system_gpio_read(GPIO_TypeDef *p_port, uint8_t pin)
{
  bool value = (bool)(p_port->IDR & BIT_POS_TO_MASK(pin));
  return value;
}

// EDITADO
void port_system_gpio_write(GPIO_TypeDef *p_port, uint8_t pin, bool value)
{
  if (value)
  {
    p_port->BSRR = BIT_POS_TO_MASK(pin); 
  }
  else
  {
    p_port->BSRR = BIT_POS_TO_MASK(pin) << 16;
  }
}

void port_system_gpio_toggle(GPIO_TypeDef *p_port, uint8_t pin)
{
  if (port_system_gpio_read(p_port, pin))
  {
    port_system_gpio_write(p_port, pin, LOW);
  }
  else
  {
    port_system_gpio_write(p_port, pin, HIGH);
  }
}

void port_system_gpio_config_alternate(GPIO_TypeDef *p_port, uint8_t pin, uint8_t alternate)
{

  if (pin > 0 && pin <= 7)
  {
    p_port->AFR[0] &= ~(0x0FUL << 4 * (pin % 8));   
    p_port->AFR[0] |= (alternate << 4 * (pin % 8)); 
  }
  else if (pin >= 8 && pin <= 15)
  {
    p_port->AFR[1] &= ~(0x0FUL << 4 * (pin % 8));   
    p_port->AFR[1] |= (alternate << 4 * (pin % 8)); 
  }
}

//------------------------------------------------------
// INTERRUPT SERVICE ROUTINES
//------------------------------------------------------
/**
 * @brief This function handles the System tick timer that increments the system millisecond counter (global variable).
 * @warning **The variable `msTicks` must be declared volatile!** Just because it is modified in this ISR, in order to avoid [*race conditions*](https://en.wikipedia.org/wiki/Race_condition)
. **Add it to the definition** after *static*.
 *
 */
void SysTick_Handler(void)
{
  msTicks++;
}

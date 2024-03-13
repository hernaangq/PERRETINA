/**
 * @file commands.h
 * @brief Commands definition for the different remotes used in the system using the NEC protocol
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 23/03/2023
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
/* Device: Liluco IR remote */
/* The Liluco IR remote and receiver work on NEC protocol */
#define LIL_ON_BUTTON 16236607      /*!< Liluco IR remote command for button ON */
#define LIL_OFF_BUTTON 16203967     /*!< Liluco IR remote command for button OFF */
#define LIL_RED_BUTTON 0x00F720DF   /*!< Liluco IR remote command for button RED */
#define LIL_GREEN_BUTTON 0x00F7A05F /*!< Liluco IR remote command for button GREEN */
#define LIL_BLUE_BUTTON 0x00F7609F  /*!< Liluco IR remote command for button BLUE */
#define LIL_WHITE_BUTTON 16244767   /*!< Liluco IR remote command for button WHITE */
#define LIL_YELLOW_BUTTON 16197847  /*!< Liluco IR remote command for button YELLOW */
#define LIL_CYAN_BUTTON 16230487    /*!< Liluco IR remote command for button CYAN */
#define LIL_MAGENTA_BUTTON 16214167 /*!< Liluco IR remote command for button MAGENTA */

#define LIL_FLASH_BUTTON  16240687  /*!< Liluco IR remote command for button FLASH */
#define LIL_STROBE_BUTTON 16248847  /*!< Liluco IR remote command for button STROBE */
#define LIL_FADE_BUTTON 16238647    /*!< Liluco IR remote command for button FADE */
#define LIL_SMOOTH_BUTTON 16246807 /*!< Liluco IR remote command for button SMOOTH */



#define LIL_NUMBER_OF_BUTTONS 9 /*!< Number of Liluco IR remote commands*/

/* Other devices */
#endif /* COMMANDS_H_ */

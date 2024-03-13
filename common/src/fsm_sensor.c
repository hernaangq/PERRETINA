/**
 * @file fsm_sensor.c
 * @brief Sensor FSM main file.
 * @author Hernán García Quijano
 * @author Ángel Rodrigo Pérez Iglesias
 * @date 18/05/2023
 */

/* Includes ------------------------------------------------------------------*/
#include "fsm_sensor.h"
#include "port_sensor.h"

/* Typedefs --------------------------------------------------------------------*/
/**
 * @brief Structure to define the Sensor FSM.
 */
typedef struct fsm_sensor
{
    fsm_t f;            /*!<Sensor FSM*/
    bool light;         /*!<If there is light or not*/
    uint32_t sensor_id; /*!<Sensor ID. Must be unique.*/

} fsm_sensor_t;

/* Defines and enums ----------------------------------------------------------*/
/* Enums */
enum
{
    LIGHT, /*!< State where the system checks the presence of light.*/
};

/* State machine input or transition functions */

/// @brief Check if there is no light
/// @param p_this Pointer to an fsm_t struct than contains an fsm_sensor_t.
/// @return true
/// @return false
static bool check_no_light(fsm_t *p_this)
{
    fsm_sensor_t *p_fsm = (fsm_sensor_t *)(p_this); // casteo de p_this
    p_fsm->light = (port_sensor_get_value(p_fsm->sensor_id) > SENSOR_LIMIT);
    return (p_fsm->light);
}

/// @brief Array representing the transitions table of the FSM sensor.
static fsm_trans_t fsm_trans_sensor[] = {
    {LIGHT, check_no_light, LIGHT, NULL},
    {-1, NULL, -1, NULL}};

/* Other auxiliary functions */

bool fsm_sensor_get_light(fsm_t *p_this)
{
    fsm_sensor_t *p_fsm = (fsm_sensor_t *)(p_this); // cast p_this
    return (p_fsm->light);
}

fsm_t *fsm_sensor_new(uint32_t sensor_id)
{
    fsm_t *p_fsm = malloc(sizeof(fsm_sensor_t)); /* Do malloc to reserve memory of all other FSM elements, although it is interpreted as fsm_t (the first element of the structure) */
    fsm_sensor_init(p_fsm, sensor_id);
    return p_fsm;
}

void fsm_sensor_init(fsm_t *p_this, uint32_t sensor_id)
{

    fsm_sensor_t *p_fsm = (fsm_sensor_t *)(p_this);
    fsm_init(p_this, fsm_trans_sensor);

    p_fsm->sensor_id = sensor_id;

    port_sensor_init(sensor_id);
}

bool fsm_sensor_check_activity(fsm_t *p_this)
{
    fsm_sensor_t *p_fsm = (fsm_sensor_t *)(p_this);
    return (port_sensor_get_value(p_fsm->sensor_id) != 0);
}

#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include "include/relay_device.h"

// 0: NC -> PUMP VCC & COM IN2, NO -> NOT USED
// 1 : NC -> G3, NO -> COM IN3
// 2 : NC -> G2, NO -> G1

typedef enum
{
    PUMP_OFF = 0,
    PUMP_G1,
    PUMP_G2,
    PUMP_G3,
} pump_state_t;

typedef enum
{
    OVERHEATING_OFF = 0,
    OVERHEATING_ON,
} overheating_t;

typedef struct
{
    relay_device_t *relay_device;
    pump_state_t pump_state;
    overheating_t overheating;
} relay_controller_t;

void relay_controller_init(relay_controller_t **self);
void relay_controller_deinit(relay_controller_t *self);

void relay_controller_set_overheat_state(relay_controller_t *self, overheating_t state);
void relay_controller_set_pump_state(relay_controller_t *self, pump_state_t state);

overheating_t relay_controller_get_overheat_state(relay_controller_t *self);
pump_state_t relay_controller_get_pump_state(relay_controller_t *self);

#endif
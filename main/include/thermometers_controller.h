#ifndef THERMOMETERS_CONTROLLER_H
#define THERMOMETERS_CONTROLLER_H

#include "driver/gpio.h"
#include "ds18x20.h"

#define N_THERMOMETERS 2

typedef struct
{
    gpio_num_t one_wire_pin;
    onewire_addr_t addrs[N_THERMOMETERS];
} thermometers_device_t;

typedef thermometers_device_t thermometers_controller_t;

void thermometers_controller_init(thermometers_controller_t **self);
void thermometers_controller_deinit(thermometers_controller_t *self);

void thermometers_controller_measurements(thermometers_controller_t *self);

#endif
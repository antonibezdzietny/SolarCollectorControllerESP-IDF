#ifndef THERMOMETERS_DEVICE_H
#define THERMOMETERS_DEVICE_H

#include "driver/gpio.h"
#include "ds18x20.h"

#define N_THERMOMETERS 2

typedef struct
{
    gpio_num_t one_wire_pin;
    int n_devices;
    onewire_addr_t addrs[N_THERMOMETERS];
} thermometers_device_t;

#endif
#ifndef RELAY_DEVICE_H
#define RELAY_DEVICE_H

#include "driver/gpio.h"

typedef struct
{
    gpio_num_t in_pin[4];
} relay_device_t;

void relay_device_init(relay_device_t **self, relay_device_t *relay_device);
void relay_device_deinit(relay_device_t *self);

bool relay_device_get_state(relay_device_t *self, int chanel);
void relay_device_set_state(relay_device_t *self, int chanel, bool state);

#endif
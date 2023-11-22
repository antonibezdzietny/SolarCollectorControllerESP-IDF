#include "include/relay_controller.h"
#include "include/devices_model.h"

static void _relay_controller_device_default_init(relay_controller_t *self);

static void _relay_controller_pump_off(relay_controller_t *self);
static void _relay_controller_pump_g1(relay_controller_t *self);
static void _relay_controller_pump_g2(relay_controller_t *self);
static void _relay_controller_pump_g3(relay_controller_t *self);

void relay_controller_init(relay_controller_t **self)
{
    (*self) = (relay_controller_t *)calloc(1, sizeof(relay_controller_t));

    _relay_controller_device_default_init((*self));

    relay_controller_set_pump_state((*self), PUMP_OFF);
    relay_controller_set_overheat_state((*self), OVERHEATING_OFF);
}

void relay_controller_deinit(relay_controller_t *self)
{
    free(self);
}

void relay_controller_set_overheat_state(relay_controller_t *self, overheating_t state)
{
    switch (state)
    {
    case OVERHEATING_OFF:
        relay_device_set_state(self->relay_device, 3, 0);
        break;
    case OVERHEATING_ON:
        relay_device_set_state(self->relay_device, 3, 1);
        break;
    }

    self->overheating = state;
}

void relay_controller_set_pump_state(relay_controller_t *self, pump_state_t state)
{
    switch (state)
    {
    case PUMP_OFF:
        _relay_controller_pump_off(self);
        break;
    case PUMP_G1:
        _relay_controller_pump_g1(self);
        break;
    case PUMP_G2:
        _relay_controller_pump_g2(self);
        break;
    case PUMP_G3:
        _relay_controller_pump_g3(self);
        break;
    }

    self->pump_state = state;
}

static void _relay_controller_device_default_init(relay_controller_t *self)
{
    relay_device_t relay_device_def = devices_model_get_relay();
    relay_device_init(&self->relay_device, &relay_device_def);
}

static void _relay_controller_pump_off(relay_controller_t *self)
{
    relay_device_set_state(self->relay_device, 0, 1); // Turn off device
}
static void _relay_controller_pump_g1(relay_controller_t *self)
{
    relay_device_set_state(self->relay_device, 1, 1);
    relay_device_set_state(self->relay_device, 2, 1);
    relay_device_set_state(self->relay_device, 0, 0); // Turn on device
}
static void _relay_controller_pump_g2(relay_controller_t *self)
{
    relay_device_set_state(self->relay_device, 1, 1);
    relay_device_set_state(self->relay_device, 2, 0);
    relay_device_set_state(self->relay_device, 0, 0); // Turn on device
}
static void _relay_controller_pump_g3(relay_controller_t *self)
{
    relay_device_set_state(self->relay_device, 1, 0);
    relay_device_set_state(self->relay_device, 0, 0); // Turn on device
}

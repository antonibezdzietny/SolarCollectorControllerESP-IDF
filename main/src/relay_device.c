#include "include/relay_device.h"

static void _relay_device_gpio_config(relay_device_t *self);

void relay_device_init(relay_device_t **self,
                       relay_device_t *relay_device_gpio)
{
    (*self) = (relay_device_t *)calloc(1, sizeof(relay_device_t));

    *(*self) = *relay_device_gpio;

    _relay_device_gpio_config((*self));
}

void relay_device_deinit(relay_device_t *self)
{
    free(self);
}

void relay_device_set_state(relay_device_t *self, int chanel, bool state)
{
    gpio_set_level(self->in_pin[chanel], !state);
}

bool relay_device_get_state(relay_device_t *self, int chanel)
{
    return !gpio_get_level(self->in_pin[chanel]);
}

static void _relay_device_gpio_config(relay_device_t *self)
{

    gpio_config_t inx_gpio_config = {
        .pin_bit_mask = ((1ULL << self->in_pin[0]) |
                         (1ULL << self->in_pin[1]) |
                         (1ULL << self->in_pin[2]) |
                         (1ULL << self->in_pin[3])),
        .mode = GPIO_MODE_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&inx_gpio_config);
}
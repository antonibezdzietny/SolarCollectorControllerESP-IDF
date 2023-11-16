#include "include/joystick_device.h"

static void _joystick_device_sw_config(joystick_device_t *self);
static void _joystick_device_adc_config(joystick_device_t *self);
static void _joystick_device_gpio_config(joystick_device_t *self);

void joystick_device_init(joystick_device_t **self,
                          joystick_device_config_t *joystick_device_config)
{
    (*self) = (joystick_device_t *)calloc(1, sizeof(joystick_device_t));

    (*self)->config = *joystick_device_config;

    _joystick_device_gpio_config((*self));
}

void joystick_device_deinit(joystick_device_t *self)
{
    if (!self->sw_handle.handle)
        ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_isr_handler_remove(self->config.sw_pin));
    free(self);
}

void joystick_device_set_isr(joystick_device_t *self,
                             joystick_device_handle_t *joystick_sw_handle)
{
    self->sw_handle = *joystick_sw_handle; // Be careful copy void *handle_arg
    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_isr_handler_add(self->config.sw_pin,
                                                       self->sw_handle.handle,
                                                       self->sw_handle.handle_arg));
}

void joystick_device_get_x(joystick_device_t *self, int *value)
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_read(self->config.x_adc_config.handle,
                                                   self->config.x_adc_config.channel,
                                                   value));
}

void joystick_device_get_y(joystick_device_t *self, int *value)
{
    ESP_ERROR_CHECK_WITHOUT_ABORT(adc_oneshot_read(self->config.y_adc_config.handle,
                                                   self->config.y_adc_config.channel,
                                                   value));
}

static void _joystick_device_sw_config(joystick_device_t *self)
{
    gpio_config_t config_sw = {
        .pin_bit_mask = 1ULL << self->config.sw_pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE, // Interrupt on Rising Edge
    };

    gpio_config(&config_sw);
}

static void _joystick_device_adc_config(joystick_device_t *self)
{
    adc_oneshot_chan_cfg_t adc_config = {
        // Config for both chanel
        .atten = ADC_ATTEN_DB_11,
        .bitwidth = ADC_BITWIDTH_11,
    };

    /* Initialization VRX ADC pin*/
    adc_oneshot_unit_init_cfg_t config_vrx = {
        .unit_id = self->config.x_adc_config.unit,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    adc_oneshot_new_unit(&config_vrx,
                         &self->config.x_adc_config.handle);

    adc_oneshot_config_channel(self->config.x_adc_config.handle,
                               self->config.x_adc_config.channel,
                               &adc_config);

    /* Initialization VRY ADC pin*/
    if (self->config.x_adc_config.unit != self->config.y_adc_config.unit)
    {
        adc_oneshot_unit_init_cfg_t config_vry = {
            .unit_id = self->config.y_adc_config.unit,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
        };

        adc_oneshot_new_unit(&config_vry,
                             &self->config.y_adc_config.handle);
    }
    else
    {
        self->config.y_adc_config.handle = self->config.x_adc_config.handle;
    }

    adc_oneshot_config_channel(self->config.y_adc_config.handle,
                               self->config.y_adc_config.channel,
                               &adc_config);
}

static void _joystick_device_gpio_config(joystick_device_t *self)
{
    _joystick_device_sw_config(self);
    _joystick_device_adc_config(self);
}
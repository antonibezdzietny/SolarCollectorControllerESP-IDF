#include "include/devices_model.h"

#include <string.h>
#include "sdkconfig.h"

static joystick_device_config_t *_joystick_config = NULL;

static void _joystick_model_init(void);

void devices_model_init(void)
{
    _joystick_model_init();
}

joystick_device_config_t devices_model_get_joystick(void)
{
    return *_joystick_config;
}

static void _joystick_model_init(void)
{
    if (!(_joystick_config == NULL))
        return;

    _joystick_config = calloc(1, sizeof(joystick_device_config_t));

    joystick_device_config_t joystick_config_default = {
        .sw_pin = CONFIG_SW_PIN,
        .x_adc_config = {
            .handle = NULL,
            .unit = CONFIG_X_ADC_UNIT,
            .channel = CONFIG_X_ADC_CHANNEL,
        },
        .y_adc_config = {
            .handle = NULL,
            .unit = CONFIG_Y_ADC_UNIT,
            .channel = CONFIG_Y_ADC_CHANNEL,
        }};

    memcpy(_joystick_config, &joystick_config_default, sizeof(joystick_device_config_t));
}
#include "include/devices_model.h"

#include <string.h>
#include "sdkconfig.h"

static display_device_config_t *_display_config = NULL;
static joystick_device_config_t *_joystick_config = NULL;
static thermometers_device_t *_thermometers_config = NULL;
static relay_device_t *_relay_device = NULL;

static void _display_model_init(void);
static void _joystick_model_init(void);
static void _thermometers_model_init(void);
static void _relay_model_init(void);

void devices_model_init(void)
{
    _display_model_init();
    _joystick_model_init();
    _thermometers_model_init();
    _relay_model_init();
}

joystick_device_config_t devices_model_get_joystick(void)
{
    return *_joystick_config;
}

display_device_config_t devices_model_get_display(void)
{
    return *_display_config;
}

thermometers_device_t devices_model_get_thermometers(void)
{
    return *_thermometers_config;
}

relay_device_t devices_model_get_relay(void)
{
    return *_relay_device;
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

static void _display_model_init(void)
{
    if (!(_display_config == NULL))
        return;

    _display_config = (display_device_config_t *)calloc(1, sizeof(display_device_config_t));

    display_device_config_t display_config_default = {
        .rs = CONFIG_DISPLAY_RS,
        .en = CONFIG_DISPLAY_EN,
        .d4 = CONFIG_DISPLAY_D4,
        .d5 = CONFIG_DISPLAY_D5,
        .d6 = CONFIG_DISPLAY_D6,
        .d7 = CONFIG_DISPLAY_D7,
    };

    memcpy(_display_config, &display_config_default, sizeof(display_device_config_t));
}

static void _thermometers_model_init(void)
{
    if (!(_thermometers_config == NULL))
        return;

    _thermometers_config = (thermometers_device_t *)calloc(1, sizeof(thermometers_device_t));

    thermometers_device_t thermometers_config_default = {
        .one_wire_pin = CONFIG_ONE_WIRE_PIN,
        .n_devices = N_THERMOMETERS,
        .addrs = {CONFIG_COLLECTOR_ONE_W_ADDRESS, CONFIG_PIPE_ONE_W_ADDRESS}};

    memcpy(_thermometers_config, &thermometers_config_default, sizeof(thermometers_device_t));
}

static void _relay_model_init(void)
{
    if (!(_relay_device == NULL))
        return;

    _relay_device = (relay_device_t *)calloc(1, sizeof(relay_device_t));

    relay_device_t relay_device = {
        .in_pin = {CONFIG_RELAY_IN_1_PIN,
                   CONFIG_RELAY_IN_2_PIN,
                   CONFIG_RELAY_IN_3_PIN,
                   CONFIG_RELAY_IN_4_PIN},
    };

    memcpy(_relay_device, &relay_device, sizeof(relay_device_t));
}
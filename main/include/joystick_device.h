#ifndef JOYSTICK_DEVICE_H
#define JOYSTICK_DEVICE_H

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

typedef struct
{
    gpio_num_t pin;
    adc_unit_t unit;
    adc_channel_t channel;
    adc_oneshot_unit_handle_t handle;
} joystick_device_adc_t;

typedef struct
{
    gpio_num_t sw_pin;
    joystick_device_adc_t y_adc_config;
    joystick_device_adc_t x_adc_config;
} joystick_device_config_t;

typedef struct
{
    gpio_isr_t handle;
    void *handle_arg;
} joystick_device_handle_t;

typedef struct
{
    joystick_device_config_t config;
    joystick_device_handle_t sw_handle;
} joystick_device_t;

/* Public interfaces for Joystick */

/* Init structure by passing config*/
void joystick_device_init(joystick_device_t **self,
                          joystick_device_config_t *joystick_device_config);
/* Free memory and disable isr*/
void joystick_device_deinit(joystick_device_t *self);
/* Init isr for sw handle IMPORTANT: Before use gpio_install_isr_service()*/
void joystick_device_set_isr(joystick_device_t *self,
                             joystick_device_handle_t *joystick_device_handle);
/* Get joystick x value*/
void joystick_device_get_x(joystick_device_t *self, int *value);
/* Get joystick y value*/
void joystick_device_get_y(joystick_device_t *self, int *value);

#endif
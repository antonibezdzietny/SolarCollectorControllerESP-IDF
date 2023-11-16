#ifndef JOYSTICK_CONTROLLER_H
#define JOYSTICK_CONTROLLER_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "include/joystick_device.h"

typedef void (*joystick_controller_handle_t)(void *arg);

typedef struct
{
    int retrieve_frequency;
    int retrieve_time_s;
} joystick_controller_retrieve_time_t;

typedef struct
{
    void *arg;
    joystick_controller_handle_t sw_pressed;
    joystick_controller_handle_t x_up;
    joystick_controller_handle_t x_down;
    joystick_controller_handle_t y_up;
    joystick_controller_handle_t y_down;
    joystick_controller_handle_t on_delete;

} joystick_controller_handles_t;

typedef struct
{
    joystick_controller_retrieve_time_t retrieve_time;
    joystick_controller_handles_t handles;
} joystick_controller_retrieve_config_t;

typedef struct
{
    joystick_device_t *joystick_device;
    joystick_controller_handles_t handles;
    joystick_controller_retrieve_time_t retrieve_time;
    QueueHandle_t _queue_sw_handle;
} joystick_controller_t;

/* Public interfaces for Joystick Controller */
void joystick_controller_init(joystick_controller_t **self);
void joystick_controller_deinit(joystick_controller_t *self);

void joystick_controller_set_retrieve_time(
    joystick_controller_t *self,
    joystick_controller_retrieve_time_t retrieve_time);
void joystick_controller_set_handles(joystick_controller_t *self,
                                     joystick_controller_handles_t handles);
void joystick_controller_run_retrieve(joystick_controller_t *self);
#endif
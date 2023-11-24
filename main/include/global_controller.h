#ifndef GLOBAL_CONTROLLER_H
#define GLOBAL_CONTROLLER_H

#include "include/relay_controller.h"
#include "include/thermometers_controller.h"
#include "include/user_io_controller.h"
#include "include/wifi_controller.h"

typedef struct
{
    user_io_controller_t *user_io_controller;
    relay_controller_t *relay_controller;
    thermometers_controller_t *thermometers_controller;
    wifi_controller_t *wifi_controller;
} global_controller_t;

void global_controller_init(global_controller_t **self);
void global_controller_deinit(global_controller_t *self);

void global_controller_run(global_controller_t *self);
void global_controller_run_logger(global_controller_t *self);

#endif
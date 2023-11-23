#ifndef GLOBAL_CONTROLLER_H
#define GLOBAL_CONTROLLER_H

#include "include/relay_controller.h"
#include "include/thermometers_controller.h"
#include "include/user_io_controller.h"

typedef struct
{
    user_io_controller_t *user_io_controller;
    relay_controller_t *relay_controller;
} global_controller_t;

#endif
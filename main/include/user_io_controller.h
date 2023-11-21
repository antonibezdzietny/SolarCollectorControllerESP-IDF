#ifndef USER_IO_CONTROLLER_H
#define USER_IO_CONTROLLER_H

#include "include/user_io_state.h"
#include "include/display_controller.h"
#include "include/joystick_controller.h"

typedef struct
{
    display_controller_t *display_controller;
    joystick_controller_t *joystick_controller;
    state_t state;
} user_io_controller_t;

void user_io_controller_init(user_io_controller_t **self);
void user_io_controller_display_refresh(user_io_controller_t *self);
#endif
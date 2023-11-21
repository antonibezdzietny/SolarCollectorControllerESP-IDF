#include "include/user_io_normal.h"
#include "include/user_io_menu.h"

joystick_controller_handle_t sw_pressed;
joystick_controller_handle_t x_up;
joystick_controller_handle_t x_down;
joystick_controller_handle_t y_up;
joystick_controller_handle_t y_down;
joystick_controller_handle_t on_delete;

static void _user_io_normal_sw_pressed(void *arg);
static void _user_io_normal_x_up(void *arg);
static void _user_io_normal_x_down(void *arg);
static void _user_io_normal_on_delete(void *arg);

void user_io_normal_set(user_io_controller_t *user_io_controller)
{
    // Change state
    user_io_controller->state.state_type = STATE_NORMAL;

    // Set handles
    user_io_controller->state.joystick_handles.sw_pressed = _user_io_normal_sw_pressed;
    user_io_controller->state.joystick_handles.x_up = _user_io_normal_x_up;
    user_io_controller->state.joystick_handles.x_down = _user_io_normal_x_down;
    user_io_controller->state.joystick_handles.on_delete = _user_io_normal_on_delete;
    user_io_controller->state.joystick_handles.arg = (void *)user_io_controller;

    joystick_controller_set_handles(user_io_controller->joystick_controller,
                                    user_io_controller->state.joystick_handles);
}

static void _user_io_normal_sw_pressed(void *arg)
{
    printf("IO NORMAL:  sw pressed\n");
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;

    // Pass argument
    user_io_controller->state.state_arg.config_idx = 0;
    user_io_menu_set(user_io_controller);
}

static void _user_io_normal_x_up(void *arg)
{
    // ! Here not used
    printf("IO NORMAL:  x up \n");
}

static void _user_io_normal_x_down(void *arg)
{
    // ! Here not used
    printf("IO NORMAL:  x down \n");
}

static void _user_io_normal_on_delete(void *arg)
{
    // In normal mode should be not used
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;
    user_io_normal_set(user_io_controller);
}
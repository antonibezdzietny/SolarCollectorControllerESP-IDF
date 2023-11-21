#include "include/user_io_config.h"
#include "include/user_io_normal.h"
#include "include/global_model.h"

static void _user_io_config_sw_pressed(void *arg);
static void _user_io_config_x_up(void *arg);
static void _user_io_config_x_down(void *arg);
static void _user_io_config_on_delete(void *arg);

static void _user_io_config_display(user_io_controller_t *user_io_controller);

void user_io_config_set(user_io_controller_t *user_io_controller)
{
    // Change state
    user_io_controller->state.state_type = STATE_CONFIG;

    // Set handles
    user_io_controller->state.joystick_handles.sw_pressed = _user_io_config_sw_pressed;
    user_io_controller->state.joystick_handles.x_up = _user_io_config_x_up;
    user_io_controller->state.joystick_handles.x_down = _user_io_config_x_down;
    user_io_controller->state.joystick_handles.on_delete = _user_io_config_on_delete;
    user_io_controller->state.joystick_handles.arg = (void *)user_io_controller;

    // Load
    user_io_controller->state.state_arg.user_io_state.value = global_model_get_config(
        (config_model_type_t)user_io_controller->state.state_arg.user_io_state.config_idx);

    joystick_controller_retrieve_time_t joystick_controller_retrieve_time = {
        .retrieve_frequency = 4,
        .retrieve_time_s = 10,
    };

    joystick_controller_set_retrieve_time(user_io_controller->joystick_controller,
                                          joystick_controller_retrieve_time);

    joystick_controller_set_handles(user_io_controller->joystick_controller,
                                    user_io_controller->state.joystick_handles);

    joystick_controller_run_retrieve(user_io_controller->joystick_controller);

    _user_io_config_display(user_io_controller);
}

static void _user_io_config_sw_pressed(void *arg)
{
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;
    config_model_type_t type = (config_model_type_t)user_io_controller->state.state_arg.user_io_state.config_idx;

    if (global_model_get_config(type) != user_io_controller->state.state_arg.user_io_state.value)
        global_model_set_config(type, user_io_controller->state.state_arg.user_io_state.value);

    user_io_controller->state.state_arg.config_idx = 0;
    user_io_normal_set(user_io_controller);
}

static void _user_io_config_x_up(void *arg)
{
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;
    config_model_type_t type = user_io_controller->state.state_arg.user_io_state.config_idx;

    if (global_model_get_max_value(type) <= user_io_controller->state.state_arg.user_io_state.value)
        return;

    user_io_controller->state.state_arg.user_io_state.value += 1;

    _user_io_config_display(user_io_controller);
}

static void _user_io_config_x_down(void *arg)
{
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;
    config_model_type_t type = (config_model_type_t)user_io_controller->state.state_arg.user_io_state.config_idx;

    if (global_model_get_min_value(type) >= user_io_controller->state.state_arg.user_io_state.value)
        return;

    user_io_controller->state.state_arg.user_io_state.value -= 1;

    _user_io_config_display(user_io_controller);
}

static void _user_io_config_on_delete(void *arg)
{
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;
    user_io_normal_set(user_io_controller);
}

static void _user_io_config_display(user_io_controller_t *user_io_controller)
{
    // Move to secund line
    display_controller_set_xy(user_io_controller->display_controller, 0, 1);

    char s_line[16];
    sprintf(s_line, "%4.1f", (float)user_io_controller->state.state_arg.user_io_state.value);
    display_controller_set_str(user_io_controller->display_controller, s_line);
}
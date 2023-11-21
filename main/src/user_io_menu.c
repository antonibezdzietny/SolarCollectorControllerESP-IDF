#include "include/user_io_menu.h"
#include "include/user_io_normal.h"
#include "include/user_io_config.h"
#include "include/config_model.h"

static void _user_io_menu_sw_pressed(void *arg);
static void _user_io_menu_x_up(void *arg);
static void _user_io_menu_x_down(void *arg);
static void _user_io_menu_on_delete(void *arg);

static void _user_io_menu_display(user_io_controller_t *user_io_controller);

void user_io_menu_set(user_io_controller_t *user_io_controller)
{
    // Change state
    user_io_controller->state.state_type = STATE_MENU;

    // Set handles
    user_io_controller->state.joystick_handles.sw_pressed = _user_io_menu_sw_pressed;
    user_io_controller->state.joystick_handles.x_up = _user_io_menu_x_up;
    user_io_controller->state.joystick_handles.x_down = _user_io_menu_x_down;
    user_io_controller->state.joystick_handles.on_delete = _user_io_menu_on_delete;
    user_io_controller->state.joystick_handles.arg = (void *)user_io_controller;

    joystick_controller_retrieve_time_t joystick_controller_retrieve_time = {
        .retrieve_frequency = 2,
        .retrieve_time_s = 10,
    };

    joystick_controller_set_retrieve_time(user_io_controller->joystick_controller,
                                          joystick_controller_retrieve_time);

    joystick_controller_set_handles(user_io_controller->joystick_controller,
                                    user_io_controller->state.joystick_handles);

    joystick_controller_run_retrieve(user_io_controller->joystick_controller);

    _user_io_menu_display(user_io_controller);
}

static void _user_io_menu_sw_pressed(void *arg)
{
    printf("IO MENU:  sw pressed\n");
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;

    // Pass argument
    user_io_controller->state.state_arg.user_io_state.config_idx = user_io_controller->state.state_arg.config_idx;
    user_io_config_set(user_io_controller);
}

static void _user_io_menu_x_up(void *arg)
{
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;

    int tmp_conf_idx = ++user_io_controller->state.state_arg.config_idx % MODEL_N_CONFIG;
    user_io_controller->state.state_arg.config_idx = tmp_conf_idx;

    _user_io_menu_display(user_io_controller);
    printf("IO MENU:  x up %d\n", user_io_controller->state.state_arg.config_idx);
}

static void _user_io_menu_x_down(void *arg)
{
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;

    int tmp_conf_idx = user_io_controller->state.state_arg.config_idx;
    tmp_conf_idx = (tmp_conf_idx + MODEL_N_CONFIG - 1) % MODEL_N_CONFIG;
    user_io_controller->state.state_arg.config_idx = tmp_conf_idx;

    _user_io_menu_display(user_io_controller);
    printf("IO MENU:  x down %d\n", user_io_controller->state.state_arg.config_idx);
}

static void _user_io_menu_on_delete(void *arg)
{
    // In normal mode should be not used
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;
    printf("IO MENU:  delete \n");
    user_io_normal_set(user_io_controller);
}

static void _user_io_menu_display(user_io_controller_t *user_io_controller)
{
    display_controller_set_clear(user_io_controller->display_controller);
    display_controller_set_str(user_io_controller->display_controller,
                               config_model_get_name((config_model_type_t)user_io_controller->state.state_arg.config_idx));
}
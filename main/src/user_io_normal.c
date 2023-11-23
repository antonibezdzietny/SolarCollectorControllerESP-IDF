#include "include/user_io_normal.h"
#include "include/user_io_menu.h"
#include "include/global_model.h"

static void _user_io_normal_sw_pressed(void *arg);
static void _user_io_normal_x_up(void *arg);
static void _user_io_normal_x_down(void *arg);
static void _user_io_normal_on_delete(void *arg);

static void _user_io_normal_display(user_io_controller_t *user_io_controller);
void user_io_normal_display_refresh(user_io_controller_t *user_io_controller);

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

    _user_io_normal_display(user_io_controller);
}

static void _user_io_normal_sw_pressed(void *arg)
{
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;

    // Pass argument
    user_io_controller->state.state_arg.config_idx = 0;
    user_io_menu_set(user_io_controller);
}

static void _user_io_normal_x_up(void *arg) { return; }

static void _user_io_normal_x_down(void *arg) { return; }

static void _user_io_normal_on_delete(void *arg)
{
    // In normal mode should be not used
    user_io_controller_t *user_io_controller = (user_io_controller_t *)arg;
    user_io_normal_set(user_io_controller);
}

static void _user_io_normal_display(user_io_controller_t *user_io_controller)
{
    display_controller_set_clear(user_io_controller->display_controller);
    char f_line[16];
    sprintf(f_line, "Kolektor  %5.1f", global_model_get_temperature(TEMP_COLLECTOR));
    display_controller_set_str(user_io_controller->display_controller, f_line);

    display_controller_set_xy(user_io_controller->display_controller, 0, 1);
    char s_line[16];
    sprintf(s_line, "Zasilanie %5.1f", global_model_get_temperature(TEMP_PIPE));
    display_controller_set_str(user_io_controller->display_controller, s_line);
}

void user_io_normal_display_refresh(user_io_controller_t *user_io_controller)
{
    if (global_model_get_error())
    {
        display_controller_set_xy(user_io_controller->display_controller, 9, 0);
        display_controller_set_str(user_io_controller->display_controller, " ERROR");

        display_controller_set_xy(user_io_controller->display_controller, 9, 1);
        display_controller_set_str(user_io_controller->display_controller, " ERROR");

        return;
    }

    display_controller_set_xy(user_io_controller->display_controller, 9, 0);
    char line[10];
    sprintf(line, "%7.1f", global_model_get_temperature(TEMP_COLLECTOR));
    display_controller_set_str(user_io_controller->display_controller, line);

    display_controller_set_xy(user_io_controller->display_controller, 9, 1);
    sprintf(line, "%7.1f", global_model_get_temperature(TEMP_PIPE));
    display_controller_set_str(user_io_controller->display_controller, line);
}
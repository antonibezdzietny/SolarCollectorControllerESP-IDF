#include "include/user_io_controller.h"
#include "include/user_io_normal.h"

void user_io_controller_init(user_io_controller_t **self)
{
    (*self) = (user_io_controller_t *)calloc(1, sizeof(user_io_controller_t));

    joystick_controller_init(&(*self)->joystick_controller);
    display_controller_init(&(*self)->display_controller);

    user_io_normal_set((*self));
}

void user_io_controller_deinit(user_io_controller_t *self)
{
    display_controller_deinit(self->display_controller);
    joystick_controller_deinit(self->joystick_controller);
    free(self);
}

void user_io_controller_display_refresh(user_io_controller_t *self)
{
    if (self->state.state_type == STATE_NORMAL)
        user_io_normal_display_refresh(self);
}

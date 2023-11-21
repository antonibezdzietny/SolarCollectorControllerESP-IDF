#include "include/user_io_controller.h"
#include "include/user_io_normal.h"

void user_io_controller_init(user_io_controller_t **self)
{
    (*self) = (user_io_controller_t *)calloc(1, sizeof(user_io_controller_t));

    joystick_controller_init(&(*self)->joystick_controller);
    display_controller_init(&(*self)->display_controller);

    user_io_normal_set((*self));
}

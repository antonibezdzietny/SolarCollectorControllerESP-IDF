#include <stdio.h>

#include "include/devices_model.h"
#include "include/joystick_controller.h"
#include "include/display_device.h"

void foo(void *arg)
{
    printf("OK \n");
}

void del(void *arg)
{
    printf("Delete \n");
}
void up(void *arg)
{
    printf("UP \n");
}
void down(void *arg)
{
    printf("DOWN \n");
}

void app_main(void)
{
    devices_model_init();
    gpio_install_isr_service(0);

    joystick_controller_t *j_controller = NULL;

    joystick_controller_init(&j_controller);

    joystick_controller_handles_t handles = {
        .sw_pressed = foo,
        .arg = NULL,
        .on_delete = del,
        .x_up = up,
        .x_down = down,
    };

    joystick_controller_retrieve_time_t t = {
        .retrieve_frequency = 3,
        .retrieve_time_s = 10,
    };

    joystick_controller_set_handles(j_controller, handles);
    joystick_controller_set_retrieve_time(j_controller, t);

    joystick_controller_run_retrieve(j_controller);

    display_device_t *disp = NULL;
    display_device_config_t disp_conf = devices_model_get_display();

    display_device_init(&disp, &disp_conf);

    display_device_set_str(disp, "Hello World");
}

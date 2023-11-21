#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "nvs_flash.h"

#include "include/devices_model.h"
#include "include/config_model.h"
#include "include/global_model.h"

#include "include/user_io_controller.h"

static void init_all_default_settings(void);

void app_main(void)
{
    nvs_flash_init();
    devices_model_init();
    gpio_install_isr_service(0);

    init_all_default_settings();

    user_io_controller_t *user_io_controller = NULL;

    user_io_controller_init(&user_io_controller);

    user_io_controller_display_refresh(user_io_controller);
}

static void init_all_default_settings(void)
{
    devices_model_init();
    config_model_init();

    global_model_init();
}
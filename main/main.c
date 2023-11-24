#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "nvs_flash.h"

#include "include/devices_model.h"
#include "include/config_model.h"
#include "include/global_model.h"

#include "include/user_io_controller.h"

#include "ds18x20.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "include/relay_controller.h"

#include "include/thermometers_controller.h"

#include "include/global_controller.h"

#include "include/wifi_controller.h"

static void init_all_default_settings(void)
{
    devices_model_init();
    config_model_init();
    global_model_init();
}

void app_main(void)
{
    nvs_flash_init();
    gpio_install_isr_service(0);

    devices_model_init();
    init_all_default_settings();

    global_controller_t *global_controller = NULL;
    global_controller_init(&global_controller);

    global_controller_run(global_controller);
    global_controller_run_logger(global_controller);
}

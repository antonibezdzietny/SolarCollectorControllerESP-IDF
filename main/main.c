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

static void init_all_default_settings(void)
{
    devices_model_init();
    config_model_init();
    global_model_init();
}

// static const onewire_addr_t SENSOR_ADDRS[] = {0x5e3c01f09521d728, 0x593c01f095cab528};
// volatile float tmps[] = {-128.f, -128.f};
// static const char *TAG = "ds18x20_test";
// static const gpio_num_t SENSOR_GPIO = 33;

void app_main(void)
{
    nvs_flash_init();
    gpio_install_isr_service(0);

    devices_model_init();
    init_all_default_settings();

    user_io_controller_t *user_io_controller = NULL;

    user_io_controller_init(&user_io_controller);

    user_io_controller_display_refresh(user_io_controller);

    relay_controller_t *relay_controller = NULL;
    relay_controller_init(&relay_controller);

    thermometers_controller_t *thermometers_controller;
    thermometers_controller_init(&thermometers_controller);

    thermometers_device_t thermometer_def = devices_model_get_thermometers();

    printf("Addrs: %d \n", thermometers_controller->one_wire_pin);

    int i = 0;
    for (;;)
    {
        thermometers_controller_measurements(thermometers_controller);
        printf("TEMP COLL: %f \n", global_model_get_temperature(TEMP_COLLECTOR));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        user_io_controller_display_refresh(user_io_controller);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

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
    devices_model_init();
    gpio_install_isr_service(0);

    init_all_default_settings();

    user_io_controller_t *user_io_controller = NULL;

    user_io_controller_init(&user_io_controller);

    user_io_controller_display_refresh(user_io_controller);

    relay_controller_t *relay_controller = NULL;
    relay_controller_init(&relay_controller);

    /*
        esp_err_t res;

        for (;;)
        {
            res = ds18x20_measure_and_read_multi(SENSOR_GPIO, SENSOR_ADDRS, 2, tmps);
            if (res != ESP_OK)
                ESP_LOGE(TAG, "Could not read from sensor");

            for (int i = 0; i < 2; ++i)
                printf("tmp %d : %5.1f \n", i, tmps[i]);

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        */
    int i = 0;
    for (;;)
    {
        i = (i + 1) % (PUMP_G3 + 1);
        relay_controller_set_pump_state(relay_controller, i);

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

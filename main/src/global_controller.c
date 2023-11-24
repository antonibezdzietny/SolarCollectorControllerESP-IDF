#include "include/global_controller.h"
#include "include/global_model.h"
#include "include/config_model.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void _global_controller_pump_driver(global_controller_t *self);

void global_controller_init(global_controller_t **self)
{
    (*self) = (global_controller_t *)calloc(1, sizeof(global_controller_t));

    user_io_controller_init(&(*self)->user_io_controller);
    relay_controller_init(&(*self)->relay_controller);
    thermometers_controller_init(&(*self)->thermometers_controller);
    wifi_controller_init(&(*self)->wifi_controller);
}

void global_controller_deinit(global_controller_t *self)
{
    user_io_controller_deinit(self->user_io_controller);
    relay_controller_deinit(self->relay_controller);
    thermometers_controller_deinit(self->thermometers_controller);
    wifi_controller_deinit(self->wifi_controller);
    free(self);
}

static void _global_controller_run_logger_task(void *args)
{
    global_controller_t *self = (global_controller_t *)args;
    vTaskDelay(1000 * 10 / portTICK_PERIOD_MS);

    post_data_t post_data[] = {{"collector_temp", 0.0}, {"pipe_temp", 0.0}, {"pump_state", 0.0}};
    for (;;)
    {
        if (!global_model_get_error())
        {
            post_data[0].field_value = global_model_get_temperature(TEMP_COLLECTOR);
            post_data[1].field_value = global_model_get_temperature(TEMP_PIPE);
            post_data[2].field_value = (float)relay_controller_get_pump_state(self->relay_controller);

            wifi_controller_send_post_data(self->wifi_controller, post_data, 3);
        }

        vTaskDelay(1000 * 60 * 5 / portTICK_PERIOD_MS);
    }
}

static void _global_controller_run_task(void *args)
{
    global_controller_t *self = (global_controller_t *)args;
    for (;;)
    {
        thermometers_controller_measurements(self->thermometers_controller);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        _global_controller_pump_driver(self);
        user_io_controller_display_refresh(self->user_io_controller);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void global_controller_run(global_controller_t *self)
{
    xTaskCreate(_global_controller_run_task, "_global_controller_run_task",
                2048, (void *)self, 5, NULL);
}

void global_controller_run_logger(global_controller_t *self)
{
    xTaskCreate(_global_controller_run_logger_task, "_global_controller_run_logger_task",
                4096, (void *)self, 2, NULL);
}

static void _global_controller_pump_if_off_driver(global_controller_t *self)
{
    // If temperature achieved min temperature
    float collector = global_model_get_temperature(TEMP_COLLECTOR);
    if (collector > global_model_get_config(MODEL_MIN_TEMP))
    {
        // If differential between temperature achieved min delta
        float pipe = global_model_get_temperature(TEMP_PIPE);
        if ((collector - pipe) > global_model_get_config(MODEL_D_GEAR_1))
        {
            relay_controller_set_pump_state(self->relay_controller, PUMP_G1);
        }
    }
}

static void _global_controller_check_overheating(global_controller_t *self)
{
    if (global_model_get_temperature(TEMP_COLLECTOR) < global_model_get_config(MODEL_MAX_TEMP))
    {
        if (relay_controller_get_overheat_state(self->relay_controller) == true)
            relay_controller_set_overheat_state(self->relay_controller, false);
    }
    else
    {
        if (relay_controller_get_overheat_state(self->relay_controller) == false)
            relay_controller_set_overheat_state(self->relay_controller, true);
    }
}

static void _global_controller_pump_if_on_driver(global_controller_t *self)
{
    float hysteresis = 2.f;

    /*Check is minimal temperature + hysteresis bigger than collector temp*/
    float collector = global_model_get_temperature(TEMP_COLLECTOR);
    if (collector + hysteresis < global_model_get_config(MODEL_MIN_TEMP))
    {
        // Temperature in no sufficient to still running pump
        relay_controller_set_pump_state(self->relay_controller, PUMP_OFF);
        return;
    }

    // If pipe is hotter than collector
    float pipe = global_model_get_temperature(TEMP_PIPE);
    if (pipe >= collector)
    {
        // Turn off pump
        relay_controller_set_pump_state(self->relay_controller, PUMP_OFF);
        return;
    }

    /* If temperature is still sufficient to run pump -> check pump gear */
    pump_state_t new_state = relay_controller_get_pump_state(self->relay_controller);

    float differential = collector - pipe;
    if (differential < global_model_get_config(MODEL_D_GEAR_2))
        new_state = PUMP_G1;
    else if (differential < global_model_get_config(MODEL_D_GEAR_3))
        new_state = PUMP_G2;
    else
        new_state = PUMP_G3;

    if (new_state != relay_controller_get_pump_state(self->relay_controller))
    {
        relay_controller_set_pump_state(self->relay_controller, new_state);
    }
}

static void _global_controller_pump_driver(global_controller_t *self)
{
    _global_controller_check_overheating(self);

    if (relay_controller_get_pump_state(self->relay_controller) == PUMP_OFF)
        _global_controller_pump_if_off_driver(self);
    else
        _global_controller_pump_if_on_driver(self);
}
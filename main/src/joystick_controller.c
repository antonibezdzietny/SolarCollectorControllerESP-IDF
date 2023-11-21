#include "include/joystick_controller.h"

#include "esp_log.h"
#include "freertos/portmacro.h"
#include "freertos/task.h"

#include "include/joystick_device.h"
#include "include/devices_model.h"

static void _joystick_controller_joystick_device_init(joystick_controller_t *self);
static void _joystick_controller_sw_incoming_isr(void *arg); // Default arg joystick_controller_t
static void _joystick_controller_sw_queue_init(joystick_controller_t *self);
static void _joystick_controller_sw_queue_task(void *arg); // Default arg joystick_controller_t
static void _joystick_controller_retrieve_task(void *arg);

void joystick_controller_init(joystick_controller_t **self)
{
    (*self) = (joystick_controller_t *)calloc(1, sizeof(joystick_controller_t));

    _joystick_controller_joystick_device_init((*self));

    (*self)->retrieve_handle = NULL;
}

void joystick_controller_deinit(joystick_controller_t *self)
{
    joystick_device_deinit(self->joystick_device);

    free(self);
}

void joystick_controller_set_retrieve_time(
    joystick_controller_t *self,
    joystick_controller_retrieve_time_t retrieve_time)
{
    self->retrieve_time = retrieve_time;
}

void joystick_controller_set_handles(joystick_controller_t *self,
                                     joystick_controller_handles_t handles)
{
    self->handles = handles;
}

void joystick_controller_run_retrieve(
    joystick_controller_t *self)
{
    if (self->retrieve_handle != NULL)
        vTaskDelete(self->retrieve_handle);

    xTaskCreate(_joystick_controller_retrieve_task, "_joystick_controller_retrieve_task",
                2048, (void *)self, 2, &self->retrieve_handle);
}

/* Config joystick by default setting passing in Kconfig*/
static void _joystick_controller_joystick_device_init(joystick_controller_t *self)
{
    joystick_device_config_t j_conf = devices_model_get_joystick();
    joystick_device_init(&self->joystick_device, &j_conf);

    joystick_device_handle_t j_handle = {
        .handle = _joystick_controller_sw_incoming_isr,
        .handle_arg = self,
    };
    joystick_device_set_isr(self->joystick_device, &j_handle);

    _joystick_controller_sw_queue_init(self);
}

static void _joystick_controller_sw_incoming_isr(void *arg)
{
    joystick_controller_t *self = (joystick_controller_t *)arg;
    uint32_t pv_number = (uint32_t)xTaskGetTickCount();
    xQueueSendFromISR(self->_queue_sw_handle, &pv_number, NULL);
}

static void _joystick_controller_sw_queue_init(joystick_controller_t *self)
{
    self->_queue_sw_handle = xQueueCreate(5, sizeof(uint32_t));

    xTaskCreate(_joystick_controller_sw_queue_task, "_joystick_controller_sw_queue_task",
                2048, self, 2, NULL);
}

static void _joystick_controller_sw_queue_task(void *arg)
{
    joystick_controller_t *self = (joystick_controller_t *)arg;

    uint32_t previous_time = 0;
    uint32_t pv_number;
    for (;;)
    {
        if (xQueueReceive(self->_queue_sw_handle, &pv_number, portMAX_DELAY))
        {
            if (self->handles.sw_pressed == NULL)
                ESP_LOGE("JOYSTICK CONTROLLER", "sw isr handle not set");
            else
            {
                uint32_t interval = (pv_number - previous_time) / portTICK_PERIOD_MS;

                if (interval > 5)
                {
                    previous_time = pv_number;
                    self->handles.sw_pressed(self->handles.arg);
                }
            }
        }
    }
}

static void _joystick_controller_retrieve_task(void *arg)
{
    joystick_controller_t *self = (joystick_controller_t *)arg;

    // Setting joystick manipulator
    int const NEUTRAL_VALUE = 300; // Joystick value in neutral position
    int const DEAD_SPACE = 200;    // Margin sensitive

    // Task running time config
    int const N_PERIOD = self->retrieve_time.retrieve_time_s * self->retrieve_time.retrieve_frequency;

    // Use only X position
    int x_value = 0;

    int n_period = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        joystick_device_get_x(self->joystick_device, &x_value);

        if (x_value > NEUTRAL_VALUE + DEAD_SPACE)
        {
            // Positive x manipulator value
            n_period = 0; // Reset counter
            self->handles.x_up(self->handles.arg);
        }
        else if (x_value < NEUTRAL_VALUE - DEAD_SPACE)
        {
            // Negative x manipulator value
            n_period = 0; // Reset counter
            self->handles.x_down(self->handles.arg);
        }

        vTaskDelayUntil(&xLastWakeTime,
                        (1000 / self->retrieve_time.retrieve_frequency) / portTICK_PERIOD_MS);

        /* Counter unavailable */
        if (++n_period >= N_PERIOD)
        {
            self->retrieve_handle = NULL;
            self->handles.on_delete(self->handles.arg);
            vTaskDelete(NULL);
        }
    }
}

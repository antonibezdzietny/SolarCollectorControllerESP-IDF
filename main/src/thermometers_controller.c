#include "include/thermometers_controller.h"
#include "include/devices_model.h"
#include "include/global_model.h"

#include <string.h>

void thermometers_controller_init(thermometers_controller_t **self)
{
    (*self) = (thermometers_controller_t *)calloc(1, sizeof(thermometers_controller_t));

    thermometers_device_t thermometer_def = devices_model_get_thermometers();

    memcpy((*self), &thermometer_def, sizeof(thermometers_controller_t));
}

void thermometers_controller_deinit(thermometers_controller_t *self)
{
    free(self);
}

void thermometers_controller_measurements(thermometers_controller_t *self)
{
    esp_err_t err = ds18x20_measure_and_read_multi(self->one_wire_pin, self->addrs,
                                                   N_THERMOMETERS, global_model_get_temperature_arr());

    if (err != ESP_OK)
        global_model_set_error(true);
    else
        global_model_set_error(false);
}
#include "include/global_model.h"

#include "nvs.h"
#include "nvs_flash.h"

static void _global_model_nvs_read(config_model_type_t type);
static void _global_model_nvs_write(config_model_type_t type, int value);

static global_model_t *_global_model = NULL;

void global_model_init(void)
{
    if (!(_global_model == NULL))
        return;

    _global_model = calloc(1, sizeof(global_model_t));

    for (int i = 0; i < MODEL_N_CONFIG; ++i)
    {
        _global_model->configurations[i] = config_model_get_default((config_model_type_t)i);
        _global_model_nvs_read((config_model_type_t)i);
    }

    _global_model->is_temperature_error = false;
}

void global_model_set_config(config_model_type_t type, int value)
{
    if (_global_model->configurations[type] == value)
        return;

    _global_model_nvs_write(type, value);
    _global_model->configurations[type] = value;
};

int global_model_get_config(config_model_type_t type) { return _global_model->configurations[type]; }

// Be careful using this array
float *global_model_get_temperature_arr() { return &_global_model->temperature; }

float global_model_get_temperature(temp_type_t type) { return _global_model->temperature[type]; }

void global_model_set_error(bool err) { _global_model->is_temperature_error = err; }

bool global_model_get_error(void) { return _global_model->is_temperature_error; }

int global_model_get_max_value(config_model_type_t type)
{
    switch (type)
    {
    case MODEL_MIN_TEMP:
        return 50;
    case MODEL_D_GEAR_1:
        return global_model_get_config(MODEL_D_GEAR_2);
    case MODEL_D_GEAR_2:
        return global_model_get_config(MODEL_D_GEAR_3);
    case MODEL_D_GEAR_3:
        return 30; // Maximum delta
    case MODEL_MAX_TEMP:
        return 95; // Maximal acceptable temperature
    default:
        return -1;
    }
}

int global_model_get_min_value(config_model_type_t type)
{
    switch (type)
    {
    case MODEL_MIN_TEMP:
        return 10;
    case MODEL_D_GEAR_1:
        return 0;
    case MODEL_D_GEAR_2:
        return global_model_get_config(MODEL_D_GEAR_1);
    case MODEL_D_GEAR_3:
        return global_model_get_config(MODEL_D_GEAR_2);
    case MODEL_MAX_TEMP:
        return 50; // Maximal acceptable temperature
    default:
        return -1;
    }
}

static void _global_model_nvs_read(config_model_type_t type)
{
    nvs_handle_t nvs_handle;
    nvs_open("storage", NVS_READONLY, &nvs_handle);
    int16_t temporary_var;
    if (!nvs_get_i16(nvs_handle, config_model_get_name(type), &temporary_var))
        _global_model->configurations[type] = temporary_var;
}

static void _global_model_nvs_write(config_model_type_t type, int value)
{
    nvs_handle_t nvs_handle;
    nvs_open("storage", NVS_READWRITE, &nvs_handle);
    ESP_ERROR_CHECK_WITHOUT_ABORT(nvs_set_i16(nvs_handle, config_model_get_name(type), (int16_t)value));
}

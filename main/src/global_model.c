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

    _global_model->collector_temp = -128.f;
    _global_model->pipe_temp = -128.f;
}

void global_model_set_config(config_model_type_t type, int value)
{
    if (_global_model->configurations[type] == value)
        return;

    _global_model_nvs_write(type, value);
    _global_model->configurations[type] = value;
};
void global_model_set_collector_temp(float value) { _global_model->collector_temp = value; };
void global_model_set_pipe_temp(float value) { _global_model->pipe_temp = value; };

int global_model_get_config(config_model_type_t type) { return _global_model->configurations[type]; }
float global_model_get_collector_temp(void) { return _global_model->collector_temp; }
float global_model_get_pipe_temp(void) { return _global_model->pipe_temp; };

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
#include "include/config_model.h"

#include <string.h>
#include "sdkconfig.h"

static config_model_t *_config = NULL;

void config_model_init(void)
{
    if (!(_config == NULL))
        return;

    _config = calloc(1, sizeof(config_model_t));

    config_model_t config_default = {
        .CONFIG_FIELDS_DEFAULT = {
            CONFIG_MIN_TEMP,
            CONFIG_D_GEAR_1,
            CONFIG_D_GEAR_2,
            CONFIG_D_GEAR_3,
            CONFIG_MAX_TEMP},
        .CONFIG_FIELDS_NAME = {
            "TEMP_PUMP_INIT",
            "D_PUMP_INIT",
            "D_GEAR_2",
            "D_GEAR_3",
            "TEMP_MAX",
        },

    };

    memcpy(_config, &config_default, sizeof(config_model_t));
}

char *config_model_get_name(config_model_type_t config_type)
{
    return _config->CONFIG_FIELDS_NAME[config_type];
}

int config_model_get_default(config_model_type_t config_type)
{
    return _config->CONFIG_FIELDS_DEFAULT[config_type];
}

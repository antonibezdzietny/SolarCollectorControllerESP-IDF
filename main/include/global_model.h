#ifndef GLOBAL_MODEL_H
#define GLOBAL_MODEL_H

/*
 *
 */

#include "include/config_model.h"
#include <stdbool.h>

typedef enum
{
    TEMP_COLLECTOR = 0,
    TEMP_PIPE,
    TEMP_N_ELEMENTS
} temp_type_t;

typedef struct
{
    volatile int configurations[MODEL_N_CONFIG];
    volatile float temperature[TEMP_N_ELEMENTS];
    volatile bool is_temperature_error;
} global_model_t;

void global_model_init(void);

void global_model_set_config(config_model_type_t type, int value);
int global_model_get_config(config_model_type_t type);

int global_model_get_max_value(config_model_type_t type);
int global_model_get_min_value(config_model_type_t type);

float *global_model_get_temperature_arr(void);
float global_model_get_temperature(temp_type_t type);

void global_model_set_error(bool err);
bool global_model_get_error(void);

#endif
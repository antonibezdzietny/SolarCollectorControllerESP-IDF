#ifndef GLOBAL_MODEL_H
#define GLOBAL_MODEL_H

/*
 *
 */

#include "include/config_model.h"

typedef struct
{
    volatile int configurations[MODEL_N_CONFIG];
    volatile float collector_temp;
    volatile float pipe_temp;
} global_model_t;

void global_model_init(void);

void global_model_set_config(config_model_type_t type, int value);
void global_model_set_collector_temp(float value);
void global_model_set_pipe_temp(float value);

int global_model_get_config(config_model_type_t type);
float global_model_get_collector_temp(void);
float global_model_get_pipe_temp(void);

#endif
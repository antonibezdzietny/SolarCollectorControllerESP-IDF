#ifndef GLOBAL_MODEL_H
#define GLOBAL_MODEL_H

/*
 *
 */

#include "include/config_model.h"

typedef struct {
  volatile int configurations[MODEL_N_CONFIG];
} global_model_t;

void global_model_init(void);

void global_model_set_config(config_model_type_t type, int value);

int global_model_get_config(config_model_type_t type);

int global_model_get_max_value(config_model_type_t type);
int global_model_get_min_value(config_model_type_t type);

#endif
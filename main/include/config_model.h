#ifndef CONFIG_MODEL_H
#define CONFIG_MODEL_H

typedef enum
{
  MODEL_MIN_TEMP = 0,
  MODEL_D_GEAR_1,
  MODEL_D_GEAR_2,
  MODEL_D_GEAR_3,
  MODEL_MAX_TEMP,
  MODEL_N_CONFIG,
} config_model_type_t;

typedef struct
{
  const int CONFIG_FIELDS_DEFAULT[MODEL_N_CONFIG];
  const char *CONFIG_FIELDS_NAME[MODEL_N_CONFIG];
} config_model_t;

void config_model_init(void);
char *config_model_get_name(config_model_type_t config_type);
int config_model_get_default(config_model_type_t config_type);

#endif
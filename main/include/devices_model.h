#ifndef DEVICES_MODEL_H
#define DEVICES_MODEL_H

#include "include/joystick_device.h"

void devices_model_init(void);

joystick_device_config_t devices_model_get_joystick(void);

#endif
#ifndef DEVICES_MODEL_H
#define DEVICES_MODEL_H

#include "include/display_device.h"
#include "include/joystick_device.h"
#include "include/thermometers_device.h"

void devices_model_init(void);

display_device_config_t devices_model_get_display(void);
joystick_device_config_t devices_model_get_joystick(void);
thermometers_device_t devices_model_get_thermometers(void);

#endif
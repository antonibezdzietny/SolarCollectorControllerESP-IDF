#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include "include/display_device.h"

typedef struct
{
    display_device_t *display_device;
} display_controller_t;

/* Public display_controller interface */
void display_controller_init(display_controller_t **self);
void display_controller_deinit(display_controller_t *self);

void display_controller_set_clear(const display_controller_t *self);
void display_controller_set_cursor(const display_controller_t *self, bool visible,
                                   bool blinking);
void display_controller_set_xy(const display_controller_t *self, uint8_t col,
                               uint8_t line);
void display_controller_set_str(const display_controller_t *self, const char *s);
void display_controller_set_char(const display_controller_t *self, char c);
#endif
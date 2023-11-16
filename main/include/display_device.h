#ifndef DISPLAY_DEVICE_H
#define DISPLAY_DEVICE_H

#include "hd44780.h"

typedef struct
{
    uint8_t rs;
    uint8_t en;
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
    uint8_t d7;
} display_device_config_t;

typedef struct
{
    hd44780_t hd44780;
} display_device_t;

/* Public display_device interface */
void display_device_init(display_device_t **self,
                         display_device_config_t *display_device_config);
void display_device_deinit(display_device_t *self);

void display_device_set_clear(const display_device_t *self);
void display_device_set_cursor(const display_device_t *self, bool visible,
                               bool blinking);
void display_device_set_xy(const display_device_t *self, uint8_t col,
                           uint8_t line);
void display_device_set_str(const display_device_t *self, const char *s);
void display_device_set_char(const display_device_t *self, char c);
#endif
#include "include/display_controller.h"

#include "include/devices_model.h"

static void _display_controller_display_device_init(display_controller_t *self);

void display_controller_init(display_controller_t **self)
{
    (*self) = (display_controller_t *)calloc(1, sizeof(display_controller_t));
}

void display_controller_deinit(display_controller_t *self)
{
    display_device_deinit(self->display_device);

    free(self);
}

void display_controller_set_clear(const display_controller_t *self)
{
    display_device_set_clear(self->display_device);
}

void display_controller_set_cursor(const display_controller_t *self, bool visible,
                                   bool blinking)
{
    display_device_set_cursor(self->display_device, visible, blinking);
}

void display_controller_set_xy(const display_controller_t *self, uint8_t col,
                               uint8_t line)
{
    display_device_set_xy(self->display_device, col, line);
}

void display_controller_set_str(const display_controller_t *self, const char *s)
{
    display_device_set_str(self->display_device, s);
}

void display_controller_set_char(const display_controller_t *self, char c)
{
    display_device_set_char(self->display_device, c);
}

static void _display_controller_display_device_init(display_controller_t *self)
{
    display_device_config_t display_default_config = devices_model_get_display();

    display_device_init(&self->display_device, &display_default_config);
}
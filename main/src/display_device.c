#include "include/display_device.h"

void display_device_init(display_device_t **self,
                         display_device_config_t *display_device_pins)
{
    (*self) = (display_device_t *)calloc(1, sizeof(display_device_t));

    /* Not configurable fields from outside */
    (*self)->hd44780.write_cb = NULL;
    (*self)->hd44780.pins.bl = HD44780_NOT_USED;
    (*self)->hd44780.font = HD44780_FONT_5X8;
    (*self)->hd44780.lines = 2;
    (*self)->hd44780.backlight = false;

    /* Configurable data */
    (*self)->hd44780.pins.rs = display_device_pins->rs;
    (*self)->hd44780.pins.e = display_device_pins->en;
    (*self)->hd44780.pins.d4 = display_device_pins->d4;
    (*self)->hd44780.pins.d5 = display_device_pins->d5;
    (*self)->hd44780.pins.d6 = display_device_pins->d6;
    (*self)->hd44780.pins.d7 = display_device_pins->d7;

    /* Init device */
    hd44780_init(&(*self)->hd44780);
    display_device_set_clear((*self));
}

void display_device_deinit(display_device_t *self)
{
    free(self);
}

void display_device_set_clear(const display_device_t *self)
{
    hd44780_clear(&self->hd44780);
}

void display_device_set_cursor(const display_device_t *self, bool visible,
                               bool blinking)
{
    hd44780_control(&self->hd44780, true, visible, blinking);
}

void display_device_set_xy(const display_device_t *self, uint8_t col,
                           uint8_t line)
{
    hd44780_gotoxy(&self->hd44780, col, line);
}

void display_device_set_str(const display_device_t *self, const char *s)
{
    hd44780_puts(&self->hd44780, s);
}

void display_device_set_char(const display_device_t *self, char c)
{
    hd44780_putc(&self->hd44780, c);
}

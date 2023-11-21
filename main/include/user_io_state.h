#ifndef USER_IO_STATE_H
#define USER_IO_STATE_H

/* Header only */
#include "include/joystick_controller.h"

typedef void (*display_handle_t)(void *arg);

typedef enum { STATE_NORMAL = 0, STATE_MENU, STATE_CONFIG } state_type_t;

typedef struct {
  state_type_t state_type;
  union {
    int config_idx;
    struct {
      int config_idx;
      float value;
    } user_io_state;
  } state_arg;

  joystick_controller_handles_t joystick_handles;
} state_t;

#endif
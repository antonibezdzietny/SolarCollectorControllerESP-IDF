#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include "esp_http_client.h"
#include "esp_wifi.h"

typedef enum
{
    WIFI_ERROR = 0,
    WIFI_OK,
    WIFI_JOINING,
} wifi_status_t;

typedef struct
{
    wifi_config_t wifi_config;
    wifi_status_t wifi_status;
    esp_http_client_config_t http_client_config;
    char *api_key_value;
} wifi_controller_t;

typedef struct
{
    char field_name[20];
    float field_value;
} post_data_t;

void wifi_controller_init(wifi_controller_t **self);
void wifi_controller_deinit(wifi_controller_t *self);

void wifi_controller_send_post_data(wifi_controller_t *self, post_data_t *post_data, int n_data);

#endif
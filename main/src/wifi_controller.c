#include "include/wifi_controller.h"
#include "sdkconfig.h"
#include <string.h>

#include "esp_log.h"

static void _wifi_controller_sta_init(wifi_controller_t *self);
static void _wifi_controller_sta_event_handle(void *event_handler_arg,
                                              esp_event_base_t event_base,
                                              int32_t event_id,
                                              void *event_data);

static esp_err_t _wifi_controller_client_event_handle(esp_http_client_event_handle_t args);

void wifi_controller_init(wifi_controller_t **self)
{
    (*self) = (wifi_controller_t *)calloc(1, sizeof(wifi_controller_t));

    wifi_config_t wifi_c = {
        .sta = {
            .ssid = CONFIG_WIFI_SSID,
            .password = CONFIG_WIFI_PASSWORD}};

    (*self)->wifi_config = wifi_c;

    esp_http_client_config_t client_c = {
        .url = CONFIG_POST_URL,
        .method = HTTP_METHOD_POST,
        .cert_pem = NULL,
        .event_handler = _wifi_controller_client_event_handle};

    (*self)->http_client_config = client_c;

    (*self)->api_key_value = CONFIG_API_KEY;

    (*self)->wifi_status = WIFI_ERROR;

    _wifi_controller_sta_init((*self));
}

void wifi_controller_deinit(wifi_controller_t *self)
{
    free((char *)self->http_client_config.url);

    free(self->api_key_value);
    free(self);
}

void _wifi_controller_reconnection(wifi_controller_t *self)
{
}

void _wifi_controller_prepare_post_data(wifi_controller_t *self, post_data_t *post_data, int n_data, char *post_str)
{
    // Add api_key
    sprintf(post_str, "%s=%s", "api_key", self->api_key_value);

    char buffer[40];
    for (uint8_t i = 0; i < n_data; ++i)
    {
        sprintf(buffer, "&%s=%.1f", post_data[i].field_name, post_data[i].field_value);
        strncat(post_str, buffer, 40);
    }
}

void wifi_controller_send_post_data(wifi_controller_t *self, post_data_t *post_data, int n_data)
{
    if (self->wifi_status != WIFI_OK)
    {
        esp_wifi_connect();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }

    char *post_str = calloc(1, sizeof(char) * (50 + 30 * n_data));
    _wifi_controller_prepare_post_data(self, post_data, n_data, post_str);

    esp_http_client_handle_t client = esp_http_client_init(&self->http_client_config);
    esp_http_client_set_post_field(client, post_str, strlen(post_str));
    esp_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");
    esp_http_client_perform(client);
    esp_http_client_cleanup(client);

    printf("TIMEOUT %d \n", self->http_client_config.timeout_ms);

    free(post_str);
}

static void _wifi_controller_sta_init(wifi_controller_t *self)
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, _wifi_controller_sta_event_handle, self);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, _wifi_controller_sta_event_handle, self);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &self->wifi_config);
    esp_wifi_start();
    esp_wifi_connect();
}

static esp_err_t _wifi_controller_client_event_handle(esp_http_client_event_handle_t args)
{
    switch (args->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGI("HTTP", "Post data error");
        return ESP_OK;
        break;

    case HTTP_EVENT_ON_DATA:
        ESP_LOGI("HTTP", "on data: %.*s\n", args->data_len, (char *)args->data);
        break;

    default:
        break;
    }
    return ESP_FAIL;
}

static void _wifi_controller_sta_event_handle(void *event_handler_arg, esp_event_base_t event_base,
                                              int32_t event_id, void *event_data)
{
    wifi_controller_t *wifi_controller = (wifi_controller_t *)event_handler_arg;

    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        ESP_LOGI("WIFI", "WiFi trying to set connection \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI("WIFI", "WiFi connected \n");
        wifi_controller->wifi_status = WIFI_JOINING;
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI("WIFI", "WiFi lost connection \n");
        wifi_controller->wifi_status = WIFI_ERROR;
        break;
    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI("WIFI", "WiFi got IP \n");
        wifi_controller->wifi_status = WIFI_OK;
        break;
    default:
        break;
    }
}
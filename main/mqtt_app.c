#include "mqtt_app.h"

#include "mqtt_client.h"
#include "esp_log.h"

static const char *TAG = "MQTT";

static esp_mqtt_client_handle_t mqtt_client = NULL;

//------------------------------------------------------------

static void mqtt_event_handler(void *handler_args,
                               esp_event_base_t base,
                               int32_t event_id,
                               void *event_data)
{
    esp_mqtt_event_handle_t event =
        (esp_mqtt_event_handle_t)event_data;

    switch(event->event_id)
    {
        case MQTT_EVENT_CONNECTED:

            ESP_LOGI(TAG,"MQTT Connected!");

            break;

        case MQTT_EVENT_DISCONNECTED:

            ESP_LOGI(TAG,"MQTT Disconnected!");

            break;

        case MQTT_EVENT_PUBLISHED:

            ESP_LOGI(TAG,"Message Published");

            break;

        default:

            break;
    }
}

//------------------------------------------------------------

void mqtt_app_start(void)
{
esp_mqtt_client_config_t mqtt_cfg =
    {
        .broker.address.uri = 
            "mqtt://YOUR_MOSQUITTO_BROKER_IP:1883"
    };

    mqtt_client =
        esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(
        mqtt_client,
        ESP_EVENT_ANY_ID,
        mqtt_event_handler,
        NULL);

    esp_mqtt_client_start(mqtt_client);
}

//------------------------------------------------------------

void mqtt_publish_message(const char *message)
{
    if(mqtt_client == NULL)
        return;

    esp_mqtt_client_publish(
        mqtt_client,
        "sensor/environment",
        message,
        0,
        1,
        0);
}
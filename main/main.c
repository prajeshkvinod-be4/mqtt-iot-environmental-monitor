#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sensor.h"
#include "wifi.h"
#include "mqtt_app.h"

void app_main(void)
{
    adc_oneshot_unit_handle_t adc_handle;

    sensor_data_t sensor = {0};

    char json[128];

    // Initialize Sensors
    sensor_init(&adc_handle);

    // Connect Wi-Fi
    wifi_init();

    // Connect MQTT Broker
    mqtt_app_start();

    while (1)
    {
        // Read Sensors
        sensor_read(&sensor,
                    adc_handle);

        // Create JSON
        create_json(&sensor,
                    json,
                    sizeof(json));

        // Print to Serial
        printf("%s\n", json);

        // Publish to MQTT
        mqtt_publish_message(json);

        // Optional Debug
        // print_sensor_data(&sensor);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
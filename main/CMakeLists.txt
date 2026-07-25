idf_component_register(
    SRCS
        "main.c"
        "sensor.c"
        "wifi.c"
        "mqtt_app.c"

    INCLUDE_DIRS
        "."

    REQUIRES
        esp_wifi
        esp_adc
        esp_event
        nvs_flash
        mqtt
)
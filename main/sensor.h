#ifndef SENSOR_H
#define SENSOR_H

#include "esp_adc/adc_oneshot.h"

//------------- Sensor Structure ----------------//

typedef struct
{
    int ntc_adc;
    float ntc_voltage;
    float temperature;

    int mq_adc;
    float mq_voltage;

} sensor_data_t;

//------------- Function Declarations ----------------//

void sensor_init(adc_oneshot_unit_handle_t *adc_handle);

void sensor_read(sensor_data_t *sensor,
                 adc_oneshot_unit_handle_t adc_handle);

void print_sensor_data(sensor_data_t *sensor);

void create_json(sensor_data_t *sensor,
                 char *json_buffer,
                 size_t buffer_size);

#endif
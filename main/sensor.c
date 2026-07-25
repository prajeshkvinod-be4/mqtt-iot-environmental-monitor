#include <stdio.h>
#include <math.h>

#include "sensor.h"

#include "esp_adc/adc_oneshot.h"

//------------- ADC Configuration ----------------//

#define NTC_CHANNEL      ADC_CHANNEL_0
#define MQ2_CHANNEL      ADC_CHANNEL_1
#define ADC_UNIT         ADC_UNIT_1

#define ADC_REF_VOLTAGE  3.3f
#define ADC_MAX_VALUE    4095.0f

//------------- NTC Constants ----------------//

#define SERIES_RESISTOR       10000.0f
#define NOMINAL_RESISTANCE    10000.0f
#define NOMINAL_TEMPERATURE   25.0f
#define BETA_VALUE            3950.0f

//------------------------------------------------//

static void read_ntc(sensor_data_t *sensor,
                     adc_oneshot_unit_handle_t adc_handle)
{
    ESP_ERROR_CHECK(
        adc_oneshot_read(adc_handle,
                         NTC_CHANNEL,
                         &sensor->ntc_adc));

    sensor->ntc_voltage =
        (sensor->ntc_adc * ADC_REF_VOLTAGE) /
        ADC_MAX_VALUE;

    if(sensor->ntc_voltage >= ADC_REF_VOLTAGE)
        return;

    float resistance =
        SERIES_RESISTOR *
        sensor->ntc_voltage /
        (ADC_REF_VOLTAGE - sensor->ntc_voltage);

    sensor->temperature =
        resistance / NOMINAL_RESISTANCE;

    sensor->temperature =
        log(sensor->temperature);

    sensor->temperature /= BETA_VALUE;

    sensor->temperature +=
        1.0f /
        (NOMINAL_TEMPERATURE + 273.15f);

    sensor->temperature =
        1.0f /
        sensor->temperature;

    sensor->temperature -= 273.15f;
}

//------------------------------------------------//

static void read_mq2(sensor_data_t *sensor,
                     adc_oneshot_unit_handle_t adc_handle)
{
    ESP_ERROR_CHECK(
        adc_oneshot_read(adc_handle,
                         MQ2_CHANNEL,
                         &sensor->mq_adc));

    sensor->mq_voltage =
        (sensor->mq_adc * ADC_REF_VOLTAGE) /
        ADC_MAX_VALUE;
}

//------------------------------------------------//

void sensor_init(adc_oneshot_unit_handle_t *adc_handle)
{
    adc_oneshot_unit_init_cfg_t init_config =
    {
        .unit_id = ADC_UNIT,
    };

    ESP_ERROR_CHECK(
        adc_oneshot_new_unit(
            &init_config,
            adc_handle));

    adc_oneshot_chan_cfg_t config =
    {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12
    };

    ESP_ERROR_CHECK(
        adc_oneshot_config_channel(
            *adc_handle,
            NTC_CHANNEL,
            &config));

    ESP_ERROR_CHECK(
        adc_oneshot_config_channel(
            *adc_handle,
            MQ2_CHANNEL,
            &config));
}

//------------------------------------------------//

void sensor_read(sensor_data_t *sensor,
                 adc_oneshot_unit_handle_t adc_handle)
{
    read_ntc(sensor, adc_handle);

    read_mq2(sensor, adc_handle);
}

//------------------------------------------------//

void print_sensor_data(sensor_data_t *sensor)
{
    printf("---------------------------------\n");

    printf("Temperature : %.2f C\n",
            sensor->temperature);

    printf("NTC ADC     : %d\n",
            sensor->ntc_adc);

    printf("NTC Voltage : %.2f V\n",
            sensor->ntc_voltage);

    printf("MQ2 ADC     : %d\n",
            sensor->mq_adc);

    printf("MQ2 Voltage : %.2f V\n",
            sensor->mq_voltage);

    printf("---------------------------------\n\n");
}

//------------------------------------------------//

void create_json(sensor_data_t *sensor,
                 char *json_buffer,
                 size_t buffer_size)
{
    snprintf(json_buffer,
             buffer_size,
             "{\"temperature\":%.2f,"
             "\"mq_adc\":%d,"
             "\"mq_voltage\":%.2f}",
             sensor->temperature,
             sensor->mq_adc,
             sensor->mq_voltage);
}
#include <driver/adc.h>
#include "data_frame.h"
#include "macros.h"

TaskHandle_t monitor_pressure_handler;

int8_t sensor_error_correction;

uint8_t run_minutes;
uint8_t run_hrs;

void read_pressure(void * ptr)
{
    uint32_t sum_of_pressure = 0;
    uint16_t adcRawValue;
    float sensorVoltage;
    int16_t P_mmHg;
    uint16_t samplesPsec = 1000 / SAMPLING_TIME;
    uint8_t seconds = 59, battery_percentage;

    struct Tx_data_frame Pressure_data = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
              .vp = ORGANIZE_COMMAND(SEND_PRESSURE),
    };

    struct Tx_data_frame run_time = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
    };

    while(1) {
    adcRawValue = adc1_get_raw(ADC1_CHANNEL_4);
    sensorVoltage = (adcRawValue / 4095.0) * ADC_REFERENCE_VOLTAGE;
    sensorVoltage /= ADC_REFERENCE_VOLTAGE;
    sensorVoltage *= (SENSOR_VS);
    sum_of_pressure += sensorVoltage * sensorVoltage;

    if (!samplesPsec) {

      sensorVoltage = (float)sqrt(sum_of_pressure / (1000 / SAMPLING_TIME));
      P_mmHg = ((sensorVoltage + sensor_error_correction) / SENSOR_VS  - 0.92) / 0.0024;
      P_mmHg *= FILTERING_COFFICIENT;
      Pressure_data.data = ORGANIZE_COMMAND((uint16_t)(-1 * P_mmHg));
      send_data((uint8_t *)&Pressure_data, TX_BUFFER);

      check_leak((uint16_t)(-1 * P_mmHg));

      if(!seconds) {
        run_minutes++;
        if(run_minutes > 59) {
          run_hrs++;
          run_minutes = 0;
          run_time.vp = ORGANIZE_COMMAND(TIMER_HRS);
          run_time.data = ORGANIZE_COMMAND(run_hrs);
          send_data((uint8_t *)&run_time, TX_BUFFER);
        }
        run_time.vp = ORGANIZE_COMMAND(TIMER_MINUTES);
        run_time.data = ORGANIZE_COMMAND(run_minutes);
        send_data((uint8_t *)&run_time, TX_BUFFER);
        seconds = 59;
      }

      samplesPsec = 1000 / SAMPLING_TIME;
      sum_of_pressure = 0;
      seconds--;
      vTaskDelay(pdMS_TO_TICKS(SAMPLING_TIME));
      continue;
    }

    samplesPsec--;
    vTaskDelay(pdMS_TO_TICKS(SAMPLING_TIME));
    }

}

void sensor_init()
{
   uint16_t adcRawValue;
   float sum_of_vout = 0, total_sum_of_vout = 0;
   uint16_t count = 1000;
   uint16_t sensor_offset;

   while(count--) {
      adcRawValue = adc1_get_raw(ADC1_CHANNEL_4);
      sum_of_vout = (adcRawValue / 4095.0) * ADC_REFERENCE_VOLTAGE;
      sum_of_vout /= ADC_REFERENCE_VOLTAGE;
      sum_of_vout *= SENSOR_VS;
      total_sum_of_vout += FILTERING_COFFICIENT * sum_of_vout;
   }

   sensor_offset = (uint16_t)(total_sum_of_vout / 1000.0);
   sensor_error_correction = sensor_offset > SENSOR_OFFSET ? -113 : 112;  
}

void adc_init(void)
{
  adc1_config_width(ADC_WIDTH_BIT_12); // 12-bit resolution
  adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
  xTaskCreate(read_pressure, "monitor_pressure", 1024, NULL, 1, &monitor_pressure_handler);
  vTaskSuspend(monitor_pressure_handler);
}

 
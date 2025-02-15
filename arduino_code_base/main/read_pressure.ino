#include <driver/adc.h>

#define ADC_REFERENCE_VOLTAGE 3300
#define TX_BUFFER 0x08

TaskHandle_t monitor_pressure_handler;

void read_pressure(void * ptr)
{
    uint16_t adcRawValue, sensorVoltage;
    int16_t P_mmHg;

    uint8_t Pressure_data[] = {0x5A, 0xA5, 0x05, 0X82, 0x30, 0X30, 0X00, 0X00};

    while(1) {
    adcRawValue = adc1_get_raw(ADC1_CHANNEL_4);
    sensorVoltage = (adcRawValue / 4095.0) * ADC_REFERENCE_VOLTAGE;

    P_mmHg = (sensorVoltage / ADC_REFERENCE_VOLTAGE - 0.92 ) / 0.002398;

    Pressure_data[7] = (uint8_t)(-1 * P_mmHg);

    send_data(Pressure_data, TX_BUFFER);

    vTaskDelay(pdMS_TO_TICKS(2));
    }

}

void adc_init(void)
{
  adc1_config_width(ADC_WIDTH_BIT_12); // 12-bit resolution
  adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);
  xTaskCreate(read_pressure, "monitor_pressure", 512, NULL, 1, &monitor_pressure_handler);
}

 
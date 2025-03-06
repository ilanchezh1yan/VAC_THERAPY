//#include "main.h"
#include <driver/adc.h>

#include "data_frame.h"
#include "todo_commands.h"
#include "macros.h"


#define LEAKAGE_VALIDATION_PRESSURE 125

extern uint16_t High_pressure;
extern int8_t sensor_error_correction;

void check_leak(uint16_t detected_pressure) 
{
    struct Tx_data_frame leak_alert = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
              .vp = ORGANIZE_COMMAND(LEAKAGE_SEND),
    };
    uint8_t leak_percentage;

    if(detected_pressure > High_pressure) {
        leak_alert.data = ORGANIZE_COMMAND(30);
        send_data((uint8_t *)&leak_alert, TX_BUFFER);
        return;
    }
    leak_percentage = (130 - (detected_pressure * 100) / High_pressure);
    leak_alert.data = ORGANIZE_COMMAND(leak_percentage);
    send_data((uint8_t *)&leak_alert, TX_BUFFER);
}

void validate_leak(void) {
    uint8_t dacValue;

    uint16_t adcRawValue;
    uint16_t samplesPsec = 1000 / SAMPLING_TIME;
    int16_t P_mmHg;

    uint32_t sum_of_pressure = 0;

    float sensorVoltage;
    float vout;

    struct Tx_data_frame seal_okay = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
              .vp = ORGANIZE_COMMAND(SEAL_STATUS),
              .data = ORGANIZE_COMMAND(0x15)
    };
    
    digitalWrite(PROPORTIONAL_VALVE, HIGH);
    High_pressure = LEAKAGE_VALIDATION_PRESSURE;
    vout = SENSOR_REFERENCE * ((-1 * LEAKAGE_VALIDATION_PRESSURE) * 0.002398 + 0.92) + sensor_error_correction;
    vout *= _3300MV_CONVERSION_FACTOR;
    dacValue = (vout / ADC_REFERENCE_VOLTAGE) * (DAC_RESOLUTION); 
    dacWrite(PRESSURE_REFERENCE, dacValue);

    dacWrite(MOTOR_CONTROL_PIN, PUMP_ON);

    for(int i = 0; i < 20;) {
      adcRawValue = adc1_get_raw(ADC1_CHANNEL_4);
      sensorVoltage = (adcRawValue / 4095.0) * ADC_REFERENCE_VOLTAGE;
      sensorVoltage /= ADC_REFERENCE_VOLTAGE;
      sensorVoltage *= (SENSOR_VS);
      sum_of_pressure += sensorVoltage * sensorVoltage;

      if (!samplesPsec) {

        sensorVoltage = (float)sqrt(sum_of_pressure / (1000 / SAMPLING_TIME));
        P_mmHg = ((sensorVoltage + sensor_error_correction) / SENSOR_VS  - 0.92) / 0.0024;
        P_mmHg *= FILTERING_COFFICIENT;

        check_leak((uint16_t)(-1 * P_mmHg));

        i++;
        samplesPsec = 1000 / SAMPLING_TIME;
        sum_of_pressure = 0;
      }
      samplesPsec--;
      vTaskDelay(pdMS_TO_TICKS(SAMPLING_TIME));
    }
    dacWrite(MOTOR_CONTROL_PIN, PUMP_OFF);
    if((uint16_t)(-1 * P_mmHg) >= LEAKAGE_VALIDATION_PRESSURE) {
        send_data((uint8_t *)&seal_okay, TX_BUFFER);
    }
    digitalWrite(PROPORTIONAL_VALVE, LOW);
}
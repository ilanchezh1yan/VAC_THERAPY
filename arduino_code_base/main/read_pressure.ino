#include "data_frame.h"
#include "ADS1115.h"
#include "macros.h"

extern uint16_t High_pressure;
extern volatile bool pressure_phase;

TaskHandle_t monitor_pressure_handler;

void read_pressure(void * ptr)
{
    uint32_t sum_of_vout = 0;
    uint16_t adcRawValue;
    float sensorVoltage;
    float P_mmHg;
    uint16_t samplesPsec = SAMPLING_DURATION / SAMPLING_TIME;
    float previousvolt;

    struct Tx_data_frame Pressure_data = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
              .vp = ORGANIZE_COMMAND(SEND_PRESSURE),
    };

    while(1) {
    adcRawValue = read_ADS1115();
    sensorVoltage = ((float)adcRawValue * 6114.0f * 1.5f) / ADS1115_15_BIT_RESOLUTION; 
    sum_of_vout += sensorVoltage;
    if (!samplesPsec) {

      sensorVoltage = sum_of_vout / (SAMPLING_DURATION / SAMPLING_TIME);
      P_mmHg = (-1) * ((sensorVoltage ) / SENSOR_VS  - 0.92f) / 0.0024f;
      Pressure_data.data = ORGANIZE_COMMAND((uint16_t)(P_mmHg));
      send_data((uint8_t *)&Pressure_data, TX_BUFFER);

      check_leak((uint16_t)(P_mmHg));
      
      if(!pressure_phase) {
        if((uint16_t)(P_mmHg) > High_pressure) {
          dacWrite(MOTOR_CONTROL_PIN, PUMP_CONTROL);
        }
        else {
          dacWrite(MOTOR_CONTROL_PIN, PUMP_ON);
        }
      }

      samplesPsec = SAMPLING_DURATION / SAMPLING_TIME;
      sum_of_vout = 0;
      vTaskDelay(pdMS_TO_TICKS(SAMPLING_TIME));
      continue;
    }

    samplesPsec--;
    vTaskDelay(pdMS_TO_TICKS(SAMPLING_TIME));
    }

}



void raed_sensor_init(void)
{
  xTaskCreate(read_pressure, "monitor_pressure", 2048, NULL, 2, &monitor_pressure_handler);
  vTaskSuspend(monitor_pressure_handler);
}

uint16_t read_ADS1115(void) {
  esp_err_t err;
  uint16_t ADS1115_out = 0;
	struct ads1115_conf_reg reg_discriptor = {
			.reg_addrs = CONFIG_REG,
      .reg_MSB = 0x40,
      .reg_LSB = 0xAC,
	};
  uint8_t data[2];

  size_t  readCount = 0;
  
  err = i2cWrite(I2C_NUM, ADS1115_ADDRS, (uint8_t *)&reg_discriptor, 3 , 10);
  if(err) {
    return 0;
  }
  reg_discriptor.reg_addrs = CONVERSION_REG;
  err = i2cWrite(I2C_NUM, ADS1115_ADDRS, (uint8_t *)&reg_discriptor.reg_addrs, 1 , 10);
  if(err) {
    return 0;
  }
  
  err = i2cRead(I2C_NUM, ADS1115_ADDRS, data, 2, 10, &readCount);
  if(err) {
    return 0;
  }
  return (uint16_t)(data[0] << 8 | data[1]);
}

 
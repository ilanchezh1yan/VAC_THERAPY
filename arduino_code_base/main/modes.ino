#include "data_frame.h"
#include "todo_commands.h"
#include "macros.h"

#define HIGH_PRESSURE_PHASE 0x00
#define LOW_PRESSURE_PHASE  0X01

extern struct data_frame receivedData;

extern uint8_t mode_select_flag;
extern uint8_t instil_flag;
extern int8_t sensor_error_correction;

extern uint16_t High_pressure;
extern uint16_t low_pressure;

extern uint8_t Thigh;
extern uint8_t Tlow;

TaskHandle_t npwt_mode_handler;

volatile bool pressure_phase;

static void continuous_mode(void)
{
    uint8_t dacValue;
    float vout;

    dacWrite(MOTOR_CONTROL_PIN, 255);
    vout = SENSOR_REFERENCE * ((-1 * High_pressure) * 0.002398 + 0.92) + sensor_error_correction;
    vout *= _3300MV_CONVERSION_FACTOR;
    dacValue = (vout / ADC_REFERENCE_VOLTAGE) * (DAC_RESOLUTION); 

    dacWrite(DAC_1, dacValue);
    vTaskSuspend(npwt_mode_handler);
}

void mode_handler(void *ptr) 
{
  uint8_t dacValue;
  uint8_t Time_period;
  float vout; 
  dacWrite(MOTOR_CONTROL_PIN, 255);
  
  while(1) {
    if(mode_select_flag)
    {
      if(pressure_phase) {
          dacWrite(MOTOR_CONTROL_PIN, 255);
          vout = SENSOR_REFERENCE * ((-1 * High_pressure) * 0.0024 + 0.92) + sensor_error_correction;
          vout *= _3300MV_CONVERSION_FACTOR;
          Time_period = Tlow;
      }
      else {
          dacWrite(MOTOR_CONTROL_PIN, 0);
          vout = SENSOR_REFERENCE * ((-1 * low_pressure) * 0.0024 + 0.92) + sensor_error_correction;
          vout *= _3300MV_CONVERSION_FACTOR;
          Time_period = Tlow;
      }
      pressure_phase = !pressure_phase;
      dacValue = (vout / ADC_REFERENCE_VOLTAGE) * DAC_RESOLUTION;
      dacWrite(DAC_1, dacValue);
      vTaskDelay(pdMS_TO_TICKS(Time_period * 1000 * 60));

    }
    else {
        continuous_mode();
    }
  }
}

void mode_task_create(void)
{
    xTaskCreate(mode_handler, "npwt_mode_handler_task", 1024, NULL, 2, &npwt_mode_handler);
    vTaskSuspend(npwt_mode_handler);
}
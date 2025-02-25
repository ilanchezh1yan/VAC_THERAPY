#include "data_frame.h"
#include "todo_commands.h"

#define MOTOR_CONTROL_PIN 26

#define ADC_REFERENCE_VOLTAGE 3280
#define DAC_RESOLUTION 255
#define MINIMUM_SENSOR_OFFSET_REDUCTION 5   
#define SENSOR_REFERENCE 5000   
#define _3300MV_CONVERSION_FACTOR 0.701

#define DAC_1 25

#define RX_BFR_SIZE 9

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

static void continuous_mode(void)
{
    uint8_t dacValue;
    float vout;

    digitalWrite(MOTOR_CONTROL_PIN, HIGH);
    vout = SENSOR_REFERENCE * ((-1 * High_pressure) * 0.002398 + 0.92) + sensor_error_correction;
    vout *= _3300MV_CONVERSION_FACTOR;
    dacValue = (vout / ADC_REFERENCE_VOLTAGE) * (DAC_RESOLUTION); 

    dacWrite(DAC_1, dacValue);
    vTaskSuspend(npwt_mode_handler);
}

void mode_handler(void *ptr) 
{
  bool pressure_phase = 0x01;
  uint8_t dacValue;
  uint8_t Time_period;
  float vout; 
  digitalWrite(MOTOR_CONTROL_PIN, HIGH);
  
  while(1) {
    if(mode_select_flag)
    {
      if(pressure_phase) {
          vout = SENSOR_REFERENCE * ((-1 * High_pressure) * 0.0024 + 0.92) + sensor_error_correction;
          vout *= _3300MV_CONVERSION_FACTOR;
          Time_period = Tlow;
      }
      else {
          vout = SENSOR_REFERENCE * ((-1 * low_pressure) * 0.0024 + 0.92) + sensor_error_correction;
          vout *= _3300MV_CONVERSION_FACTOR;
          Time_period = Tlow;
      }
      dacValue = (vout / ADC_REFERENCE_VOLTAGE) * DAC_RESOLUTION;
      dacWrite(DAC_1, dacValue);
      vTaskDelay(pdMS_TO_TICKS(Time_period * 1000 * 60));
      pressure_phase = !pressure_phase;

    }
    else {
        continuous_mode();
    }
  }
}

void mode_task_create(void)
{
    xTaskCreate(mode_handler, "npwt_mode_handler_task", 1024, NULL, 1, &npwt_mode_handler);
}
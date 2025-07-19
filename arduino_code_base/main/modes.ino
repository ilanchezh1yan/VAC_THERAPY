#include "data_frame.h"
#include "todo_commands.h"
#include "macros.h"

#define HIGH_PRESSURE_PHASE 0x00
#define LOW_PRESSURE_PHASE  0X01

extern struct data_frame receivedData;

extern uint8_t mode_select_flag;
extern uint8_t instil_flag;

extern uint16_t High_pressure;
extern uint16_t low_pressure;

extern uint8_t Thigh;
extern uint8_t Tlow;

TaskHandle_t npwt_mode_handler;

uint8_t dacValue;

volatile bool pressure_phase;

static void continuous_mode(void)
{
    float vout;

    dacWrite(MOTOR_CONTROL_PIN, PUMP_ON);
    vout = SENSOR_REFERENCE * ((-1 * High_pressure) * 0.002398 + 0.92);
    
    vout *= _3300MV_CONVERSION_FACTOR;
    Serial.println(vout);
    dacValue = (vout / ADC_REFERENCE_VOLTAGE) * (DAC_RESOLUTION);
    pressure_phase = 0x01;

    dacWrite(PRESSURE_REFERENCE, dacValue);
    vTaskSuspend(npwt_mode_handler);
}

void mode_handler(void *ptr) 
{
  uint8_t Time_period;
  float vout; 
  
  while(1) {
    if(mode_select_flag)
    {
      if(pressure_phase) {
          digitalWrite(PROPORTIONAL_VALVE, HIGH);
          dacWrite(MOTOR_CONTROL_PIN, PUMP_ON);
          vout = SENSOR_REFERENCE * ((-1 * High_pressure) * 0.0024 + 0.92);
          vout *= _3300MV_CONVERSION_FACTOR;
          Time_period = Tlow;
      }
      else {
          digitalWrite(PROPORTIONAL_VALVE, LOW);
          dacWrite(MOTOR_CONTROL_PIN, PUMP_OFF);
          vout = SENSOR_REFERENCE * ((-1 * low_pressure) * 0.0024 + 0.92);
          vout *= _3300MV_CONVERSION_FACTOR;
          Time_period = Tlow;
      }
      dacValue = (vout / ADC_REFERENCE_VOLTAGE) * DAC_RESOLUTION;
      dacWrite(PRESSURE_REFERENCE, dacValue);
      pressure_phase = !pressure_phase;
      vTaskDelay(pdMS_TO_TICKS(Time_period * 1000 * 60));

    }
    else {
        continuous_mode();
    }
  }
}

void mode_task_create(void)
{
    xTaskCreate(mode_handler, "npwt_mode_handler_task", 1024, NULL, 3, &npwt_mode_handler);
    vTaskSuspend(npwt_mode_handler);
}
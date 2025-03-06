#include "macros.h"

void GPIO_init(void) {
  pinMode(PROPORTIONAL_VALVE, OUTPUT);
}

void setup(void) {
  GPIO_init();
  uart_init();
  spi_init();
  adc_init();
  Response_task_init();
  mode_task_create();
  fuel_gauge_init();
  if(readFuelGauge(FULLCAPREP_REG) == DEFAULT_FULLCAPREP) {
    writeFuelGauge(FULLCAPREP_REG, BATTERY_CAPACITY);
  }
  sensor_init();
  dacWrite(MOTOR_CONTROL_PIN, PUMP_OFF);
  Serial.begin(115200);
}


void loop(void) {
      return;
}

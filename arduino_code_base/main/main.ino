
#include "macros.h"

void GPIO_init(void) {
  pinMode(PROPORTIONAL_VALVE, OUTPUT);
  pinMode(_5V_ENABLE, OUTPUT_OPEN_DRAIN);
  digitalWrite(_5V_ENABLE, HIGH);
}

void setup() {
  Serial.begin(115200);
  GPIO_init();
  uart_init();
  spi_init();
  raed_sensor_init();
  Response_task_init();
  mode_task_create();
  if(readFuelGauge(FULLCAPREP_REG) == DEFAULT_FULLCAPREP) {
    writeFuelGauge(FULLCAPREP_REG, BATTERY_CAPACITY);
  }
  dacWrite(MOTOR_CONTROL_PIN, PUMP_OFF);
  fuel_gauge_init();
  runtime_calculation(); 
}


void loop(void) {
      return;
}

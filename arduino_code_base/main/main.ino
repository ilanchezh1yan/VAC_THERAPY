#define MOTOR_CONTROL_PIN 26

#define FULLCAPREP 0X10

void setup(void) 
{
  uart_init();
  spi_init();
  adc_init();
  Response_task_init();
  mode_task_create();
  fuel_gauge_init();
  if(readFuelGauge(FULLCAPREP) == 3000) {
    writeFuelGauge(FULLCAPREP, 15000);
  }
  dacWrite(MOTOR_CONTROL_PIN, 0);
  sensor_init();
  Serial.begin(115200);
}


void loop(void) {
      return;
}

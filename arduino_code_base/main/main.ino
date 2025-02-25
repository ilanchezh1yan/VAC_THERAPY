#define TX_BUFFER 0x08
#define MOTOR_CONTROL_PIN 26

extern TaskHandle_t monitor_pressure_handler;
extern TaskHandle_t Response_handler;
extern TaskHandle_t npwt_mode_handler;

void setup(void) 
{
  Serial.begin(115200);
  pinMode(MOTOR_CONTROL_PIN, OUTPUT);
  digitalWrite(MOTOR_CONTROL_PIN, LOW);
  uart_init();
  spi_init();
  adc_init();
  sensor_init();
  Response_task_init();
  mode_task_create();
  vTaskSuspend(Response_handler);
  vTaskSuspend(monitor_pressure_handler);
  vTaskSuspend(npwt_mode_handler);
}

void loop(void) {
      return;
}
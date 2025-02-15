#include "data_frame.h"

#define CHECK_CANISTER 0x1015
#define CHANGE_CANISTER 0X1016
#define LEAKAGE_CHECK 0X1030

#define RX_BFR_SIZE 9

extern uint8_t receivedData[RX_BFR_SIZE];
TaskHandle_t Response_handler;
extern TaskHandle_t monitor_pressure_handler;

void cmd_Response(void *ptr)
{
  uint16_t user_cmd;
  while(1) {
  user_cmd = receivedData[4] << 8 | receivedData[5];
  receivedData[4] = 0;
  receivedData[5] = 0;
  switch(user_cmd) {
    case CHECK_CANISTER:
        validate_canister();
        break;
    case CHANGE_CANISTER:
        change_canister();
        break;
    default:
        break;
  }
    vTaskSuspend(Response_handler);
  }
}

void Response_task_init(void)
{
   xTaskCreate(cmd_Response, "Response_to_user_cmd", 2048, NULL, 1, &Response_handler);
}
void setup(void) 
{
  uart_init();
  spi_init();
  Response_task_init();
  vTaskSuspend(Response_handler);
  vTaskSuspend(monitor_pressure_handler);
}

void loop(void) {

}
#include "data_frame.h"
#include "todo_commands.h"

#define RX_BFR_SIZE 9

uint8_t mode_select_flag;
uint8_t instil_flag;
TaskHandle_t Response_handler;

uint16_t user_cmd;

extern struct data_frame receivedData;
extern TaskHandle_t monitor_pressure_handler;
extern TaskHandle_t npwt_mode_handler;

void cmd_Response(void *ptr)
{
  while(1) {
  user_cmd = ORGANIZE_COMMAND(receivedData.vp);

  receivedData.vp =  0;
  //Serial.println(user_cmd, HEX);
  switch(user_cmd) {
    case CHECK_CANISTER:
        validate_canister();
        break;

    case CHANGE_CANISTER:
        change_canister();
        break;

    case SET_CONST_PRESSURE:
   // case SET_HIGH_PRESSURE:
   // case SET_LOW_PRESSURE:
    case SET_ON_TIME:
    case SET_OFF_TIME:
        
        read_set_parameter();
        /*
        if(mode_init && user_cmd == SET_CONST_PRESSURE && (eTaskGetState(npwt_mode_handler) == eSuspended)) {
          vTaskResume(npwt_mode_handler);
        }
        */
        break;

    case RESTORE_DEFAULT:
        restore_default((uint16_t)receivedData.msb.data << 8 | receivedData.data);
        break;

    case NPWT_MODES:
        mode_select_flag = receivedData.data;
        instil_flag = receivedData.msb.data_flag; 
        vTaskResume(npwt_mode_handler);
        vTaskResume(monitor_pressure_handler);
        break;

    case MODE_INTERRUPTION:
        switch (receivedData.data) {
          case PAUSE:
            mode_pause();
            break;

          case RESUME:
            mode_resume();
            break;
          
          case STOP:
            mode_stop();
            break;

          default:
            break;
        }
        break;

    default:
        break;
  }
      vTaskSuspend(Response_handler);
  }
}

void Response_task_init(void)
{
   xTaskCreate(cmd_Response, "Response_to_user_cmd", 2048, NULL, 2, &Response_handler);
}
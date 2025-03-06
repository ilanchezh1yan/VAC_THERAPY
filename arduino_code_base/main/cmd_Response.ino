#include "data_frame.h"
#include "todo_commands.h"

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

  switch(user_cmd) {
    case CHECK_CANISTER:
        validate_canister();
        break;

    case CHANGE_CANISTER:
        change_canister();
        break;

    case LEAKAGE_SEND:

        validate_leak();
        break;

    case SET_CONST_PRESSURE:
    case SET_ON_TIME:
    case SET_OFF_TIME:
        
        read_set_parameter();
        break;

    case RESTORE_DEFAULT:
        restore_default((uint16_t)receivedData.msb.data << 8 | receivedData.data, (bool)0);
        break;

    case SET_INITIAL_VALUES:
        restore_default((uint16_t)receivedData.msb.data << 8 | receivedData.data, (bool)1);
        break;

    case NPWT_MODES:
        mode_select_flag = receivedData.data;
        instil_flag = receivedData.msb.data_flag; 
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

      case BACK_BUTTON:
      case HOME_BUTTON:
        reset_indicators();
        break;

    default:
        break;
  }
      vTaskSuspend(Response_handler);
  }
}

void Response_task_init(void)
{
   xTaskCreate(cmd_Response, "Response_to_user_cmd", 2048, NULL, 4, &Response_handler);
   vTaskSuspend(Response_handler);
}
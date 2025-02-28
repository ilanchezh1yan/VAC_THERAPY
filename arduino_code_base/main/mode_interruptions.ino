#include "data_frame.h"
#include "macros.h"

extern TaskHandle_t npwt_mode_handler;

extern uint8_t run_minutes;
extern uint8_t run_hrs;

void mode_pause(void) {

    dacWrite(MOTOR_CONTROL_PIN, 0);
    vTaskSuspend(npwt_mode_handler);
    vTaskSuspend(monitor_pressure_handler);
}

void mode_resume(void) {
  if(eTaskGetState(npwt_mode_handler) == eBlocked) {
      xTaskAbortDelay(npwt_mode_handler);
  }
  else {
    vTaskResume(npwt_mode_handler);
  }
  vTaskResume(monitor_pressure_handler);
}
void mode_stop(void) {
    struct Tx_data_frame run_time = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
    };

    dacWrite(MOTOR_CONTROL_PIN, 0);
    vTaskSuspend(npwt_mode_handler);
    vTaskSuspend(monitor_pressure_handler);

    run_minutes = 0;
    run_hrs = 0;

    run_time.vp = ORGANIZE_COMMAND(TIMER_HRS);
    run_time.data = ORGANIZE_COMMAND(0x0000);
    send_data((uint8_t *)&run_time, TX_BUFFER);

    run_time.vp = ORGANIZE_COMMAND(TIMER_MINUTES);
    send_data((uint8_t *)&run_time, TX_BUFFER);
}

void reset_indicators(void) {
    struct Tx_data_frame restore_indicators = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
    };

    restore_indicators.vp = ORGANIZE_COMMAND(SEND_PRESSURE);
    restore_indicators.data = 0;
    send_data((uint8_t *)&restore_indicators, TX_BUFFER);
    
    restore_indicators.vp = ORGANIZE_COMMAND(LEAKAGE_SEND);
    restore_indicators.data = ORGANIZE_COMMAND(1);
    send_data((uint8_t *)&restore_indicators, TX_BUFFER);

}

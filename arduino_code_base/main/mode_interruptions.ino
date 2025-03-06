//#include "main.h"
#include "data_frame.h"
#include "macros.h"

extern TaskHandle_t npwt_mode_handler;
extern volatile bool pressure_phase;

extern uint8_t run_minutes;
extern uint8_t run_hrs;

void mode_pause(void) {

    dacWrite(MOTOR_CONTROL_PIN, PUMP_OFF);
    vTaskSuspend(npwt_mode_handler);
    vTaskSuspend(monitor_pressure_handler);
    pressure_phase = 0x00;
}

void mode_resume(void) {

  digitalWrite(PROPORTIONAL_VALVE, HIGH);
  if(eTaskGetState(npwt_mode_handler) == eBlocked) {
      xTaskAbortDelay(npwt_mode_handler);
  }
  else {
    vTaskResume(npwt_mode_handler);
  }
  vTaskResume(monitor_pressure_handler);
  pressure_phase = 0x01;
}

void mode_stop(void) {
    struct Tx_data_frame run_time = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
    };

    digitalWrite(PROPORTIONAL_VALVE, LOW);
    dacWrite(MOTOR_CONTROL_PIN, PUMP_OFF);
    vTaskSuspend(npwt_mode_handler);
    vTaskSuspend(monitor_pressure_handler);

    run_minutes = 0;
    run_hrs = 0;

    run_time.vp = ORGANIZE_COMMAND(TIMER_HRS);
    run_time.data = ORGANIZE_COMMAND(0x0000);
    send_data((uint8_t *)&run_time, TX_BUFFER);

    run_time.vp = ORGANIZE_COMMAND(TIMER_MINUTES);
    send_data((uint8_t *)&run_time, TX_BUFFER);
    pressure_phase = 0x00;
}

void reset_indicators(void) {

  mode_stop();

  struct Tx_data_frame restore_indicators = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
  };

    restore_indicators.vp = ORGANIZE_COMMAND(SEND_PRESSURE);
    restore_indicators.data = 0;
    send_data((uint8_t *)&restore_indicators, TX_BUFFER);
    
    restore_indicators.vp = ORGANIZE_COMMAND(LEAKAGE_SEND);
    restore_indicators.data = ORGANIZE_COMMAND(130);
    send_data((uint8_t *)&restore_indicators, TX_BUFFER);

}

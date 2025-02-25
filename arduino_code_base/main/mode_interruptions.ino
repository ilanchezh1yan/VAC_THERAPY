#include "data_frame.h"

#define MOTOR_CONTROL_PIN 26

extern TaskHandle_t npwt_mode_handler;

extern uint8_t run_minutes;
extern uint8_t run_hrs;

void mode_pause(void) {

    digitalWrite(MOTOR_CONTROL_PIN, LOW);
    vTaskSuspend(npwt_mode_handler);
    vTaskSuspend(monitor_pressure_handler);
}

void mode_resume(void) {
    vTaskResume(npwt_mode_handler);
    vTaskResume(monitor_pressure_handler);

}
 
void mode_stop(void) {
    struct Tx_data_frame run_time = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
    };

    digitalWrite(MOTOR_CONTROL_PIN, LOW);
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

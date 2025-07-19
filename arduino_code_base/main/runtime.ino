#include "data_frame.h"
#include "macros.h"

#define MINUTE 60000

TaskHandle_t runtimeCalculation_handler;

uint8_t run_minutes;
uint8_t run_hrs;

void runtime_task (void * ptr) {
  struct Tx_data_frame run_time = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
    };
    run_minutes = 0;
  while(1) {
        if(run_minutes > 59) {
          run_hrs++;
          run_minutes = 0;
          run_time.vp = ORGANIZE_COMMAND(TIMER_HRS);
          run_time.data = ORGANIZE_COMMAND(run_hrs);
          send_data((uint8_t *)&run_time, TX_BUFFER);
        }
        run_time.vp = ORGANIZE_COMMAND(TIMER_MINUTES);
        run_time.data = ORGANIZE_COMMAND(run_minutes);
        send_data((uint8_t *)&run_time, TX_BUFFER);
        vTaskDelay(pdMS_TO_TICKS(MINUTE));
        run_minutes++;
  }
}

void runtime_calculation(void) {
  xTaskCreate(runtime_task, "Therapy Duration calculation", 1024, NULL, 1, &runtimeCalculation_handler);
  vTaskSuspend(runtimeCalculation_handler);
}


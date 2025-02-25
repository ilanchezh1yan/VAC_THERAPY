#include "data_frame.h"
#include "todo_commands.h"

extern uint16_t High_pressure;
static uint8_t leak_tolarance_duration;

void check_leak(uint16_t detected_pressure) 
{
    struct Tx_data_frame leak_alert = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
              .vp = ORGANIZE_COMMAND(LEAKAGE_SEND),
    };
    uint8_t leak_percentage;

    if(detected_pressure > High_pressure) {
        return;
    }
    leak_percentage = (1 - detected_pressure / High_pressure) * 100;
    leak_alert.data = ORGANIZE_COMMAND((leak_percentage / 5) + 1);
    send_data((uint8_t *)&leak_alert, TX_BUFFER);
}
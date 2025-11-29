#include "todo_commands.h"
#include "data_frame.h"

void DWIN_write_variable_data_vp(uint16_t vp, uint16_t data) {
  struct Tx_data_frame DataStream = {
    .Header = ORGANIZE_COMMAND(0x5AA5),
    .data_length = 0X05,
    .R_W_cmd = 0X82,
  };
  DataStream.vp = ORGANIZE_COMMAND(vp);
  DataStream.data = ORGANIZE_COMMAND(data);
  send_data((uint8_t *)&DataStream, sizeof(DataStream));
}
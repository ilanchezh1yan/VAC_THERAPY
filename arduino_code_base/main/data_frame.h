#ifndef DATA_FRAME_H
#define DATA_FRAME_H

struct data_frame {
  uint16_t Header;
  uint8_t data_length;
  uint8_t R_W_cmd;
  uint16_t vp;
  uint8_t words;
  uint8_t data_flag;
  uint8_t data;
};

#endif

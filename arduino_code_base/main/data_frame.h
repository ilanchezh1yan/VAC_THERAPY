#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#define TX_BUFFER 0x08
#define RX_BFR_SIZE 0x09

#define WRITE_COMMAND 0X82
#define READ_COMMAND 0X83

struct __attribute__ ((packed)) response_frame {
      uint16_t Header;
      uint8_t data_length;
      uint8_t instruction;
      uint16_t data;
};


struct __attribute__ ((packed)) data_frame {
  uint16_t Header;
  uint8_t data_length;
  uint8_t R_W_cmd;
  uint16_t vp;
  uint8_t words;
  union {
    uint8_t data_flag;
    uint8_t data;
  }msb;
  uint8_t data;
};

struct __attribute__ ((packed)) Tx_data_frame {
  uint16_t Header;
  uint8_t data_length;
  uint8_t R_W_cmd;
  uint16_t vp;
  uint16_t data;
};

#endif

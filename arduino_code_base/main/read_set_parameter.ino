#include "data_frame.h"
#include "todo_commands.h"

#define DEFAULT_CONST_PRESSURE    125
#define DEFAULT_LOW_PRESSURE      0
#define DEFAULT_THIGH             6
#define DEFAULT_TLOW              6

#define INITIAL_CONST_PRESSURE    50
#define INITIAL_THIGH             1
#define INITIAL_TLOW              1

#define CONST_PRESSURE            SET_CONST_PRESSURE
#define HIGH_PRESSURE             SET_HIGH_PRESSURE
#define LOW_PRESSURE              SET_LOW_PRESSURE
#define THIGH                     SET_ON_TIME 
#define TLOW                      SET_OFF_TIME

extern struct data_frame receivedData;
extern uint16_t user_cmd;

uint16_t High_pressure = DEFAULT_CONST_PRESSURE;
uint16_t low_pressure = DEFAULT_LOW_PRESSURE;

uint8_t Thigh = DEFAULT_THIGH;
uint8_t Tlow = DEFAULT_TLOW;

void read_set_parameter(void)
{
    switch(user_cmd) {
      case CONST_PRESSURE:
          High_pressure = receivedData.msb.data << 8 | receivedData.data;
          break;

      case THIGH:
          Thigh = receivedData.data;
          break;

      case TLOW:
          Tlow = receivedData.data;
          break;

      default:
          break;
      
    }
    
}

void restore_default(uint16_t select_mode, bool mode_init)
{
   struct Tx_data_frame default_data = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
    };

  if(mode_init) {
    High_pressure = INITIAL_CONST_PRESSURE;
    low_pressure = DEFAULT_LOW_PRESSURE;
    Thigh = INITIAL_THIGH;
    Tlow = INITIAL_TLOW;
  }
  else {
      High_pressure = DEFAULT_CONST_PRESSURE;
      low_pressure = DEFAULT_LOW_PRESSURE;
      Thigh = DEFAULT_THIGH;
      Tlow = DEFAULT_TLOW;
  }

  switch(select_mode) {
    case CONTINUOUS_MODE:
      default_data.vp = ORGANIZE_COMMAND(SET_CONST_PRESSURE);
      default_data.data = ORGANIZE_COMMAND(High_pressure);
      send_data((uint8_t *)&default_data, TX_BUFFER);
      break;

    case INTERMITTEN_MODE:
      default_data.vp = ORGANIZE_COMMAND(SET_CONST_PRESSURE);
      default_data.data = ORGANIZE_COMMAND(High_pressure);
      send_data((uint8_t *)&default_data, TX_BUFFER);
      default_data.vp = ORGANIZE_COMMAND(SET_ON_TIME);
      default_data.data = ORGANIZE_COMMAND(Thigh);
      send_data((uint8_t *)&default_data, TX_BUFFER);
      default_data.vp = ORGANIZE_COMMAND(SET_OFF_TIME);
      default_data.data = ORGANIZE_COMMAND(Tlow);
      send_data((uint8_t *)&default_data, TX_BUFFER);
      break;

    default:
      break;

  }
}





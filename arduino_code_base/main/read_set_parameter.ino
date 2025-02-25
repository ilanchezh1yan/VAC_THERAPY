#include "data_frame.h"
#include "todo_commands.h"

#define DEFAULT_CONST_PRESSURE    125
#define DEFAULT_LOW_PRESSURE      0
#define DEFAULT_THIGH             6
#define DEFAULT_TLOW              6

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

    //Serial.println(user_cmd);
    switch(user_cmd) {
      case CONST_PRESSURE:
          //Serial.println(receivedData.data);
          High_pressure = receivedData.msb.data << 8 | receivedData.data;
          break;

#if 0
      case LOW_PRESSURE:
          low_pressure = receivedData.msb.data << 8 | receivedData.data;
          //Serial.println(receivedData.data);
          break;
#endif

      case THIGH:
          Thigh = receivedData.data;
          break;

      case TLOW:
          Tlow = receivedData.data;
          break;

      default:
          //Serial.println("default");
          break;
      
    }
    
}

void restore_default(uint16_t select_mode)
{
   struct Tx_data_frame default_data = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
    };

  High_pressure = DEFAULT_CONST_PRESSURE;
  low_pressure = DEFAULT_LOW_PRESSURE;
  Thigh = DEFAULT_THIGH;
  Tlow = DEFAULT_TLOW;

  switch(select_mode) {
    case CONTINUOUS_MODE:
      default_data.vp = ORGANIZE_COMMAND(SET_CONST_PRESSURE);
      default_data.data = ORGANIZE_COMMAND(DEFAULT_CONST_PRESSURE);
      send_data((uint8_t *)&default_data, TX_BUFFER);
      break;

    case INTERMITTEN_MODE:
      default_data.vp = ORGANIZE_COMMAND(SET_CONST_PRESSURE);
      default_data.data = ORGANIZE_COMMAND(DEFAULT_CONST_PRESSURE);
      send_data((uint8_t *)&default_data, TX_BUFFER);
      default_data.vp = ORGANIZE_COMMAND(SET_ON_TIME);
      default_data.data = ORGANIZE_COMMAND(DEFAULT_THIGH);
      send_data((uint8_t *)&default_data, TX_BUFFER);
      default_data.vp = ORGANIZE_COMMAND(SET_OFF_TIME);
      default_data.data = ORGANIZE_COMMAND(DEFAULT_TLOW);
      send_data((uint8_t *)&default_data, TX_BUFFER);
      break;


    case CONTINUOUS_MODE_INSTIL:
      default_data.vp = ORGANIZE_COMMAND(SET_CONST_PRESSURE);
      //default_data.vp = ORGANIZE_COMMAND();
      break;

    case INTERMITTEN_MODE_INSTIL:
      default_data.vp = ORGANIZE_COMMAND(SET_CONST_PRESSURE);
      default_data.vp = ORGANIZE_COMMAND(SET_ON_TIME);
      default_data.vp = ORGANIZE_COMMAND(SET_OFF_TIME);
      //default_data.vp = ORGANIZE_COMMAND(RESTORE_DEFAULT);
      break;

    default:
      break;

  }
}



